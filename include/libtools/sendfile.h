/**
 * @file
 *
 * Copyright (C) 2015  Oleh Kravchenko <oleg@kaa.org.ua>
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

#ifndef __TOOLS_SENDFILE_H
#define __TOOLS_SENDFILE_H

/**
 * @addtogroup network
 *
 * @{
 */

/**
 * @brief Send file to the socket
 * @param [in] sockfd socket descriptor
 * @param [in] path name of file to send
 * @param [in] size size of file to send
 * @return on success, zero is returned
 * @retval -1 error occurred
 * @remarks If file size is less than @p size,
 * function will fail with undefined value in #errno.
 *
 * Function updates #errno, if error occurred:
 * @li #EIO I/O error
 * @li #EPIPE connection lost
 */
int sendfile(int sockfd, const char *path, size_t size);

/** @} */

#endif /* __TOOLS_SENDFILE_H */
