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
#include "usbdrv/usbdrv.h"

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

struct buttons_t {
    uint8_t state;      /* current button states */
    uint8_t sample;     /* old button sample */
    uint8_t press[BUTTONS];   /* count button presses, incremented on button release */
    uint8_t done;       /* set to one when buttonpress sequence is done,
                           aka there has not been any button activity for the last second,
                           must be reset by main loop before new button activity is recorded */
    uint8_t timeout;    /* incremented for each iteration of the button ISR */
};

struct options_t {
    uint8_t usb:1;      /* set to one if usb is currently enabled */
};

volatile struct buttons_t btn;
struct options_t opt;


volatile uint16_t timing[376];  /* hold on/off values for the current timing */
volatile uint16_t *current_timing;      /* pointer to the current location in the timing table */

struct {
    uint8_t silent:1;       /* set to one, if the transmit should
                             * be done without flashing leds */
    uint8_t single_step:1;  /* wait for keypress between codes */
} options;

/*
 * prototypes
 */

uint16_t noinline next_word(void);

void blink(uint8_t sequence1, uint8_t sequence2, uint8_t len);


/*
 * helper macros
 */

#define HI8(x)  ((uint8_t)((x) >> 8))
#define LO8(x)  ((uint8_t)(x))

#define LED1_ON() LED_PORT &= ~_BV(LED1_PIN)
#define LED1_OFF() LED_PORT |= _BV(LED1_PIN)
#define LED2_ON() LED_PORT &= ~_BV(LED2_PIN)
#define LED2_OFF() LED_PORT |= _BV(LED2_PIN)

#define df_select() DF_PORT &= ~_BV(DF_CS_PIN)
#define df_release() DF_PORT |= _BV(DF_CS_PIN)

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

    /* ignore new button presses while old ones are still unprocessed */
    if (btn.done != 0)
        return;

    /* reset button press counters after processing */
    if (btn.timeout > 100) {
        for (uint8_t i = 0; i < 4; i++)
            btn.press[i] = 0;
        btn.timeout = 0;
    }

    /* read button 1 to 4, on pins PC5 to PC2 */
    uint8_t new_sample = PINC;

    /* mask other pins, adjust offset */
    new_sample &= _BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5);
    new_sample >>= PC2;

    /* if something changed */
    if (btn.sample != new_sample) {

        /* iterate through all the buttons */
        for (uint8_t i = 0; i < BUTTONS; i++) {

            /* check if button state is different from last two samples */
            uint8_t s = btn.sample & _BV(i);
            if ( s ^ (btn.state & _BV(i)) ) {
                /* update button state */
                if (s) {
                    btn.state |= _BV(i);
                } else {
                    /* if this is the keypress, increment button press counter */
                    btn.state &= ~_BV(i);
                    btn.press[(BUTTONS-1)-i]++;
                }
            }
        }

        /* reset timeout */
        btn.timeout = 0;
    } else
        btn.timeout++;

    /* remember sample */
    btn.sample = new_sample;

    /* check for timeout */
    if (btn.timeout > 100) {
        btn.done = 1;
    }
}

/* pin change interrupts 1 and 2 do nothing, enabled just for wakeup */
ISR(PCINT1_vect){}
ISR(PCINT2_vect){}

/* usb functions */
uchar   usbFunctionSetup(uchar data[8])
{
    usbRequest_t *req = (void *)data;
    uint8_t len = 0;
    static uint8_t buf[4];

    /* set global data pointer to local buffer */
    usbMsgPtr = buf;

    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    return 0;
}

uchar usbFunctionRead(uchar *data, uchar len)
{
    return 0;
}

/* user interface functions */

/* blink out a sequence (LSB first), every bit is 200ms long */
void noinline blink(uint8_t sequence1, uint8_t sequence2, uint8_t len) {
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

/*
 * main function
 */
int main(void)
{
    /* hardware on port D:
     * PD0: RX
     * PD1: TX
     * PD2: INT0/D+
     * PD3: INT1/IR_IN
     * PD4: D-
     * PD5: IR_OUT
     * PD7: DF_CS
     */
    DDRD = _BV(PD5) | _BV(PD7);
    PORTD = _BV(PD7);

    /* configure pin change interrupt for button 1 and 2 and int0 pin,
     * for waking up from sleep mode... */
    PCMSK1 = _BV(PCINT12) | _BV(PCINT13);
    PCMSK2 = _BV(PCINT18);

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

    /* init spi */
    SPCR = _BV(SPE) | _BV(MSTR);
    SPSR |= _BV(SPI2X);

    /* init timer2 for key debouncing, CTC, prescaler 1024,
     * timeout after 10ms */
    TCCR2A = _BV(WGM21);
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
    TIMSK2 = _BV(OCIE2A);
    OCR2A = F_CPU/100/1024;

    /* init option structure */
    opt.usb = 0;

    /* init button structure */
    btn.sample = 0;

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

#ifdef DEBUG_UART
    UDR0 = 'D';
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = df_status;
    while(!(UCSR0A & _BV(UDRE0)));
#endif

    df_release();

    /* initialize usb communication pins and interrupt */
    usbInit();

    sei();

#ifdef BLINK_START
    /* blink start sequence */
    blink(BLINK_START);
#endif

    if (df_status == DF_STATUS_IDLE)
        blink(BLINK_DF_SEEN);
    else
        blink(BLINK_DF_ERROR);

    while (1)
    {

        if (btn.done) {
            if (btn.press[3] == 1) {
                /* enable usb */

                if (!opt.usb)
                    usbDeviceConnect();
                else
                    usbDeviceDisconnect();

                opt.usb ^= 1;
            }

            btn.done = 0;
        }

        /* process usb requests */
        usbPoll();
    }
}
