/**
 * @file
 *
 * Copyright (C) 2017  Oleh Kravchenko <oleg@kaa.org.ua>
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

#ifndef __LIBTOOLS_INPUT_H
#define __LIBTOOLS_INPUT_H

#include <stddef.h>

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief Like fgets() but with few exceptions
 * @param [out] s pointer to string buffer
 * @param [in] n length of string buffer
 * @param [in] stream file input stream
 * @return pointer to string buffer
 * @retval NULL error occurred
 *
 * It behaves like fgets() but ignores the part of a string that doesn't fit
 * in buffer and without '\n' on string end.
 *
 * Function updates #errno, if error occurred:
 * @li #EIO stdin is closed or I/O error
 * @li #ENOBUFS buffer is small for inputted or default string
 */
char *fgetz(char *s, int n, FILE *stream);

/**
 * @brief Read string from stdin with prompt
 * @param [in] prompt string that is displayed as a prompt to stdout
 * @param [in] def default string value
 * @param [out] s pointer to string buffer
 * @param [in] size size of buffer @p s
 * @return zero returned
 * @retval -1 error occurred
 * @sa fgetz
 *
 * Function updates #errno, if error occurred:
 * @li #ENOBUFS buffer is small for inputted or default string
 */
int prompts(const char *prompt, const char *def, char *s, size_t size);

/**
 * @brief Read integer from stdin with prompt
 * @param [in] prompt string that is displayed as a prompt to stdout
 * @param [in] def default integer value
 * @param [out] i pointer to integer
 * @return zero returned
 * @retval -1 error occurred
 * @sa prompts, strtol
 *
 * Function updates #errno, if error occurred:
 * @li #EINVAL non numeric input
 * @li #ERANGE numerical result out of range
 */
int prompti(const char *prompt, int def, int *i);

/** @} */

#endif /* __LIBTOOLS_INPUT_H */
