/*
 * unzap firmware
 *
 * (c) by Alexander Neumann <alexander@lochraster.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "ui.h"
#include "timer.h"
#include "debug.h"
#include "usb.h"
#include "global.h"
#include "pt/pt.h"

/* module local variables */

static struct pt btn_thread;
/* buttons are idle at start (internal pullups active) */
static uint8_t btn_state = _BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN);
static uint8_t btn_last_sample = _BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN);
static uint8_t btn_press = 0;

static struct pt blink_thread;
static uint8_t blink_seq_led1 = 0;
static uint8_t blink_seq_led2 = 0;

static struct pt input_thread;

/* initialize the button and led pins */
void ui_init(void)
{
    /* initialize buttons */
    BTN_DDR &= ~(_BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN));
    BTN_PORT |= _BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN);

    /* initialize leds */
    LED_DDR |= _BV(LED1_PIN) | _BV(LED2_PIN);
    LED_PORT |= _BV(LED1_PIN) | _BV(LED2_PIN);

    /* initialize button sample thread and variables */
    PT_INIT(&btn_thread);

    /* initialize blink thread and variables */
    PT_INIT(&blink_thread);

    /* initialize input thread */
    PT_INIT(&input_thread);
}

/* blink out a sequence (LSB first), every bit is 150ms long */
void ui_blink(uint8_t sequence1, uint8_t sequence2)
{
    blink_seq_led1 = sequence1;
    blink_seq_led2 = sequence2;
}

/* check if the current blink sequency is done */
bool ui_blinking(void)
{
    return !(blink_seq_led1 || blink_seq_led2);
}

/* sample buttons, set bit in btn_press if a button has been pressed */
static PT_THREAD(ui_sample_buttons(struct pt *thread))
{
    /* make sure no variable is created on the stack */
    static timer_t btn_timer;
    static uint8_t btn_sample;

    PT_BEGIN(thread);

    while(1) {

        /* only execute this thread every 10ms */
        timer_set(&btn_timer, 1);
        PT_WAIT_UNTIL(thread, timer_expired(&btn_timer));

        /* sample buttons */
        btn_sample = BTN_PIN & (_BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN));

        /* mark bits which stayed the same since last sample */
        btn_last_sample ^= ~btn_sample;

        /* mark bits which have not changed, but whose state is different */
        btn_last_sample = btn_state ^ (btn_sample & btn_last_sample);

        /* if old state is one (button not pressed), new state is zero (button pressed),
         * so set these bits in btn_press */
        btn_press |= btn_last_sample & btn_state;

        /* remember new state and last sample */
        btn_state ^= btn_last_sample;
        btn_last_sample = btn_sample;
    }

    PT_END(thread);
}

/* execute led blinking */
static PT_THREAD(ui_do_blinking(struct pt *thread))
{
    static timer_t t;

    PT_BEGIN(thread);

    while (1) {

        /* wait until there is something to do */
        PT_WAIT_UNTIL(thread, blink_seq_led1 || blink_seq_led2);

        /* turn leds on/off, according to blink sequence */
        if (blink_seq_led1 & 1)
            LED1_ON();
        else
            LED1_OFF();

        if (blink_seq_led2 & 1)
            LED2_ON();
        else
            LED2_OFF();

        blink_seq_led1 >>= 1;
        blink_seq_led2 >>= 1;

        /* wait 150ms */
        timer_set(&t, 15);
        PT_WAIT_UNTIL(thread, timer_expired(&t));

        /* turn off leds */
        LED1_OFF();
        LED2_OFF();

    }

    PT_END(thread);
}

/* parse input */
static PT_THREAD(ui_input(struct pt*thread))
{
    static timer_t t;
    static uint8_t option_set;

    PT_BEGIN(thread);

    while(1) {

        /* if some button has been pressed, extend reset timeout to 800ms */
        if (btn_press)
            timer_set(&t, 80);

        /* handle button 2 press */
        if (btn_press & _BV(BTN2_PIN))
            option_set++;

        /* handle button 4 press */
        if (btn_press & _BV(BTN4_PIN)) {
            if (usb_enabled()) {
                usb_disable();
                ui_blink(0, 5);
            } else {
                usb_enable();
                ui_blink(5, 0);
            }
        }

        /* reset button presses */
        btn_press = 0;

        /* if timer expired and an option has been set, parse */
        if (option_set && timer_expired(&t)) {

            if (option_set > 3)
                ui_blink(BLINK_INVALID);
            else {
                /* compute option bit number */
                option_set--;

                if (option_set < 3) {
                    /* toggle option */
                    global.opts.raw ^= _BV(option_set);

                    /* blink result */
                    option_set = global.opts.raw & _BV(option_set);
                }

                if (option_set)
                    ui_blink(BLINK_OPTION_SET);
                else
                    ui_blink(BLINK_OPTION_UNSET);
            }

            option_set = 0;
        }

        PT_YIELD(thread);
    }

    PT_END(thread);
}

/* poll for user actions */
void ui_poll(void)
{
    /* sample buttons */
    PT_SCHEDULE(ui_sample_buttons(&btn_thread));

    /* do blinking */
    PT_SCHEDULE(ui_do_blinking(&blink_thread));

    /* process input */
    PT_SCHEDULE(ui_input(&input_thread));
}
