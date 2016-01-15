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

#ifndef __TOOLS_ENV_H
#define __TOOLS_ENV_H

#include <stddef.h>

/**
 * @addtogroup tools
 *
 * @{
 */

/**
 * @brief read environment variable
 * @param [in] env variable name
 * @param [in] s pointer to string buffer
 * @param [in] len length of buffer at @p s
 * @return pointer to string buffer @p s
 * @retval NULL environment variable don't exist
 */
char *read_env(const char *env, char *s, size_t len);

/** @} */

#endif /* __TOOLS_ENV_H */
