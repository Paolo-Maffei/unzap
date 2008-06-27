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

#include <avr/io.h>
#include "config.h"
#include "df.h"

#include <util/delay.h>

#ifndef noinline
#define noinline __attribute__((noinline))
#endif

#define CMD_READ_STATUS 0xD7

#define cs_low()    DF_PORT &= ~_BV(DF_CS_PIN)
#define cs_high()   DF_PORT |= _BV(DF_CS_PIN)

void df_init(void)
{
    /* configure cs pin as output, set to high */
    DF_DDR |= _BV(DF_CS_PIN);
    DF_PORT |= _BV(DF_CS_PIN);

    /* configure spi pins MOSI and SCK as output */
    SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK);
    SPI_DDR &= ~_BV(SPI_MISO);

    /* initialize spi as master, using F_CPU/2 as clock */
    SPCR = _BV(SPE) | _BV(MSTR);
    SPSR |= _BV(SPI2X);
}

static noinline uint8_t spi_send(uint8_t data)
{
     SPDR = data;
     while(!(SPSR & _BV(SPIF)));
     return SPDR;
}

uint8_t df_status(void)
{
    uint8_t status;

    cs_low();
    spi_send(CMD_READ_STATUS);
    status = spi_send(0);
    cs_high();

    return status;
}
