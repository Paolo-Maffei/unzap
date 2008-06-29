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
#include "global.h"
#include "debug.h"
#include "pt/pt.h"

#ifndef noinline
#define noinline __attribute__((noinline))
#endif

#define CMD_READ_STATUS 0xD7

#define CMD_PAGE_ERASE 0x81
#define CMD_BLOCK_ERASE 0x50
#define CMD_CHIP_ERASE1 0xC7
#define CMD_CHIP_ERASE2 0x94
#define CMD_CHIP_ERASE3 0x80
#define CMD_CHIP_ERASE4 0x9a

#define CMD_READ_FLASH 0x03

#define CMD_READ_BUF1 0xd1
#define CMD_READ_BUF2 0xd3

#define CMD_WRITE_BUF1 0x84
#define CMD_WRITE_BUF2 0x87

#define CMD_LOAD_BUF1 0x53
#define CMD_LOAD_BUF2 0x55

#define CMD_SAVE_BUF1 0x83
#define CMD_SAVE_BUF2 0x86

#define cs_low()    DF_PORT &= ~_BV(DF_CS_PIN)
#define cs_high()   DF_PORT |= _BV(DF_CS_PIN)

typedef struct
{
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
} df_chipstatus_t;

typedef enum
{
    DF_IDLE = 0,        /* dataflash is idle */
    DF_CHIPERASE = 1,   /* a chiperase is processing */
    DF_WAIT = 2,        /* some operation on the dataflash is running, wait for
                           the dataflash to become ready */
} df_action_t;

/* dataflash thread */
static df_action_t df_action;
static uint16_t df_page;
static struct pt df_thread;

/* module local lowlevel functions */
static noinline uint8_t spi_send(uint8_t data)
{
     SPDR = data;
     while(!(SPSR & _BV(SPIF)));
     return SPDR;
}

static noinline uint8_t df_status(void)
{
    uint8_t status;

    cs_low();
    spi_send(CMD_READ_STATUS);
    status = spi_send(0);
    cs_high();

    return status;
}

static noinline bool df_ready(void)
{
    df_chipstatus_t status;

    cs_low();
    spi_send(CMD_READ_STATUS);
    status.raw = spi_send(0);
    cs_high();

    return status.ready;
}


/* api functions */

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

    /* initialize dataflash thread */
    PT_INIT(&df_thread);

    /* try to read chipid */
    df_chipstatus_t status;
    status.raw = df_status();

    if (status.id == DF_CHIPID && status.ready) {
        /* dataflash has been found and is ready */
        global.dataflash = 1;
    } else
        /* no dataflash found or not ready */
        global.dataflash = 0;
}

uint8_t df_chipid(void)
{
    df_chipstatus_t status;
    status.raw = df_status();
    return status.id;
}

df_status_t df_chip_erase(void)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

#ifdef DF_USE_REAL_CHIPERASE
    cs_low();
    spi_send(CMD_CHIP_ERASE1);
    spi_send(CMD_CHIP_ERASE2);
    spi_send(CMD_CHIP_ERASE3);
    spi_send(CMD_CHIP_ERASE4);
    cs_high();

    df_action = DF_CHIPERASE;
#else
    df_page = 0;
    df_action = DF_CHIPERASE;
#endif

    return DF_PROCESSING;
}

df_status_t df_page_erase(uint16_t page)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

    cs_low();
    spi_send(CMD_PAGE_ERASE);
    page <<= 2;
    spi_send(HI8(page));
    spi_send(LO8(page));
    spi_send(0);
    cs_high();

    df_action = DF_WAIT;

    return DF_PROCESSING;
}

df_status_t df_read(uint16_t page, uint16_t offset, void *data, uint16_t length)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

    if (length == 0)
        return DF_OK;

    if (page >= DF_PAGES || (offset+length) > DF_PAGESIZE)
        return DF_ERROR;

    cs_low();

    /* send opcode */
    spi_send(CMD_READ_FLASH);

    page <<= 2;
    spi_send(HI8(page));
    spi_send(LO8(page) | HI8(offset));
    spi_send(LO8(offset));

    uint8_t *ptr = data;

    do {
        *ptr++ = spi_send(0);
    } while (--length > 0);

    cs_high();

    return DF_OK;
}

df_status_t df_read_buf(df_buf_t buf, uint16_t offset, void *data, uint16_t length)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

    if (length == 0)
        return DF_OK;

    if ((offset+length) > DF_PAGESIZE)
        return DF_ERROR;

    cs_low();

    /* send opcode */
    if (buf == DF_BUF1)
        spi_send(CMD_READ_BUF1);
    else
        spi_send(CMD_READ_BUF2);

    spi_send(0);
    spi_send(HI8(offset));
    spi_send(LO8(offset));

    uint8_t *ptr = data;

    do {
        *ptr++ = spi_send(0);
    } while (--length > 0);

    cs_high();

    return DF_OK;
}

bool df_busy(void)
{

    return df_action != DF_IDLE;
}

df_status_t df_load_page(uint16_t page, df_buf_t buf)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

    if (page >= DF_PAGES)
        return DF_ERROR;

    cs_low();
    if (buf == DF_BUF1)
        spi_send(CMD_LOAD_BUF1);
    else
        spi_send(CMD_LOAD_BUF2);

    page <<= 2;
    spi_send(HI8(page));
    spi_send(LO8(page));
    spi_send(0);

    cs_high();

    df_action = DF_WAIT;

    return DF_PROCESSING;
}

df_status_t df_write_buf(df_buf_t buf, uint16_t offset, void *data, uint16_t length)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

    cs_low();

    if (buf == DF_BUF1)
        spi_send(CMD_WRITE_BUF1);
    else
        spi_send(CMD_WRITE_BUF2);

    spi_send(0);
    spi_send(HI8(offset));
    spi_send(LO8(offset));

    uint8_t *ptr = data;
    do {
        spi_send(*ptr++);
    } while (--length > 0);

    cs_high();

    return DF_OK;
}

df_status_t df_save_buf(df_buf_t buf, uint16_t page)
{
    if (df_chipid() != DF_CHIPID)
        return DF_NOT_FOUND;

    if (df_action)
        return DF_BUSY;

    if (page >= DF_PAGES)
        return DF_ERROR;

    cs_low();
    if (buf == DF_BUF1)
        spi_send(CMD_SAVE_BUF1);
    else
        spi_send(CMD_SAVE_BUF2);

    page <<= 2;
    spi_send(HI8(page));
    spi_send(LO8(page));
    spi_send(0);

    cs_high();

    df_action = DF_WAIT;

    return DF_PROCESSING;
}

static PT_THREAD(df_work(struct pt *thread))
{
    PT_BEGIN(thread);

    while(1) {
        if (df_action == DF_CHIPERASE) {
#ifdef DF_USE_REAL_CHIPERASE
            PT_WAIT_UNTIL(thread, df_ready());
            df_action = DF_IDLE;
#else
            do {
                cs_low();

                /* send opcode */
                spi_send(CMD_BLOCK_ERASE);

                /* send 3 address bytes: 2 don't care bits, 9 bits block address,
                 * 13 don't care bits */
                spi_send(HI8(df_page << 5));
                spi_send(LO8(df_page << 5));
                spi_send(0);

                cs_high();

                PT_WAIT_UNTIL(thread, df_ready());

                df_page++;
            } while (df_page < DF_PAGES/8);

            df_action = DF_IDLE;
#endif
        } else if (df_action == DF_WAIT) {
            debug_putc('B');
            PT_WAIT_UNTIL(thread, df_ready());
            debug_putc('R');

            df_action = DF_IDLE;
        }


        PT_YIELD(thread);
    }

    PT_END(thread);
}

void df_poll(void)
{
    if (df_action)
        PT_SCHEDULE(df_work(&df_thread));
}
