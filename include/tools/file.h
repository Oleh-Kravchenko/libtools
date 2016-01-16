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

#ifndef __TOOLS_FILE_H
#define __TOOLS_FILE_H

#include <stddef.h>

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief save data to file
 * @param [in] path file name
 * @param [in] buf pointer to data buffer
 * @param [in] len length of data at buffer @p buf
 * @return on success, zero is returned
 * @retval -1 error occurred
 */
int put_file_contents(const char *path, const void *buf, size_t len);

/**
 * @brief allocate memory and read data from file
 * @param [in] path file name
 * @param [out] buf pointer to file data
 * @param [out] len pointer to file length
 * @return on success pointer to allocated memory
 * @retval NULL error occurred
 */
void *get_file_contents(const char *path, void **buf, size_t *len);

/** @} */

#endif /* __TOOLS_FILE_H */
