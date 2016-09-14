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

#ifndef __LIBTOOLS_FLOCK_H
#define __LIBTOOLS_FLOCK_H

#include <stdio.h>

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief open and lock file
 * @param [in] path name of the file to be created or opened
 * @param [in] mode permissive mode
 * @param [out] fp file descriptor
 * @return pointer to descriptor
 * @retval NULL error occurred, @p fp also should be NULL
 *
 * If error occurred function update #errno - see errors specified for
 * the routines fopen(3), fileno(3), flock(2), and fclose(3).
 */
FILE *flopen(const char *path, const char *mode, FILE **fp);

/**
 * @brief open and try lock file
 * @param [in] path name of the file to be created or opened
 * @param [in] mode permissive mode
 * @param [out] fp file descriptor
 * @return pointer to descriptor
 * @retval NULL error occurred, @p fp also should be NULL
 *
 * If error occurred function update #errno - see errors specified for
 * the routines fopen(3), fileno(3), flock(2), and fclose(3).
 */
FILE *fltryopen(const char *path, const char *mode, FILE **fp);

/**
 * @brief close file openned by flopen() or fltryopen()
 * @param [in] fp file descriptor (can be NULL)
 * @return on success, zero is returned
 * @retval -1 error occurred
 *
 * If error occurred function update #errno - see errors specified for
 * the routines fileno(3), flock(2), and fclose(3).
 */
int flclose(FILE *fp);

/** @} */

#endif /* __LIBTOOLS_FLOCK_H */
