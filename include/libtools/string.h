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

#ifndef __LIBTOOLS_STRING_H
#define __LIBTOOLS_STRING_H

#include <stddef.h>
#include <string.h>

/**
 * @addtogroup strings
 *
 * @{
 */

/**
 * @brief Concatenate dynamically allocated string with constant string
 * @param [in,out] dest pointer to pointer of dynamically allocated string
 * @param [in] src pointer to constant string
 * @return pointer to concatenated string
 * @retval NULL error occurred
 * @remarks @p dest can point to NULL value
 * @remarks @p src can be NULL
 */
char *str_cat(char **dest, const char *src);

/**
 * @brief Similar to str_cat() except it will use at most @p n bytes from @p src
 * @param [in,out] dest pointer to pointer of dynamically allocated string
 * @param [in] src pointer to constant string
 * @param [in] n limit constant string length
 * @return pointer to concatenated string
 * @retval NULL error occurred
 * @remarks @p dest can point to NULL value
 * @remarks @p src can be NULL
 */
char *str_ncat(char **dest, const char *src, size_t n);

/**
 * @brief Replace all occurrences of the search string with the replacement string
 * @param [in] search value being searched for
 * @param [in] replace replacement value that replaces found search values
 * @param [in] subject string being searched and replaced on
 * @return pointer to allocated string
 * @retval NULL error occurred
 */
char *str_replace(const char *search, const char *replace, const char *subject);

/**
 * @brief Size-bounded string copying
 * @param [in] dst where to copy the string to
 * @param [in] src where to copy the string from
 * @param [in] size size of the destination buffer
 * @return total length of the string tried to create
 */
size_t strlcpy(char *dst, const char *src, size_t size);

/**
 * @brief Size-bounded string concatenation
 * @param [in] dst string to be appended to
 * @param [in] src string to append to it
 * @param [in] size size of the destination buffer
 * @return total length of the string tried to create
 */
size_t strlcat(char *dst, const char *src, size_t size);

/**
 * @brief Check, if string starts with substring
 * @param [in] s pointer to string
 * @param [in] ss pointer to substring
 * @return result of comparison
 * @retval 0 string is starting from substring
 */
static inline int str_startswith(const char *s, const char *ss)
{
	return (!strncmp(s, ss, strlen(ss)));
}

/** @} */

#endif /* __LIBTOOLS_STRING_H */
