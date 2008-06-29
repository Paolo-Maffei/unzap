/*
 * unzap firmware
 *
 * this file has been copied from
 * http://www.roboternetz.de/wissen/index.php/Speicherverbrauch_bestimmen_mit_avr-gcc
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

#include <avr/io.h>  // RAMEND
#include "mem-check.h"

#include "global.h"
#include "debug.h"

// Mask to init SRAM and check against
#define MASK 0xaa

// From linker script
extern unsigned char __heap_start;

unsigned short
get_mem_unused (void)
{
    unsigned short unused = 0;
    unsigned char *p = &__heap_start;

    do
    {
        if (*p++ != MASK)
            break;

        unused++;
    } while (p <= (unsigned char*) RAMEND);

    return unused;
}

/* !!! never call this function !!! */
void __attribute__ ((naked, section(".init3"))) init_mem (void);
void init_mem (void)
{
   __asm volatile (
      "ldi r30, lo8 (__heap_start)"  "\n\t"
      "ldi r31, hi8 (__heap_start)"  "\n\t"
      "ldi r24, %0"                  "\n\t"
      "ldi r25, hi8 (%1)"            "\n"
      "0:"                           "\n\t"
      "st  Z+,  r24"                 "\n\t"
      "cpi r30, lo8 (%1)"            "\n\t"
      "cpc r31, r25"                 "\n\t"
      "brlo 0b"
         :
         : "i" (MASK), "i" (RAMEND+1)
   );
}

