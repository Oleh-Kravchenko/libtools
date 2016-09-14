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

#ifndef __LIBTOOLS_DIRECTORY_H
#define __LIBTOOLS_DIRECTORY_H

#include <sys/types.h>

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief verify or create directory
 * @param [in] path directory name
 * @param [in] mode directory permissions
 * @return zero returned
 * @retval -1 error occurred
 *
 * This function make parent directories, if needed.
 */
int mkdirectory(const char *path, mode_t mode);

/** @} */

#endif /* __LIBTOOLS_DIRECTORY_H */
