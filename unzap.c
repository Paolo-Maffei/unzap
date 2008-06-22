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

#define df_select() DF_PORT &= ~_BV(DF_CS_PIN)
#define df_release() DF_PORT |= _BV(DF_CS_PIN)

/*
 * structures and variables
 */

/* these are the global system states */
typedef enum {
    IDLE = 0,       /* the system is idle, nothing is happening,
                     * this might include power-down mode */
    LOAD_CODE,      /* the main loop should load the next code and execute
                     * its generating function */
    READ_COMMAND,       /* wait for more button presses */
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

#define BUTTONS 3
volatile uint8_t button_state;              /* current button states */
volatile uint8_t button_sample;             /* old button sample */
volatile uint8_t button_press[BUTTONS];     /* counting button presses,
                                             * incremented on button release */

volatile uint16_t timing[376];  /* hold on/off values for the current timing */
volatile uint16_t *current_timing;      /* pointer to the current location in the timing table */

volatile uint16_t *current_code;        /* pointer to the current location in the code table */
uint16_t *single_code;                  /* pointer to the current code, in single-step mode */

volatile uint8_t sleep_counter;

uint8_t toggle_bit;                     /* global toggle-bit for rc5 */

struct {
    uint8_t silent:1;       /* set to one, if the transmit should
                             * be done without flashing leds */
    uint8_t single_step:1;  /* wait for keypress between codes */
    uint8_t ignore_bat:1;   /* ignore battery voltage */
} options;

/*
 * prototypes
 */

uint16_t noinline next_word(void);

void send_raw(void);
void send_nec(void);
void send_pause(void);
void send_rc5(void);

uint8_t battery_voltage(void);

void blink(uint8_t sequence1, uint8_t sequence2, uint8_t len);

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
    /* switch prescaler to 64 */
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

    pwm_enable();
    if (!options.silent)
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
ISR(TIMER2_COMPA_vect)
{
    uint8_t new_sample = 0;

    /* read button 1 */
    if ((PINC & _BV(PC5)) > 0)
        new_sample |= _BV(0);

    /* read button 2 */
    if ((PINC & _BV(PC4)) > 0)
        new_sample |= _BV(1);

    /* read button 3 */
    if ((PINC & _BV(PC3)) > 0)
        new_sample |= _BV(2);

    /* if something changed */
    if (button_sample != new_sample) {

        /* iterate through all the buttons */
        for (uint8_t i = 0; i < BUTTONS; i++) {

            /* check if button state is different from last two samples */
            uint8_t s = button_sample & _BV(i);
            if ( s ^ (button_state & _BV(i)) ) {
                /* update button state */
                if (s) {
                    button_state |= _BV(i);
                } else {
                    /* if this is the keypress, increment button press counter */
                    button_state &= ~_BV(i);
                    button_press[i]++;
                }
            }
        }
    }

    /* remember sample */
    button_sample = new_sample;

    /* increment sleep counter */
    if (state == IDLE) {
        sleep_counter++;

        if (sleep_counter == 0)
            state = SLEEP;
    } else
        sleep_counter = SLEEP_COUNTER_VALUE;

}

/* pin change interrupt 2 does nothing, enabled just for wakeup */
ISR(PCINT1_vect) {}

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

void send_rc5(void)
{
    /* load pwm timing, retransmit and retransmit timeout */
    struct params_t params;
    params.raw = next_word();
    uint16_t retransmit_delay = next_word();

    /* load data */
    struct params_t dataparam;
    dataparam.raw = next_word();

    /* remember positing in timing table */
    uint16_t pos = 0;

    /* calculate retransmit timeout */
    uint16_t retransmit = retransmit_delay;

    for (uint8_t i = 0; i <= params.repeat; i++) {

        retransmit = retransmit_delay -= 3*RC5_TIME;

        uint16_t data = dataparam.raw;

        timing[pos++] = RC5_TIME; /* on */
        timing[pos++] = RC5_TIME; /* off */
        timing[pos++] = RC5_TIME; /* on */

        if (toggle_bit) {
            /* insert off-on */
            timing[pos] = RC5_TIME; /* off */
            timing[pos+1] = RC5_TIME; /* on */
            pos+=2;
        } else {
            /* insert on-off */
            timing[pos-1] += RC5_TIME; /* double on time */
            timing[pos] = RC5_TIME; /* off */
            pos++;
        }

        uint8_t old_bit = toggle_bit;

        /* insert data into timing array */
        for (uint8_t b = 0; b < 11; b++) {

            uint8_t bit = data & 1;

            if ( bit ^ old_bit) {
                /* if old bit and current bit are different, increase last timing */
                timing[pos-1] += RC5_TIME;
                timing[pos] = RC5_TIME;
                pos++;
                old_bit = bit;
            } else {
                /* else if both bits are the same, insert normal cycles */
                timing[pos++] = RC5_TIME;
                timing[pos++] = RC5_TIME;
            }

            /* subtract cycle length from retransmit */
            retransmit -= 2*RC5_TIME;
            data >>= 1;
        }

        /* if pos is even, the last period is off, insert repeat delay there */
        if (pos % 2 == 0) {
            timing[pos-1] = retransmit;
        /* else the last period is on, add repeat delay as new timing */
        } else {
            timing[pos++] = retransmit;
        }

        /* reset retransmit delay */
        retransmit = retransmit_delay;
    }

    /* now timing[] is an array of on-off times. iterate over it, and
     * correct the off times */
    for (uint8_t j = 0; j < pos; j+=2) {
        timing[j+1] += timing[j];
    }

    /* flip toggle bit */
    toggle_bit = !toggle_bit;

    /* mark end of code */
    timing[pos-1] = 0;

    /* set loaded pwm value */
    pwm_set(params.pwm);
}

/* adc function */
uint8_t battery_voltage(void)
{
    /* start conversion */
    ADCSRA |= _BV(ADSC) | _BV(ADIF);

    /* wait for conversion */
    while(!(ADCSRA & _BV(ADIF)));

    return ADCH;
}

/* user interface functions */

/* blink out a sequence (LSB first), every bit is 200ms long */
void noinline blink(uint8_t sequence1, uint8_t sequence2, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        /* set led1 */
        if (sequence1 & 1)
            PORTB &= ~_BV(PB2);
        else
            PORTB |= _BV(PB2);
        sequence1 >>= 1;

        /* set led2 */
        if (sequence2 & 1)
            PORTB &= ~_BV(PB1);
        else
            PORTB |= _BV(PB1);
        sequence2 >>= 1;

        /* wait 150ms */
        for (uint8_t t = 0; t < 15; t++)
            _delay_loop_2(F_CPU/100/4);
    }

    /* turn off leds */
    PORTB |= _BV(PB1) | _BV(PB2);
}

/*
 * main function
 */
int main(void)
{
    /* hardware on port D:
     * PD0: RX
     * PD1: TX
     * PD2: D+
     * PD3: INT1/IR_IN
     * PD4: D-
     * PD5: IR_OUT
     * PD7: DF_CS
     */
    DDRD = _BV(PD5) | _BV(PD7);
    PORTD = _BV(PD7);

    /* configure pin change interrupt for button 1 and 2,
     * for waking up from sleep mode... */
    PCMSK1 = _BV(PCINT12) | _BV(PCINT13);

    /* hardware on port B:
     * PB0: PUD
     * PB1: /LED2
     * PB2: /LED1
     * PB3: MOSI
     * PB4: MISO
     * PB5: SCK
     */
    DDRB = _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB5);
    PORTB = _BV(PB1) | _BV(PB2);

    /* hardware on port C:
     * PC0: IR_IN2
     * PC1: POWER
     * PC2: BTN4
     * PC3: BTN3
     * PC4: BTN2
     * PC5: BTN1
     */
    DDRC = 0;
    PORTC = _BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5);

    /* init analog to digital converter,
     * convert on PC1(ADC1)
     * prescaler 64 => 250khz frequency */
    ADMUX = _BV(REFS0) | _BV(ADLAR) | _BV(ADIF) | _BV(MUX0);
    ADCSRA = _BV(ADEN) | _BV(ADPS1);

    /* init spi */
    SPCR = _BV(SPE) | _BV(MSTR);
    SPSR |= _BV(SPI2X);

    /* init timer2 for key debouncing, CTC, prescaler 1024,
     * timeout after 10ms */
    TCCR2A = _BV(WGM21);
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
    TIMSK2 = _BV(OCIE2A);
    OCR2A = F_CPU/100/1024;

    /* configure sleep mode */
    set_sleep_mode(SLEEP_MODE_STANDBY);

#ifdef DEBUG_UART
    /* uart */
    UBRR0H = 0;
    UBRR0L = 8;
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
    UCSR0B = _BV(TXEN0);

    UDR0 = 'b';
    while(!(UCSR0A & _BV(UDRE0)));
#endif

    /* read dataflash status */
    df_select();
    SPDR = 0xd7;
    while(!(SPSR & _BV(SPIF)));
    SPDR = 0;
    while(!(SPSR & _BV(SPIF)));

    uint8_t df_status = SPDR;

    /* read battery voltage */
    uint8_t bat = battery_voltage();

#ifdef DEBUG_UART
    UDR0 = 'D';
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = df_status;
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = 'V';
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = bat;
    while(!(UCSR0A & _BV(UDRE0)));
#endif

    df_release();

    sei();

#ifdef BLINK_START
    /* blink start sequence */
    blink(BLINK_START);
#endif

    if (df_status == DF_STATUS_IDLE)
        blink(BLINK_DF_SEEN);
    else
        blink(BLINK_DF_ERROR);

    if (bat >= MIN_BAT_VOLTAGE)
        blink(BLINK_BAT_OK);
    else
        blink(BLINK_BAT_FAIL);

    uint8_t pos;
    uint8_t button_sum = 0;

    while (1)
    {
        /* if a button has been pressed and we're idle, wait some more time to determine mode */
        if (state == IDLE && (button_press[0] > 0 || button_press[1] > 0)) {

            /* wait one second via timer1, using prescaler 1024 */
            TIFR1 = TIFR1;
            OCR1A = F_CPU/1024/2;
            TCCR1B = _BV(CS12) | _BV(CS10);

            button_sum = 0;
            state = READ_COMMAND;
        }

        /* if we're waiting for a command, and some button has been pressed, reset timeout */
        uint8_t sum = button_press[0] + button_press[1];
        if (state == READ_COMMAND && sum != button_sum) {
            TCNT1 = 0;
            button_sum = sum;
        }

        /* if we're waiting for a command, check if the timer expired */
        if (state == READ_COMMAND && (TIFR1 & _BV(OCF1A))) {

            /* disable timer1 */
            TCCR1B = 0;
            TCNT1 = 0;

            /* parse button presses */
            if (button_press[0] == 1 && button_press[1] == 0) {

                if (battery_voltage() < MIN_BAT_VOLTAGE && !options.ignore_bat) {
                    blink(BLINK_VOLTAGE);
                } else {

                    /* start transmitting */
                    pos = 0;

                    current_code = &codes[0];
                    single_code = &codes[0];
                    state = LOAD_CODE;

#ifdef BLINK_MODE1
                    /* blink mode 1 */
                    blink(BLINK_MODE1);
#endif

                    if (!options.silent)
                        PORTB &= ~_BV(PB1);
                }
            } else if (button_press[0] == 0) {
                if (button_press[1] == 1) {
                    options.silent = !options.silent;

                    /* blink for silent toggle */
                    if (options.silent)
                        blink(BLINK_SILENT);
                    else
                        blink(BLINK_NONSILENT);

                } else if (button_press[1] == 2) {
                    options.single_step = !options.single_step;

                    /* blink for single step toggle */
                    if (options.single_step)
                        blink(BLINK_STEP);
                    else
                        blink(BLINK_NOSTEP);
                } else if (button_press[1] == 3) {
                    options.ignore_bat = !options.ignore_bat;

                    /* blink for ignore battery toggle */
                    if (options.ignore_bat)
                        blink(BLINK_BAT_IGNORE);
                    else
                        blink(BLINK_BAT_HONOR);
                } else
                    blink(BLINK_INVALID);
            } else
                blink(BLINK_INVALID);

            /* reset state, if not yet done */
            if (state == READ_COMMAND)
                state = IDLE;

            /* reset buttons */
            button_press[0] = 0;
            button_press[1] = 0;
            button_press[2] = 0;
        }

        if (state == LOAD_CODE) {

            /* if we're in single-step mode, wait for a keypress */
            if (options.single_step) {
                while (button_press[0] == 0 && button_press[1] == 0 && button_press[2] == 0);

                /* send next code if button1 has been pressed, stop if button2
                 * has been pressed, resend code if button3 has been pressed */
                if (button_press[2]) {
                    current_code = single_code;
                    button_press[1] = 0;
                } else if (!button_press[1]) {
                    button_press[1] = 0;
                }

                /* reset buttons */
                button_press[0] = 0;
                button_press[2] = 0;
            }

            /* stop sending if button2 has been pressed */
            if (button_press[1] > 0) {
                button_press[0] = 0;
                button_press[1] = 0;
                PORTB |= _BV(PB1);

#ifdef BLINK_MODE1_END
                blink(BLINK_MODE1_END);
#endif

                state = IDLE;
            } else {

#ifdef DEBUG_UART
                UDR0 = 'L';
                while(!(UCSR0A & _BV(UDRE0)));
#endif
                /* if we're in single step mode, remember code address for single-retransmit */
                single_code = current_code;

                /* load next generating function and generate timing table */
                void (*func)(void);
                uint16_t ptr = next_word();
                func = (void *)ptr;

                if (func != NULL) {
#ifdef DEBUG_UART
                    UDR0 = 'p';
                    while(!(UCSR0A & _BV(UDRE0)));
                    UDR0 = pos++;
                    while(!(UCSR0A & _BV(UDRE0)));
#endif

                    /* call generating function */
                    func();

#ifdef DEBUG_UART
                    UDR0 = 'f';
                    while(!(UCSR0A & _BV(UDRE0)));
#endif

                    /* reset timing pointer */
                    current_timing = &timing[0];

                    /* update state */
                    state = TRANSMIT_CODE;

                    /* init timer1 for initial delay before sending:
                     * prescaler 256, CTC mode (TOP == OCR1A)
                     * enable compare interrupts */
                    OCR1A = DELAY_NEXT_CODE;
                    OCR1B = 0xffff;
                    TIFR1 = TIFR1;
                    TIMSK1 = _BV(OCIE1A) | _BV(OCIE1B);
                    TCCR1B = _BV(CS12) | _BV(WGM12);
                } else {
#ifdef DEBUG_UART
                    UDR0 = 'E';
                    while(!(UCSR0A & _BV(UDRE0)));
#endif

                    PORTB |= _BV(PB1);

#ifdef BLINK_MODE1_END
                    blink(BLINK_MODE1_END);
#endif

                    state = IDLE;
                }

                sleep_counter = 0;
            }
        }

        if (state == SLEEP) {
            /* enable pin change interrupt for button 1 and 2,
             * for waking up from sleep mode... */
            PCICR = _BV(PCIE1);

            /* set and enter sleep mode */
            sleep_mode();
            sleep_counter = SLEEP_COUNTER_VALUE;
            state = IDLE;

            /* disable pin change interrupt for button 1 and 2,
             * for waking up from sleep mode... */
            PCICR = 0;

        }
    }
}
