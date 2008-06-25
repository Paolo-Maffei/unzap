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
#include "pt/pt.h"

/* module local variables */

static struct pt btn_thread;
static uint8_t btn_state;
static uint8_t btn_last_sample;
static uint8_t btn_press;


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

    /* buttons are idle at start (internal pullups active) */
    btn_state = _BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN);
    btn_last_sample = _BV(BTN1_PIN) | _BV(BTN2_PIN) | _BV(BTN3_PIN) | _BV(BTN4_PIN);
    btn_press = 0;
}

/* blink out a sequence (LSB first), every bit is 200ms long */
void ui_blink(uint8_t sequence1, uint8_t sequence2, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        /* set led1 */
        if (sequence1 & 1)
            LED1_ON();
        else
            LED1_OFF();
        sequence1 >>= 1;

        /* set led2 */
        if (sequence2 & 1)
            LED2_ON();
        else
            LED2_OFF();
        sequence2 >>= 1;

        /* wait 200ms */
        for (uint8_t t = 0; t < 20; t++)
            _delay_loop_2(F_CPU/100/4);
    }

    /* turn off leds */
    LED1_OFF();
    LED2_OFF();
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

/* poll for user actions */
void ui_poll(void)
{
    /* sample buttons */
    PT_SCHEDULE(ui_sample_buttons(&btn_thread));

    if (btn_press) {
        debug_putc(btn_press);
        btn_press = 0;
    }
}
