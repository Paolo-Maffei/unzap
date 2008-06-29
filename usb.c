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

#include "usb.h"
#include "config.h"
#include "global.h"
#include "debug.h"
#include "usbdrv/usbdrv.h"
#include "df.h"
#include "ui.h"
#include "mem-check.h"

/* supply custom usbDeviceConnect() and usbDeviceDisconnect() macros
 * which turn the interrupt on and off at the right times,
 * and prevent the execution of an interrupt while the pullup resistor
 * is switched off */
#ifdef USB_CFG_PULLUP_IOPORTNAME
#undef usbDeviceConnect
#define usbDeviceConnect()      do { \
                                    USB_PULLUP_DDR |= (1<<USB_CFG_PULLUP_BIT); \
                                    USB_PULLUP_OUT |= (1<<USB_CFG_PULLUP_BIT); \
                                    USB_INTR_ENABLE |= (1 << USB_INTR_ENABLE_BIT); \
                                   } while(0);
#undef usbDeviceDisconnect
#define usbDeviceDisconnect()   do { \
                                    USB_INTR_ENABLE &= ~(1 << USB_INTR_ENABLE_BIT); \
                                    USB_PULLUP_DDR &= ~(1<<USB_CFG_PULLUP_BIT); \
                                    USB_PULLUP_OUT &= ~(1<<USB_CFG_PULLUP_BIT); \
                                   } while(0);
#endif

/* module-local variables */

typedef enum {
    USB_IDLE = 0,
    USB_DF_PAGE = 1,
} usb_action_t;

static bool usb_status = 0;
static usb_action_t usb_action;
static uint16_t usb_df_page;
static uint16_t usb_offset;
static uint16_t usb_bytes_remaining;

/* usb functions */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *req = (void *)data;
    usbMsgLen_t len = 0;
    static uint8_t buf[8];

    /* set global data pointer to local buffer */
    usbMsgPtr = buf;

    if (req->bRequest == USBRQ_ECHO) {
        buf[0] = req->wValue.bytes[0];
        buf[1] = req->wValue.bytes[1];
        len = 2;
    } else if (req->bRequest == USBRQ_GET_DATAFLASH_INFO) {
        buf[0] = LO8(DF_PAGES);
        buf[1] = HI8(DF_PAGES);
        buf[2] = LO8(DF_PAGESIZE);
        buf[3] = HI8(DF_PAGESIZE);
        buf[4] = df_chipid();
        len = 5;
    } else if (req->bRequest == USBRQ_GET_DATAFLASH_STATUS) {
        buf[0] = df_busy();
        len = 1;
    } else if (req->bRequest == USBRQ_READ_WRITE_DATAFLASH) {
        /* wait until the dataflash is ready */
        while(df_busy());

        usb_df_page = req->wValue.word;
        usb_offset = req->wIndex.word;
        usb_bytes_remaining = req->wLength.word;
        usb_action = USB_DF_PAGE;

        /* if this is an out request, load old page to buffer 1 */
        if (!(req->bmRequestType & _BV(7)))
            df_load_page(usb_df_page, DF_BUF1);

        return USB_NO_MSG;
    } else if (req->bRequest == USBRQ_DATAFLASH_ERASE) {
        buf[0] = df_chip_erase();
        len = 1;
    } else if (req->bRequest == USBRQ_DATAFLASH_ERASE_PAGE) {
        buf[0] = df_page_erase(req->wValue.word);
        len = 1;
#ifdef DEBUG
    } else if (req->bRequest == USBRQ_DEBUG_GETMEM) {
        uint16_t mem = get_mem_unused();
        buf[0] = HI8(mem);
        buf[1] = LO8(mem);
        len = 2;
#endif
    } else {
        debug_putc('S');
        for (uint8_t i = 0; i < 8; i++)
            debug_putc(data[i]);
    }

    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    if (len > usb_bytes_remaining)
        len = usb_bytes_remaining;

    usb_bytes_remaining -= len;

    if (usb_action == USB_DF_PAGE) {
        /* wait until the dataflash is ready again */
        while(df_busy());

        df_write_buf(DF_BUF1, usb_offset, data, len);

        usb_offset += len;

        if (usb_bytes_remaining == 0) {
            len = 1;
            /* write this page */
            df_save_buf(DF_BUF1, usb_df_page);
        } else
            len = 0;
    } else
        len = 0;

    if (len == 1)
        usb_action = USB_IDLE;

    return len;
}

uchar usbFunctionRead(uchar *data, uchar len)
{
    if (usb_action == USB_DF_PAGE) {
        /* wait until the dataflash is ready again */
        while(df_busy());

        if ((usb_offset+len) > DF_PAGESIZE)
            len = LO8(DF_PAGESIZE-usb_offset);

        if (df_read(usb_df_page, usb_offset, data, len) != DF_OK)
            len = 0;

        usb_bytes_remaining -= len;
        usb_offset += len;
    } else
        len = 0;

    if (len == 0)
        usb_action = USB_IDLE;

    return len;
}

/* api functions */
void usb_init(void)
{
    /* initialize usb communication pins and interrupt */
    usbInit();
}

void usb_enable(void)
{
    usbDeviceConnect();
    usb_status = true;
}

void usb_disable(void)
{
    usbDeviceDisconnect();
    usb_status = false;
}

bool usb_enabled(void)
{
    return usb_status;
}

void usb_poll(void)
{
    if (usb_status)
        usbPoll();
}
