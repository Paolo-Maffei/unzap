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

#endif /* _CONFIG_H */
