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

#ifndef _CONFIG_H
#define _CONFIG_H

#include <avr/version.h>

/* check for avr-libc version */
#if __AVR_LIBC_VERSION__ < 10404UL
#error "newer libc version (>= 1.4.4) needed!"
#endif

/* check if cpu speed is defined */
#ifndef F_CPU
#error "please define F_CPU!"
#endif

#if !defined(HARDWARE_REV) || (HARDWARE_REV != 1 && HARDWARE_REV != 2)
#error "please define HARDWARE_REV to be either 1 or 2"
#endif

/*
 * hardware definition
 */

#define LED_PORT PORTB
#define LED1_PIN PB2
#define LED2_PIN PB1

#define DF_PORT PORTD
#define DF_CS_PIN PD7
#define DF_STATUS_IDLE 0xAC

/* leave 1 second between last button press and process by mainloop */
#define BUTTONS 4
#define BUTTON_TIMEOUT 80

/*
 * timing constants
 */

/* wait 250ms at prescaler 256, MUST be <= 0xfffd! */
#define DELAY_NEXT_CODE (F_CPU/256/4)

/* special protocols */
#define NEC_HEADER_ON  (F_CPU/64 * 9000 / 1000000)
#define NEC_HEADER_OFF (F_CPU/64 * (90+45) / 10000)
#define NEC_ON         (F_CPU/64 *  560 / 1000000)
#define NEC_OFF_ZERO   (F_CPU/64 * 1120 / 1000000)
#define NEC_OFF_ONE    (F_CPU/64 * 2250 / 1000000)
#define NEC_REPEAT_ON  (F_CPU/64 * 9000 / 1000000)
#define NEC_REPEAT_OFF (F_CPU/64 * (9000+2250) / 1000000)

#define RC5_TIME       (F_CPU/64 * 889 / 1000000)

/* wait one second before entering sleep mode (using timer2, which expires every 10ms) */
#define SLEEP_COUNTER_VALUE 100

/* blink sequences */
#define BLINK_START 0x01, 0x00, 2
#define BLINK_MODE1 0x01, 0x01, 3
#define BLINK_MODE1_END 0x02, 0x04, 5
#define BLINK_MODE2 0x05, 0x05, 5
#define BLINK_MODE2_END 0x02, 0x04, 5

#define BLINK_SILENT 0x01, 0x00, 1
#define BLINK_NONSILENT 0x00, 0x01, 1

#define BLINK_STEP 0x01, 0x00, 1
#define BLINK_NOSTEP 0x00, 0x01, 1

#define BLINK_INVALID 0x3f, 0x3f, 6

#define BLINK_DF_SEEN 0x01, 0, 2
#define BLINK_DF_ERROR 0, 0x01, 2

#endif /* _CONFIG_H */
