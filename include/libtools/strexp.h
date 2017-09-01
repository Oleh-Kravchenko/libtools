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

#ifndef __LIBTOOLS_STREXP_H
#define __LIBTOOLS_STREXP_H

/** expansion of strings */
struct strexp {
	/** number of strings in @p argv */
	int argc;

	/** pointer to strings */
	char **argv;

	/** buffer of strings */
	char *str;
};

/**
 * @brief Perform string expansion like argv && argc
 * @param [in] str pointer to string for expansion
 * @param [out] args pointer to expansion
 * @return zero returned
 * @retval -1 error occurred
 */
int strexp(const char *str, struct strexp *args);

/**
 * @brief Free resources allocated for expansion
 * @param [in] args pointer to expansion
 */
void strexp_free(struct strexp *args);

#endif /* __LIBTOOLS_STREXP_H */
