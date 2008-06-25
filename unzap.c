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
#include <util/delay.h>

#include "config.h"
#include "global.h"
#include "debug.h"
#include "df.h"
#include "usb.h"
#include "ui.h"
#include "timer.h"

#ifndef noinline
#define noinline __attribute__((noinline))
#endif

/*
 * prototypes
 */

/*
 * helper macros
 */


/* user interface functions */

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
    //DDRD = _BV(PD5) | _BV(PD7);
    //PORTD = _BV(PD7);

    /* hardware on port B:
     * PB0: PUD
     * PB1: /LED2
     * PB2: /LED1
     * PB3: MOSI
     * PB4: MISO
     * PB5: SCK
     */
    //DDRB = _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB5);
    //PORTB = _BV(PB1) | _BV(PB2);

    /* hardware on port C:
     * PC0: IR_IN2
     * PC1: POWER
     * PC2: BTN4
     * PC3: BTN3
     * PC4: BTN2
     * PC5: BTN1
     */
    //DDRC = 0;
    //PORTC = _BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5);

    /* init spi */
    //SPCR = _BV(SPE) | _BV(MSTR);
    //SPSR |= _BV(SPI2X);

    /* init debug uart */
    debug_init();

    /* init timer library */
    timer_init();

    /* init user interface */
    ui_init();

    //debug_putc('D');
    //debug_putc(df_status());

    /* initialize usb stack */
    usb_init();
    //usb_enable();

    /* enable interrupts */
    sei();

    timer_t t;

    while (1)
    {
        usb_poll();
        ui_poll();
    }
}
