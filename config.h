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

#define LED_PORTNAME B
#define LED1_PIN     2
#define LED2_PIN     1

#define BTN_PORTNAME C
#define BTN1_PIN     5
#define BTN2_PIN     4
#define BTN3_PIN     3
#define BTN4_PIN     2

#define DF_PORT PORTD
#define DF_CS_PIN PD7

/* compile for an at45db161d as default */
#define AT45DB161D

#ifdef AT45DB161D
    #define DF_STATUS_IDLE 0xAC

    #define DF_PAGESIZE 512
    #define DF_PAGES 4096
#else
    #error "unknown dataflash!"
#endif


/* macros for concatenating register names (copied from avrusb) */
#define _CONCAT(a, b)            a ## b
#define _CONCAT_EXPANDED(a, b)   _CONCAT(a, b)
#define _OUTPORT(name)           _CONCAT(PORT, name)
#define _INPORT(name)            _CONCAT(PIN, name)
#define _DDRPORT(name)           _CONCAT(DDR, name)

/* define names to use in source */
#define LED_PORT _OUTPORT(LED_PORTNAME)
#define LED_DDR  _DDRPORT(LED_PORTNAME)

#define BTN_PORT _OUTPORT(BTN_PORTNAME)
#define BTN_DDR  _DDRPORT(BTN_PORTNAME)
#define BTN_PIN  _INPORT(BTN_PORTNAME)

#endif /* _CONFIG_H */
