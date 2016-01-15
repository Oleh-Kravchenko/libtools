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

#ifndef __TOOLS_MKPASSWD_H
#define __TOOLS_MKPASSWD_H

#include <stddef.h>

/**
 * @addtogroup strings
 *
 * @{
 */

/**
 * @brief make strong password
 * @param [in,out] s NULL-terminated string with password
 * @param [in] len maximum length of @p s
 * @return pointer to @p s or NULL, if failed
 *
 * This function use random(), so before using you should initialize
 * random seed by some value, for example by this call srandom(time(NULL))
 */
char *mkpasswd(char *s, size_t len);

/** @} */

#endif /* __TOOLS_MKPASSWD_H */
