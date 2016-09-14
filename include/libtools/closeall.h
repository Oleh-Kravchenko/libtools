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

#ifndef __LIBTOOLS_CLOSEALL_H
#define __LIBTOOLS_CLOSEALL_H

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief close all file descriptors of current process
 * @param [in] nostdclose don't close stdio file descriptors
 * @return zero is returned
 * @retval -1 error occurred
 *
 * This function can be useful for implementation system() like functions.
 */
int closeall(int nostdclose);

/** @} */

#endif /* __LIBTOOLS_CLOSEALL_H */
