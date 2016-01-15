/**
 * @file
 *
 * Copyright (C) 2016  Oleh Kravchenko <oleg@kaa.org.ua>
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

#ifndef __TOOLS_RECVALL_H
#define __TOOLS_RECVALL_H

#include <stddef.h>

/**
 * @addtogroup network
 *
 * @{
 */

/**
 * @brief read buffer from socket descriptor
 * @param [in] sockfd socket descriptor
 * @param [in] buf buffer for read data
 * @param [in] len length of @p buf in bytes
 * @param [in] flags please read "man recv" for flags description
 * @return amount of received bytes, should be equal @p len
 * @retval 0 connection closed
 * @retval -1 error occurred
 */
ssize_t recvall(int sockfd, void *buf, size_t len, int flags);

/** @} */

#endif /* __TOOLS_RECVALL_H */
