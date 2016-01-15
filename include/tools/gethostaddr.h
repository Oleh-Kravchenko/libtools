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

#ifndef __TOOLS_GETHOSTADDR_H
#define __TOOLS_GETHOSTADDR_H

#include <netinet/in.h>

/**
 * @addtogroup network
 *
 * @{
 */

/**
 * @brief resolve hostname or ip address into in_addr
 * @param [in] hostname hostname or ip address string
 * @param [out] addr struct in_addr
 * @return pointer to @p addr, if successful
 * @retval NULL error occurred
 */
struct in_addr *gethostaddr(const char *hostname, struct in_addr *addr);

/** @} */

#endif /* __TOOLS_GETHOSTADDR_H */
