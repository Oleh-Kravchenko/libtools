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

#ifndef __TOOLS_TRIM_H
#define __TOOLS_TRIM_H

/**
 * @addtogroup strings
 *
 * @{
 */

/**
 * @brief trim string
 * @param [in,out] s pointer to string
 */
void trim(char *s);

/**
 * @brief trim string from left
 * @param [in,out] s pointer to string
 */
void ltrim(char *s);

/**
 * @brief trim string from right
 * @param [in,out] s pointer to string
 */
void rtrim(char *s);

/** @} */

#endif /* __TOOLS_TRIM_H */
