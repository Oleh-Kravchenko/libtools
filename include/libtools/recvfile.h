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

#ifndef __LIBTOOLS_RECVFILE_H
#define __LIBTOOLS_RECVFILE_H

/**
 * @addtogroup network
 *
 * @{
 */

/**
 * @brief receive file from socket
 * @param [in] sockfd socket descriptor
 * @param [in] path filename for receive file
 * @param [in] size size of file to receive
 * @return on success, zero is returned
 *
 * If error occurred, function can create file with name @p path and
 * it will be incomplete.
 */
int recvfile(int sockfd, const char *path, size_t size);

/** @} */

#endif /* __LIBTOOLS_RECVFILE_H */
