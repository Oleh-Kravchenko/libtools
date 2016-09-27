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

#ifndef __LIBTOOLS_POPEN2_H
#define __LIBTOOLS_POPEN2_H

#include <stdio.h>
#include <unistd.h>

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief Run process and return his standard streams
 * @param [in] path executable filename
 * @param [out] in pointer to stdin stream for child process
 * @param [out] out pointer to stdout stream for child process
 * @param [out] err pointer to stderr stream for child process
 * @return process id
 * @retval -1 error occurred
 * @remarks don't forget close standard streams and wait for child
 *
 * If error occurred function update #errno - see errors specified for
 * the routines close(), fclose(), fdopen(), fork() and pipe().
 */
pid_t popen2(const char *path, FILE **in, FILE **out, FILE **err);

/** @} */

#endif /* __LIBTOOLS_POPEN2_H */
