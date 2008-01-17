#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*
 * constants
 */
// XXX FIXME
#define PARAMS 7
#define VARS 10

/* wait 200ms at prescaler 256 (using timer1) */
#define DELAY_NEXT_CODE (F_CPU/256/5)

/*
 * structures and variables
 */

/* these are the global system states */
typedef enum {
    IDLE = 0,       /* the system is idle, nothing is happening,
                     * this might include power-down mode */
    START,          /* start transmitting now */
    WAIT_BIT,       /* wait for on/off sequence to finish */
    WAIT_RETRANSMIT,/* wait until retransmit should occur */
    WAIT_CODE,      /* wait before transmitting the next code */
    CODE_DONE,      /* code transmission has been completed (including retransmits) */
} state_t;

/* each code generating function returns these values */
typedef enum {
    SEND_DONE = 0,      /* one complete code sequence is done */
    SEND_RETRANSMIT,    /* one datagram has been transmitted, schedule retransmit */
    SEND_BIT,           /* values for on- and off-cycle have been loaded */
} send_return_t;

/* the datatype for generating functions */
typedef send_return_t (*send_function_t)(void);

/* structure for storing the different codes */
typedef struct {
    uint8_t freq;               /* the TOP value for the pwm timer0 */
    send_function_t func;       /* the function which generates the timing sequence */
    uint16_t params[PARAMS];    /* parameters for the generating function */
} code_t;

/*
 * prototypes
 */

/*
 * global variables
 */
volatile state_t state = IDLE;
volatile uint8_t tccr0b;

/* current code */
code_t *code;
volatile send_function_t func = NULL;
volatile uint16_t params[PARAMS];
volatile uint8_t vars[VARS];
/* current bit, reset by input capture interrupt of timer 1, incremented by
 * generating function */
volatile uint8_t bit;
/* current retransmit counter, reset by main function, incremented by compare
 * match A interrupt of timer 1 */
volatile uint8_t retransmit;
/* remaining retransmit timeout, is set to retransmit timeout by generating
 * function, decremented by compare match A interrupt of timer 1 */
volatile uint16_t retransmit_timeout;

/*
 * pwm functions
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
    OCR0B = freq/4;     \
}

/* sending functions */
send_return_t send_space(void);
send_return_t send_nec(void);

#if 0
send_return_t send_space(void)
{

    /* set on timing */
    OCR1B = params[0];

    /* params[5] holds the bits to be sent */
    if (params[5] & _BV(bit))
        /* set off timing for one */
        OCR1A = params[0]+params[1];
    else
        /* set off timing for zero */
        OCR1A = params[0]+params[2];

    /* set retransmit timeout, if this is the first bit */
    if (bit == 0)
        retransmit_timeout = params[3] - OCR1A;

    if (bit < params[4])
        return SEND_BIT;
    else {
        if (retransmit == params[6])
            return SEND_DONE;
        else {
            return SEND_RETRANSMIT;
        }
    }
}
#endif

send_return_t send_nec(void)
{

    /* vars[0] is zero, if no header has been sent before */
    if (vars[0] == 0) {
        /* send start header: 9ms pulse, 4.5ms pause */
        OCR1B = 2250;
        OCR1A = 3375;

        /* mark header sent */
        vars[0] = 1;

        /* set retransmit timeout */
        retransmit_timeout = params[3] - 3375;

        return SEND_BIT;
    } else if (retransmit > 0) {
        /* this is a retransmit, just send "button still pressed" code */
        if (bit == 0) {
            /* send header: * 9ms pulse, 2.25ms pause */
            OCR1B = 2250;
            OCR1A = 2812;

            /* set retransmit timeout */
            retransmit_timeout = params[3] - 2812;

        } else if (bit == 1) {
            /* send a zero */
            OCR1B = params[0];
            OCR1A = params[2];
        } else {
            /* do 2 retransmits */
            if (retransmit != 2)
                return SEND_RETRANSMIT;
            else
                return SEND_DONE;
        }


        /* increment bit counter and send */
        bit++;
        return SEND_BIT;
    }

    /* set on timing: 560us */
    OCR1B = params[0];

    if (bit < params[4]) {
        /* params[5] and params[7] contain the data to be sent, LSB first */
        if (params[5 + bit/8] & _BV(bit % 8))
            /* set pause for sending a one (on+off == 2.25ms) */
            OCR1A = params[1];
        else
            /* set pause for sending a zero (on+off == 1.12ms) */
            OCR1A = params[2];

    } else if (bit == params[4]) {
        /* send terminating zero */
        OCR1A = params[2];
    } else
        return SEND_RETRANSMIT;

    bit++;
    return SEND_BIT;
}

/* code table */
code_t PROGMEM codes[] = \
{
    /* code 0093 */
    /* freq    function     on  one  zero repeat bits    data data */
    //{    33, send_space, { 260, 780, 1840, 31250,  15, 0xAAAA,   2 } },

    /* nec code: 00001010 11110101 11101000 00010111 */
    { 210, send_nec, { 140, 562, 280, 27500, 16, 0xaf50, 0xe817 } },
};

/* the input capture interrupt is used for waiting in between the different codes */
ISR(TIMER1_CAPT_vect)
{
    PORTC &= ~_BV(PC5);

    /* all of the following code is common for state WAIT_CODE and WAIT_RETRANSMIT */

    /* turn on pwm */
    pwm_enable();

    /* timer1 is already resetted (-> CTC mode), change the prescaler to 64,
     * for bit timing, in CTC mode (TOP == OCR1A) */
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

    /* set remaining retransmit timeout, call generating function, return value
     * does not matter (because this should initiate the transmit of the first
     * on/off sequence).
     *
     * this function must set up OCR1B and OCR1A according to the desired timing,
     * and retransmit_timeout according to the desired retransmit timeout */
    bit = 0;
    func();

    /* set new system mode */
    state = WAIT_BIT;

    /* clear old interrupts and enable compare interrupts */
    TIFR1 = _BV(OCF1A) | _BV(OCF1B) | _BV(ICF1);
    TIMSK1 = _BV(OCIE1A) | _BV(OCIE1B);
}

/* this compare match interrupt is used to call the generating function for
 * next bit timing */
ISR(TIMER1_COMPA_vect)
{
    /* call generating function again */
    send_return_t ret = func();

    if (ret == SEND_BIT) {
        /* turn on pwm, everything is set up, so that this interrupt is
         * called after the bit is transmitted */
        pwm_enable();
        // PORTC &= ~_BV(PC5);

        /* update retransmit timeout */
        retransmit_timeout -= OCR1A;

    } else if (ret == SEND_RETRANSMIT) {
        /* wait between code and retransmit, set up input compare match,
         * turn off output compare match inturrupts and enable input compare
         * match interrupt */
        ICR1 = retransmit_timeout;

        /* reconfigure timer1 for prescaler 64, in CTC mode (TOP = ICR1) */
        TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM13) | _BV(WGM12);

        TIFR1 = _BV(ICF1);
        TIMSK1 = _BV(ICIE1);
        state = WAIT_RETRANSMIT;

        /* increment retransmit counter */
        retransmit++;

        PORTC |= _BV(PC5);
    } else if (ret == SEND_DONE) {
        /* disable all interrupts and set state */
        TIMSK1 = 0;
        state = CODE_DONE;
        PORTC |= _BV(PC5);
    }
}

/* this compare match interrupt is used to turn off pwm */
ISR(TIMER1_COMPB_vect)
{
    pwm_disable();
    // PORTC |= _BV(PC5);
}

int main(void)
{
    /* init output pin */
    DDRD = _BV(PD5);

    /* init button */
    PORTD = _BV(PD2);

    /* init and turn off led */
    DDRC = _BV(PC5);
    PORTC = _BV(PC5);

    sei();

    // XXX FIXME
    state = START;

    /* remember the current code index and the amount of different codes
     * in the code array */
    uint8_t current_code;
    uint8_t codesize = sizeof(codes)/sizeof(code_t);

    while(1) {

        if (state == START) {
            /* initialize global variales with first code */
            current_code = 0;
            code = &codes[0];
            state = CODE_DONE;
        }

        if (state == CODE_DONE && current_code < codesize) {
            /* load next function address from *code */
            func = (send_function_t)pgm_read_word(&code->func);

            /* load parameters */
            uint16_t *ptr = code->params;
            for (uint8_t i = 0; i < PARAMS; i++)
                params[i] = pgm_read_word(ptr++);
            pwm_set(pgm_read_byte(&code->freq));

            /* reset send function variables */
            for (uint8_t i = 0; i < VARS; i++)
                vars[i] = 0;

            /* reset retransmit counter, set state */
            retransmit = 0;
            state = WAIT_CODE;

            /* set code delay, start timer1 with prescaler 256, in CTC mode (TOP = ICR1) */
            ICR1 = DELAY_NEXT_CODE;
            TIFR1 = _BV(ICF1);
            TIMSK1 = _BV(ICIE1);
            TCCR1B = _BV(CS12) | _BV(WGM13) | _BV(WGM12);

            /* increment code pointer and index */
            current_code++;
            code = &codes[current_code];
        }
    }
}
