/**
 * @file
 *
 * Copyright (C) 2014  Oleh Kravchenko <oleg@kaa.org.ua>
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

#ifndef __LIBTOOLS_RECVLINE_H
#define __LIBTOOLS_RECVLINE_H

#include <stddef.h>

/**
 * @addtogroup network
 *
 * @{
 */

/**
 * @brief receive data from socket until delimiter is not reached
 * @param [in] fd socket descriptor
 * @param [out] buf buffer for received data
 * @param [in] size length of buffer @p buf in bytes
 * @param [in] d pointer to delimiter data
 * @param [in] dlen length of delimiter data in bytes
 * @return amount of received bytes
 * @retval 0 connection closed
 * @retval -1 error occurred
 *
 * Buffer @p buf should contain data with terminal delimiter @p d if not,
 * than buffer @p buf is too small to store whole string.
 */
ssize_t recvline(int fd, char *buf, size_t size, const char *d, size_t dlen);

/** @} */

#endif /* __LIBTOOLS_RECVLINE_H */
