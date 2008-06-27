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

#ifndef __GLOBAL_H
#define __GLOBAL_H


/* helper macros */
#ifndef HI8
#define HI8(x)  ((uint8_t)((x) >> 8))
#endif

#ifndef LO8
#define LO8(x)  ((uint8_t)(x))
#endif

/* global variables */
typedef struct {
    union {
        struct {
            uint8_t stealth:1;
            uint8_t single_step:1;
            uint8_t ignore_voltage:1;
        };
        uint8_t raw;
    };
} options_t;

typedef struct {
    options_t opts;
} global_t;

extern global_t global;

#endif
