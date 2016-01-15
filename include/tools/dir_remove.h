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

#ifndef __TOOLS_DIR_REMOVE_H
#define __TOOLS_DIR_REMOVE_H

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief recursive remove directory or file
 * @param [in] path name of directory or file
 * @return on success, zero is returned
 *
 * Function updates #errno if error occurred:
 * @li ENOMEM no memory format path
 *
 * If function fails to delete directory (file) it will continue
 * to delete rest of the directories (files), but don't return zero.
 */
int dir_remove(const char *path);

/** @} */

#endif /* __TOOLS_DIR_REMOVE_H */
