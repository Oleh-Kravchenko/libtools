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

#ifndef __LIBTOOLS_MOVEFILE_H
#define __LIBTOOLS_MOVEFILE_H

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief move file to new path
 * @param [in] oldpath filename to move
 * @param [in] newpath new path of file
 * @return zero is returned
 * @retval -1 error occured, check #errno for more info
 *
 * When @p oldpath and @p newpath on the same filesystem - rename() syscall
 * will be used. If rename() syscall failed
 * function will try create new and delete old file.
 */
int movefile(const char *oldpath, const char *newpath);

/** @} */

#endif /* __LIBTOOLS_MOVEFILE_H */
