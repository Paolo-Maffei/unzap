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

#ifndef _DF_H
#define _DF_H

#include <stdint.h>

typedef struct {
    union {
        struct {
            uint8_t pagesize:1;
            uint8_t protect:1;
            uint8_t id:4;
            uint8_t compare:1;
            uint8_t ready:1;
        };
        uint8_t raw;
    };
} df_status_t;

void df_init(void);
uint8_t df_status(void);

#endif
