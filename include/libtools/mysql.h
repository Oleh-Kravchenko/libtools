/**
 * @file
 *
 * Copyright (C) 2015  Oleh Kravchenko <oleg@kaa.org.ua>
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

#ifndef __LIBTOOLS_MYSQL_H
#define __LIBTOOLS_MYSQL_H

#include <stdarg.h>
#include <mysql/mysql.h>

/**
 * @addtogroup tools
 *
 * @{
 */

/**
 * @copydoc mysql_asprintf()
 * @param [in] args list of arguments
 */
char *mysql_vasprintf(MYSQL *mysql, char **str, const char *format, va_list args);

/**
 * @brief allocate and format mysql query string
 * @param [in] mysql mysql connection handle
 * @param [out] str mysql query string
 * @param [in] format format of output string
 * @return pointer to allocated string
 * @retval NULL error occurred
 *
 * This function is similar to asprintf(), but has another string format.
 * Next interpreted sequences supported:
 * @li @b \%int\% argument is integer
 * @li @b \%double\% argument is double
 * @li @b \%bool\% same as \%int\%, boolean
 * @li @b \%str\% NULL-terminated string, can be NULL
 * @li @b \%time\% unix time, time_t
 * @li @b \%\% one character '\%'
 *
 * Zero \%int\% values, empty or NULL strings (\%str\%) serialized
 * to SQL NULL. Boolean values serialized to TRUE (!= 0) or FALSE (0)
 *
 * Next sequences never serialized to NULL:
 * @li @b \%INT\% argument is integer
 * @li @b \%DOUBLE\% argument is double
 * @li @b \%STR\% NULL-terminated string, can be NULL
 * @li @b \%RSTR\% string without quotes
 *
 * Function updates #errno, if error occurred:
 * @li EINVAL argument @p format have invalid interpreted sequences
 */
char *mysql_asprintf(MYSQL *mysql, char **str, const char *format, ...);

/**
 * @brief Start new transaction
 * @param [in] mysql connection handle
 * @return on success, zero is returned
 * @retval -1 error occurred
 * @remarks automatically commit current transaction, before starting new
 */
int mysql_transaction(MYSQL *mysql);

/**
 * @brief Send a MySQL query
 * @param [in] mysql mysql connection handle
 * @param [in] format mysql query string
 * @return on success, zero is returned
 * @retval -1 error occurred
 * @sa mysql_asprintf
 *
 * Function update #errno, if error occurred:
 * @li #EIO I/O error (in most cases MySQL error)
 */
int mysql_qprintf(MYSQL *mysql, const char *format, ...);

/** @} */

#endif /* __LIBTOOLS_MYSQL_H */
