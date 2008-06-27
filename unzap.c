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
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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

/* global variables */
global_t global;

/* main function */
int main(void)
{
    debug_init();
    timer_init();
    ui_init();
    usb_init();

    /* enable interrupts */
    sei();

    /* blink start sequence */
    ui_blink(BLINK_START);

    while (1)
    {
        usb_poll();
        ui_poll();
    }
}
