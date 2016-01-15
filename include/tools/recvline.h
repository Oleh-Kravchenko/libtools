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

#ifndef __TOOLS_RECVLINE_H
#define __TOOLS_RECVLINE_H

#include <stddef.h>

/**
 * @addtogroup network
 *
 * @{
 */

/**
 * @brief receive string from socket
 * @param [in] sockfd socket descriptor
 * @param [in] buf buffer for received string
 * @param [in] size length of buffer @p buf in bytes
 * @param [in] s end of line marker
 * @param [in] len length of @p s marker
 * @return amount of received bytes
 * @retval 0 connection closed
 * @retval -1 error occurred
 *
 * Buffer @p buf should contain string with EOL @p s if not,
 * than buffer @p buf is too small to store whole string.
 */
ssize_t recvline(int sockfd, char *buf, size_t size, const char *s, size_t len);

/** @} */

#endif /* __TOOLS_RECVLINE_H */
