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
#include <stdbool.h>

typedef enum {
    DF_OK = 0,
    DF_PROCESSING = 1,
    DF_NOT_FOUND = 2,
    DF_BUSY = 3,
    DF_ERROR = 4,
} df_status_t;

typedef enum {
    DF_BUF1 = 0,
    DF_BUF2 = 1,
} df_buf_t;

void df_init(void);
void df_poll(void);

/* api functions */
uint8_t df_chipid(void);
df_status_t df_chip_erase(void);
df_status_t df_page_erase(uint16_t page);
df_status_t df_read(uint16_t page, uint16_t offset, void *data, uint16_t length);
df_status_t df_read_buf(df_buf_t buf, uint16_t offset, void *data, uint16_t length);
df_status_t df_load_page(uint16_t page, df_buf_t buf);
df_status_t df_write_buf(df_buf_t buf, uint16_t offset, void *data, uint16_t length);
df_status_t df_save_buf(df_buf_t buf, uint16_t page);
bool df_busy(void);

#endif
