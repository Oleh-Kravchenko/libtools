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

#ifndef __TOOLS_EXECUTEFILE_H
#define __TOOLS_EXECUTEFILE_H

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief execute file as program
 * @param [in] path path to executable file
 * @return status code of child process
 * @retval -1 error occurred
 */
int executefile(const char *path);

/** @} */

#endif /* __TOOLS_EXECUTEFILE_H */
