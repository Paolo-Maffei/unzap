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
#include <avr/interrupt.h>

#include "ir.h"
#include "config.h"
#include "global.h"
#include "ui.h"
#include "pt/pt.h"
#include "debug.h"

typedef union {
    uint8_t raw;
    struct {
        uint8_t recording:1;
    };
} ir_flags_t;

/* use GPIOR0 as the flag register */
#define ir_flags ((volatile ir_flags_t *)(&GPIOR0))

#define IR_TIMESLOTS 250

static struct pt ir_thread;
volatile uint16_t timing[IR_TIMESLOTS];
volatile uint8_t timing_top;

void ir_init(void)
{
    /* configure pin-change interrupt */
    PCMSK2 = _BV(PCINT19);

    /* configure ir output pin */
    DDRD |= _BV(PD5);
    PORTD &= ~_BV(PD5);

    /* init ir flags */
    ir_flags->raw = 0;

    PT_INIT(&ir_thread);
}

ISR(PCINT2_vect, ISR_NOBLOCK)
{
    if (ir_flags->recording) {
        if (timing_top < IR_TIMESLOTS) {
            /* save this measure and reset timer */
            uint16_t time = TCNT1;
            TCNT1 = 0;
            timing[timing_top++] = time;

            LED1_TOGGLE();
        }
    } else {
        /* reset timer, set running flag, toggle led */
        TCNT1 = 0;
        ir_flags->recording = 1;
        LED1_TOGGLE();
    }

    /* reset overflow flag */
    TIFR1 = _BV(TOV1);
}


static PT_THREAD(ir_record_thread(struct pt *thread))
{
    PT_BEGIN(thread);

    while(1) {

        PT_WAIT_UNTIL(thread, global.mode == MODE_RECORD);

        /* reset timing */
        timing_top = 0;
        ir_flags->recording = 0;

        /* reset and init timer1, prescaler 256 */
        TCNT1 = 0;
        TCCR1A = 0;
        TCCR1B = _BV(CS12);
        TIMSK1 = 0;

        /* enable pin-change interrupt */
        PCIFR = PCIFR;
        PCICR |= _BV(PCIE2);

        /* wait complete recording */
        PT_WAIT_UNTIL(thread, ir_flags->recording);
        PT_WAIT_UNTIL(thread, TIFR1 & _BV(TOV1));

        /* disable timer */
        TCCR1B = 0;

        /* disable pin change interrupt */
        PCICR &= ~_BV(PCIE2);

        ui_blink(0x33, 0);

#if 0
        for (uint8_t i = 0; i < timing_top; i++) {
            debug_putc(HI8(timing[i]));
            debug_putc(LO8(timing[i]));
        }
#endif

        global.mode = MODE_PLAY;
    }

    PT_END(thread);
}

void ir_poll(void)
{
    PT_SCHEDULE(ir_record_thread(&ir_thread));
}
