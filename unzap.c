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

/* wait 200ms at prescaler 256 (using timer1) */
#define DELAY_NEXT_CODE (F_CPU/256/5)

/*
 * structures and variables
 */

/* these are the global system states */
typedef enum {
    IDLE = 0,       /* the system is idle, nothing is happening,
                     * this might include power-down mode */
    WAIT_BIT,       /* wait for on/off sequence to finish */
    WAIT_RETRANSMIT,/* wait until retransmit should occur */
    WAIT_CODE,      /* wait before transmitting the next code */
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
void pwm_set(uint8_t freq);
send_return_t send_space(void);

/*
 * global variables
 */
volatile state_t state = IDLE;

/* current code */
code_t *code;
volatile send_function_t func = NULL;
volatile uint16_t params[PARAMS];
volatile uint8_t bit;           /* current bit, reset by input capture interrupt of
                                   timer 1, incremented by compare match interrupt A of
                                   timer 1 */
volatile uint8_t retransmit;    /* current retransmit, reset by main function,
                                   incremented by compare match A interrupt of
                                   timer 1 */

/*
 * pwm functions
 */
/* setup timer0: fast pwm mode, clear OC0B on match, prescaler 8 */
#define pwm_enable() { \
    TCCR0A = _BV(WGM00) | _BV(COM0B1); \
    TCCR0B = _BV(CS01) | _BV(WGM02);   \
}
#define pwm_disable() { \
    TCCR0A = 0;         \
    TCCR0B = 0;         \
    TCNT0 = 0;          \
}
#define pwm_set(freq) { \
    OCR0A = freq;       \
    OCR0B = freq/4;     \
}

/* sending functions */
send_return_t send_space(void)
{
#if 0
    /* set on timing */
    OCR1B = params[0];

    /* params[5] holds the bits to be sent */
    if (params[5] & _BV(bit))
        /* set off timing for one */
        OCR1A = params[0]+params[1];
    else
        /* set off timing for zero */
        OCR1A = params[0]+params[2];
#endif

    OCR1B = 260;
    OCR1A = 260+780;

    return SEND_BIT;

#if 0
    if (bit < params[4])
        return SEND_BIT;
    else {
        if (retransmit == 2)
            return SEND_DONE;
        else {
            ICR1 = 31250;
            return SEND_RETRANSMIT;
        }
    }
#endif
}

/* code table */
code_t PROGMEM codes[] = \
{
    /* code 0093 */
    /* freq    function     on  one  zero repeat bits    data data */
    {    33, send_space, { 260, 780, 1840,    70,  15, 0xAAAA,   0 } },
    // { send_space, { 260, 780, 1840,    70,  15, 0x41a2,   0 } },
};

/* the input capture interrupt is used for waiting in between the different codes */
ISR(TIMER1_CAPT_vect)
{

    /* timer1 is already resetted (-> CTC mode), change the prescaler to 64,
     * for bit timing, in CTC mode (TOP == OCR1A) */
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

    /* reset bit counter, call generating function, return value does not
     * matter (because this should initiate the transmit of the first on/off sequence).
     *
     * this function must set up OCR1B and OCR1A according to the desired timing. */
    bit = 0;
    func();

    /* set new system mode */
    state = WAIT_BIT;

    /* clear old interrupts and enable compare interrupts */
    TIFR1 = _BV(OCF1A) | _BV(OCF1B) | _BV(ICF1);
    TIMSK1 = _BV(OCIE1A) | _BV(OCIE1B);

    /* turn on pwm */
    pwm_enable();
}

/* this compare match interrupt is used to call the generating function for
 * next bit timing */
ISR(TIMER1_COMPA_vect)
{
#if 0
    /* increment bit counter for generating function */
    bit++;

    /* call generating function again */
    send_return_t ret = func();

    if (ret == SEND_BIT) {
#endif
        /* turn on pwm, everything is set up, so that this interrupt is
         * called after the bit is transmitted */
        pwm_enable();
        // PORTC &= ~_BV(PC5);
#if 0
    } else if (ret == SEND_RETRANSMIT) {
        /* wait between code and retransmit, input compare match has been
         * set up correctly by generating function, turn off output compare
         * match inturrupts and enable input compare match interrupt */
        TIFR1 = _BV(ICF1);
        TIMSK1 = _BV(ICIE1);
        state = WAIT_RETRANSMIT;
    } else if (ret == SEND_DONE) {
        /* disable all interrupts and set state */
        TIMSK1 = 0;
        state = IDLE;
    }
#endif

}

/* this compare match interrupt is used to turn off pwm */
ISR(TIMER1_COMPB_vect)
{
    pwm_disable();
    // PORTC |= _BV(PC5);
}

int main(void) {
    /* init output pin */
    DDRD = _BV(PD5);

    /* init button */
    PORTD = _BV(PD2);

    /* init and turn off led */
    DDRC = _BV(PC5);
    PORTC = _BV(PC5);

    /* enable timer1 input capture interrupt, which is used for delay between codes,
     * and load delay (for prescaler 256) */
    TIMSK1 = _BV(ICIE1);
    ICR1 = DELAY_NEXT_CODE;

    sei();

#if 0
    /* wait 2s, doing nothing */
    for (uint8_t i = 0; i < 200; i++)
        _delay_loop_2(50000);
#endif

    // XXX FIXME
    pwm_set(33);

    while(1) {

        if (state == IDLE) {
            /* initialize global variales with first code */
            code = &codes[0];
            state = WAIT_CODE;

            /* load next function address from *code */
            func = (send_function_t)pgm_read_word(&code->func);

            /* load parameters */
            uint16_t *ptr = code->params;
            for (uint8_t i = 0; i < PARAMS; i++)
                params[i] = pgm_read_word(ptr++);

            /* reset retransmit counter */
            retransmit = 0;

            /* start timer1 with prescaler 256, in CTC mode (TOP = ICR1) */
            TCCR1B = _BV(CS12) | _BV(WGM13) | _BV(WGM12);
        }
#if 0
        code_t *code = &codes[0];

        while(1) {
            send_function_t send_func;
            send_func = (send_function_t)pgm_read_word(&code->func);

            if (send_func == NULL)
                break;

            // pwm_set(pgm_read_byte(&codes[p].freq));
            pwm_set(33);
            uint16_t *ptr = code->params;
            for (uint8_t i = 0; i < PARAMS; i++)
                send_params[i] = pgm_read_word(ptr++);

            send_func(0);

            code++;
        }

        while(1);
#endif
    }
}
