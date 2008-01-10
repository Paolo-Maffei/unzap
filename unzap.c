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

/* wait 250ms at prescaler 256 */
#define DELAY_NEXT_CODE (F_CPU/256/4)

/*
 * structures and variables
 */

/* these are the global system states */
typedef enum {
    IDLE = 0,       /* the system is idle, nothing is happening,
                     * this might include power-down mode */
    WAIT_ON,        /* wait for on sequence to finish */
    WAIT_OFF,       /* wait for off sequence to finish */
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
typedef send_return_t (*send_function_t)(uint8_t);

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
send_return_t send_space(uint8_t);

/*
 * global variables
 */
state_t state = IDLE;
uint16_t send_params[PARAMS];

/*
 * pwm functions
 */
/* phase correct pwm mode, clear OC0B on match */
#define pwm_init() { TCCR0A = _BV(WGM00) | _BV(COM0B1); }
#define pwm_disable() { TCCR0B = 0; }
void pwm_set(uint8_t freq) {
    OCR0A = freq;
    OCR0B = freq/4;
}
/* prescaler 8, phase correct pwm mode */
#define pwm_enable() { TCCR0B = _BV(CS01) | _BV(WGM02); }

/* sending functions */
send_return_t send_space(uint8_t retransmit)
{
    return SEND_DONE;
}

/* code table */
code_t PROGMEM codes[] = {
    /* code 0093 */
    /* freq    function     on  one  zero repeat bits    data data */
    {    33, send_space, { 260, 780, 1840,    70,  15, 0xAAAA,   0 } },
    // { send_space, { 260, 780, 1840,    70,  15, 0x41a2,   0 } },
};

/* compare match a interrupt is used for timing in-between different codes */
ISR(TIMER1_COMPA_vect) {

}

ISR(TIMER1_COMPB_vect) {
}

int main(void) {
    /* init output pin */
    DDRD = _BV(PD5);

    /* init button */
    PORTD = _BV(PD2);

    /* init led */
    DDRC = _BV(PC5);
    PORTC = _BV(PC5);

    pwm_init();

    sei();

    while(1) {

        for (uint8_t i = 0; i < 200; i++)
            _delay_loop_2(50000);

        /* turn off led */
        PORTC |= _BV(PC5);


        uint8_t p = 0;
        while(1) {
            send_function_t send_func;
            send_func = (send_function_t)pgm_read_word(&codes[p].func);

            if (send_func == NULL)
                break;

            // pwm_set(pgm_read_byte(&codes[p].freq));
            pwm_set(33);
            uint16_t *ptr = codes[p].params;
            for (uint8_t i = 0; i < PARAMS; i++)
                send_params[i] = pgm_read_word(ptr++);

            send_func(0);

            p++;
        }

        while(1);
    }
}
