/**
 * @file
 *
 * Copyright (C) 2019  Oleh Kravchenko <oleg@kaa.org.ua>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIBTOOLS_HEX_H
#define __LIBTOOLS_HEX_H

#include <stddef.h>

/**
 * @addtogroup tools
 *
 * @{
 */

/**
 * @brief convert binary data to hex string
 * @param [in] data pointer to binary data
 * @param [in] len length of binary data
 * @param [out] hex pointer to result buffer
 * @param [in] hex_len length of buffet at pointer @p hex
 * @return number of converted bytes
 */
size_t bin2hex(const void *data, size_t len, char *hex, size_t hex_len);

/**
 * @brief convert a hex digit to integer
 * @param [in] ch ascii character
 * @return integer
 */
int hex2int(char ch);

/**
 * @brief convert hex string to binary data
 * @param [in] hex pointer to string with hex numbers
 * @param [out] data pointer to data buffer
 * @param [in] data_len length of data buffer at pointer @p data
 * @return number of converted bytes
 */
size_t hex2bin(const char *hex, void *data, size_t data_len);

/** @} */

#endif /* __LIBTOOLS_HEX_H */
