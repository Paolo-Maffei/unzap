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
static uint8_t usb_status;

/* usb functions */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *req = (void *)data;
    usbMsgLen_t len = 0;
    static uint8_t buf[8];

    /* set global data pointer to local buffer */
    usbMsgPtr = buf;

    debug_putc('S');
    for (uint8_t i = 0; i < 8; i++)
        debug_putc(data[i]);

    if (req->bRequest == 0x23) {
        buf[0] = 0x23;
        len = 1;
    } else if (req->bRequest == 0x24) {
        len = req->wLength.word;
        LED1_ON();
    }

    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{


    return 0;
}

uchar usbFunctionRead(uchar *data, uchar len)
{


    return 0;
}

/* api functions */
void usb_init(void)
{
    usb_status = 0;

    /* initialize usb communication pins and interrupt */
    usbInit();
}

void usb_enable(void)
{
    usbDeviceConnect();
    usb_status = 1;
}

void usb_disable(void)
{
    usbDeviceDisconnect();
    usb_status = 0;
}

void usb_poll(void)
{
    if (usb_status)
        usbPoll();
}
