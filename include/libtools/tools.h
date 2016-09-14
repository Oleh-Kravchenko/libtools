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

#ifndef __LIBTOOLS_TOOLS_H
#define __LIBTOOLS_TOOLS_H

/**
 * @defgroup tools libtools
 *
 * This is a set of application functions that can be useful not only within
 * this project. This set contains functionality for working with sockets,
 * files, MySQL, strings, regular expressions, directories, etc.
 *
 * @{
 */

/*------------------------------------------------------------------------*/

/**
 * @defgroup strings String routines
 * @ingroup tools
 *
 * Functionality for creating SQL queries, making strong passwords, trimming
 * a string if there are any space characters, parsing string as URL
 */

/*------------------------------------------------------------------------*/

/**
 * @defgroup network Network routines
 * @ingroup tools
 *
 * HTTP server, HTTP client, socket functionality
 */

/*------------------------------------------------------------------------*/

/**
 * @defgroup io Input/Output routines
 * @ingroup tools
 *
 * Functionality includes closing descriptors of current process, running
 * server as a daemon, recursive remove of a directory or a file, opening
 * and locking file, closing file
 */

/*------------------------------------------------------------------------*/

/**
 * @def countof
 *
 * Calculate length of array
 */
#ifndef countof
#	define countof(x) (sizeof(x) / sizeof(x[0]))
#endif /* countof */

/**
 * @def unused
 *
 * Variable can be not used in Release build,
 * to suppress compiler warning this macro should be used.
 */
#ifndef unused
#	define unused(x) ((void)x)
#endif /* unused */

/**
 * @def min
 *
 * return minimal number
 */
#ifndef min
#	define min(a, b) __extension__ ({                                 \
		typeof (a) _a = (a);                                      \
		typeof (b) _b = (b);                                      \
		_a > _b ? _b : _a;                                        \
	})
#endif /* min */

/**
 * @def max
 *
 * return maximum number
 */
#ifndef max
#	define max(a, b) __extension__ ({                                 \
		typeof (a) _a = (a);                                      \
		typeof (b) _b = (b);                                      \
		_a < _b ? _b : _a;                                        \
	})
#endif /* max */

/**
 * @def cast_ptr
 * @brief cast pointer
 * @param [in] t target type
 * @param [in] x pointer
 */
#ifndef cast_ptr
#	define cast_ptr(t, x)  __extension__ ({                           \
		(typeof(t)*)x;                                            \
	})
#endif /* cast_ptr */

/**
 * @def __stringify
 *
 * This macro do indirect stringification of input arguments,
 * mostly used with `__LINE__`.
 */
#ifndef __stringify
#	define __stringify_1(...) #__VA_ARGS__
#	define __stringify(...) __stringify_1(__VA_ARGS__)
#endif /* __stringify */

/** @} */

#endif /* __LIBTOOLS_TOOLS_H */
