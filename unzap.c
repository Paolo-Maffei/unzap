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

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "config.h"

#define noinline __attribute__((noinline))

/*
 * structures and variables
 */

/* these are the global system states */
typedef enum {
    IDLE = 0,       /* the system is idle, nothing is happening,
                     * this might include power-down mode */
    LOAD_CODE,      /* the main loop should load the next code and execute
                     * its generating function */
    TRANSMIT_CODE,  /* the system is transmitting the code currently loaded
                     * in the timing[] array */
    LAST_ON_PULSE,  /* the system is just transmitting the last on pulse, afterwards
                     * the current code transmission is complete */
    SLEEP,          /* processor should sleep */
} state_t;

/* structure for efficiently addressing pwm and repeat parameters in code table */
struct params_t {
    union {
        struct {
            uint8_t repeat;     /* low byte is repeat count */
            uint8_t pwm;        /* high byte is pwm value */
        };
        struct {
            uint8_t bits;
            uint8_t flags;
        };
        uint16_t raw;
    };
};

/*
 * global variables
 */
volatile state_t state = IDLE;
volatile uint8_t tccr0b;    /* holds the value for TCCR0B register, set by pwm_set() */

#define BUTTONS 1
volatile uint8_t button_buf[BUTTONS]; /* button states and debouncing buffers */
volatile uint8_t button_timeout[BUTTONS]; /* timeout, if nonzero, decrement and skip button sampling */

volatile uint16_t timing[376];  /* hold on/off values for the current timing */
volatile uint16_t *current_timing;      /* pointer to the current location in the timing table */

volatile uint16_t *current_code;        /* pointer to the current location in the code table */

volatile uint8_t sleep_counter;

/*
 * prototypes
 */

uint16_t noinline next_word(void);

void send_raw(void);
void send_nec(void);
void send_pause(void);

/* helper macros */
#define HI8(x)  ((uint8_t)((x) >> 8))
#define LO8(x)  ((uint8_t)(x))

/* flags for send_pause */
#define REPEAT_HEADER _BV(0)
#define REPEAT_SUBTRACT_HEADER _BV(1)

/* the code table */
uint16_t PROGMEM codes[] = \
{
    #include "codes.h"

    /* terminate this list by two nullwords */
    0, 0
};

/*
 * pwm macros
 */
#define pwm_enable() { \
    /* setup timer0: fast pwm mode, clear OC0B on match, prescaler 8 */ \
    TCCR0A = _BV(WGM00) | _BV(COM0B1); \
    TCCR0B = tccr0b;   \
}
#define pwm_disable() { \
    TCCR0A = 0;         \
    TCCR0B = 0;         \
    TCNT0 = 0;          \
}
#define pwm_set(freq) { \
    if (freq > 127)  \
        tccr0b = _BV(CS00) | _BV(WGM02);   \
    else                                   \
        tccr0b = _BV(CS01) | _BV(WGM02);   \
    OCR0A = freq;       \
    OCR0B = freq/8;     \
}

/* this compare match interrupt is used to turn on pwm at the beginnig of a on/off cycle */
ISR(TIMER1_COMPA_vect)
{

    pwm_enable();
    PORTB &= ~_BV(PB2);

    /* load on pulse duration */
    OCR1B = *current_timing++;

    /* load off pulse duration,
     * if this is zero, disable compare match interrupt A, and update state */
    uint16_t off = *current_timing++;
    if (off == 0) {
        TIMSK1 = _BV(OCIE1B);
        state = LAST_ON_PULSE;
    } else {
        OCR1A = off;
    }
}

/* this compare match interrupt is used to turn off pwm */
ISR(TIMER1_COMPB_vect)
{
    pwm_disable();
    PORTB |= _BV(PB2);

    if (state == LAST_ON_PULSE) {
        /* stop timer1, disable interrupts and update state */
        TCCR1B = 0;
        TIMSK1 = 0;
        state = LOAD_CODE;
    }

}

/* button debouncing timer */
ISR(TIMER2_OVF_vect)
{
    for (uint8_t i = 0; i < BUTTONS; i++) {
        if (button_timeout[i]) {
            button_timeout[i]--;
        } else {
            button_buf[i] <<= 1;
            switch (i) {
                case 0: if (!(PIND & _BV(PD2)))
                            button_buf[0] |= _BV(0);
                        break;
#if BUTTONS > 1
                case 1: if (!(PIND & _BV(PD4)))
                            button_buf[1] |= _BV(0);
                        break;
#endif
            }
        }
    }

    if (state == IDLE) {
        sleep_counter++;

        if (sleep_counter == 0)
            state = SLEEP;
    }
}

/* pin change interrupt 2 does nothing, enabled just for wakeup */
ISR(PCINT2_vect) {}

/* helper functions */
uint16_t noinline next_word(void)
{

    return pgm_read_word(current_code++);

}

/* sending functions */
void send_raw(void)
{
    /* load pwm timing, retransmit and retransmit timeout */
    struct params_t params;
    params.raw = next_word();
    uint16_t retransmit_delay = next_word();

    /* remember positing in timing table */
    uint16_t pos = 0;
    uint16_t *ptr;

    for (uint8_t r = 0; r <= params.repeat; r++) {
        /* load the beginning of the current timing sequence */
        ptr = (uint16_t*)current_code;

        /* copy all timing values up to the terminating nullword */
        uint16_t t;
        while ( (t = pgm_read_word(ptr++)) != 0 )
            timing[pos++] = t;

        /* terminate this sequency with the repeat delay */
        timing[pos++] = retransmit_delay;
    }

    /* terminate the whole sequency with a zero */
    timing[pos] = 0;

    /* remember positing in code table */
    current_code = ptr;

    /* set loaded pwm value */
    pwm_set(params.pwm);
}

void send_nec(void)
{
    /* load pwm timing, retransmit and retransmit timeout */
    struct params_t params;
    params.raw = next_word();
    uint16_t retransmit_delay = next_word();

    /* load data */
    uint16_t data[2];
    data[0] = next_word();
    data[1] = next_word();

    /* add header */
    timing[0] = NEC_HEADER_ON;
    timing[1] = NEC_HEADER_OFF;

    /* remember positing in timing table, calculate retransmit timeout */
    uint16_t pos = 2;
    uint16_t retransmit = retransmit_delay - NEC_HEADER_OFF;

    /* compute 32 bits */
    for (uint8_t j = 0; j < 2; j++) {
        for (uint8_t i = 0; i < 16; i++) {

            /* on timing */
            timing[pos++] = NEC_ON;

            if (data[j] & 1)
                /* one */
                timing[pos] = NEC_OFF_ONE;
            else
                /* zero */
                timing[pos] = NEC_OFF_ZERO;

            /* subtract cycle length from retransmit */
            retransmit -= timing[pos];
            pos++;

            data[j] >>= 1;
        }
    }

    /* insert last on pulse */
    timing[pos++] = NEC_ON;

    /* insert retransmits */
    for (uint8_t i = 0; i < params.repeat; i++) {
        timing[pos++] = retransmit;
        timing[pos++] = NEC_REPEAT_ON;
        timing[pos++] = NEC_REPEAT_OFF;
        timing[pos++] = NEC_ON;
        retransmit = retransmit_delay - NEC_REPEAT_OFF - NEC_ON;
    }

    /* mark end of code */
    timing[pos] = 0;

    /* set loaded pwm value */
    pwm_set(params.pwm);
}

void send_pause(void)
{
    /* load pwm timing, retransmit and retransmit timeout */
    struct params_t params;
    params.raw = next_word();
    uint16_t retransmit_delay = next_word();

    /* load header */
    uint16_t header_on = next_word();
    uint16_t header_len = next_word();

    /* load timing values */
    uint16_t on = next_word();
    uint16_t one = next_word();
    uint16_t zero = next_word();

    /* load data */
    struct params_t params2;
    params2.raw = next_word();

    /* remember positing in timing table */
    uint16_t pos = 0;

    /* calculate retransmit timeout */
    uint16_t retransmit;

    uint16_t *dataptr;

    for (uint8_t i = 0; i <= params.repeat; i++) {

        dataptr = (uint16_t *)current_code;

        retransmit = retransmit_delay;

        if (header_on != 0) {

            if (i == 0 || (params2.flags & REPEAT_HEADER)) {
                /* add header */
                timing[pos] = header_on;
                timing[pos+1] = header_len;
                pos += 2;
            }

            if (i == 0 || (params2.flags & REPEAT_SUBTRACT_HEADER)) {
                /* calculate retransmit timeout */
                retransmit -= header_len;
            }
        }

        uint16_t data = 0;

        /* insert data into timing array */
        for (uint8_t b = 0; b < params2.bits; b++) {
            if (b % 16 == 0) {
                /* load data */
                data = pgm_read_word(dataptr++);
            }

            /* on timing */
            timing[pos++] = on;

            if (data & 1) {
                /* one */
                timing[pos++] = one;

                /* subtract cycle length from retransmit */
                retransmit -= one;
            } else {
                /* zero */
                timing[pos++] = zero;

                /* subtract cycle length from retransmit */
                retransmit -= zero;
            }

            data >>= 1;
        }

        /* insert last on pulse and retransmit delay */
        timing[pos++] = on;
        timing[pos++] = retransmit+on;

        /* reset retransmit delay */
        retransmit = retransmit_delay;
    }

    /* remember positing in code table */
    current_code = dataptr;

    /* mark end of code */
    timing[pos-1] = 0;

    /* set loaded pwm value */
    pwm_set(params.pwm);
}

/*
 * main function
 */
int main(void)
{
    /* hardware on port D:
     * PD0: RX
     * PD1: TX
     * PD2: BTN1
     * PD3: INT1/IR_IN
     * PD4: BTN2
     * PD5: IR_OUT
     * PD6: BTN3
     */
    DDRD = _BV(PD5);
    PORTD = _BV(PD2) | _BV(PD4) | _BV(PD6);

    /* configure pin change interrupt for button 1 and 2,
     * for waking up from sleep mode... */
    PCMSK2 = _BV(PCINT18) | _BV(PCINT20);

    /* hardware on port B:
     * PB0: BTN4
     * PB1: /LED2
     * PB2: /LED1
     */
    DDRB = _BV(PB1) | _BV(PB2);
    PORTB = _BV(PB0) | _BV(PB1) | _BV(PB2);

    /* init timer2 for key debouncing, prescaler 256 */
    TCCR2B = _BV(CS22) | _BV(CS20);
    TIMSK2 = _BV(TOIE2);

#ifdef DEBUG_UART
    /* uart */
    UBRR0H = 0;
    UBRR0L = 8;
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
    UCSR0B = _BV(TXEN0);

    UDR0 = 'b';
    while(!(UCSR0A & _BV(UDRE0)));
#endif

    sei();

    uint8_t pos = 0;

    while (1)
    {
        if (button_buf[0] == 0xff) {
            button_buf[0] = 0;
            button_timeout[0] = 100;

            /* if we're idle, reset pointer and start transmitting */
            if (state == IDLE) {
                pos = 0;

                current_code = &codes[0];
                state = LOAD_CODE;
                PORTB &= ~_BV(PB1);
            }
        }

        if (state == LOAD_CODE) {
#ifdef DEBUG_UART
            UDR0 = 'L';
            while(!(UCSR0A & _BV(UDRE0)));
#endif

            /* load next generating function and generate timing table */
            void (*func)(void);
            uint16_t ptr = next_word();
            func = (void *)ptr;

            if (func != NULL && (PIND & _BV(PD4))) {
#ifdef DEBUG_UART
                UDR0 = 'p';
                while(!(UCSR0A & _BV(UDRE0)));
                UDR0 = pos++;
                while(!(UCSR0A & _BV(UDRE0)));
#endif

                /* call generating function */
                func();

                UDR0 = 'f';
                while(!(UCSR0A & _BV(UDRE0)));

                /* reset timing pointer */
                current_timing = &timing[0];

                /* update state */
                state = TRANSMIT_CODE;

                /* init timer1 for initial delay before sending:
                 * prescaler 64, CTC mode (TOP == OCR1A)
                 * enable compare interrupts */
                OCR1A = DELAY_NEXT_CODE;
                OCR1B = 0xffff;
                TIFR1 = _BV(OCF1A) | _BV(OCF1B);
                TIMSK1 = _BV(OCIE1A) | _BV(OCIE1B);
                TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);
            } else {
#ifdef DEBUG_UART
                UDR0 = 'E';
                while(!(UCSR0A & _BV(UDRE0)));
#endif

                PORTB |= _BV(PB1);
                state = IDLE;
            }

            sleep_counter = 0;
        }

        if (state == SLEEP) {
#ifdef DEBUG_UART
            UDR0 = 'S';
            while(!(UCSR0A & _BV(UDRE0)));
            while(!(UCSR0A & _BV(TXC0)));
#endif
            /* enable pin change interrupt for button 1 and 2,
             * for waking up from sleep mode... */
            PCICR = _BV(PCIE2);

            /* set and enter sleep mode */
            set_sleep_mode(SLEEP_MODE_STANDBY);
            sleep_enable();
            sleep_cpu();
            sleep_disable();
            sleep_counter = 0;
            state = IDLE;

            /* disable pin change interrupt for button 1 and 2,
             * for waking up from sleep mode... */
            PCICR = 0;

        }
    }
}
