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

#ifndef __TOOLS_URL_H
#define __TOOLS_URL_H

#include <stdint.h>

/**
 * @addtogroup strings
 * @defgroup url URL parsing
 * @brief Just wrapper around http://uriparser.sourceforge.net/
 *
 * For more details about syntax of URL please take a look:
 * @li https://tools.ietf.org/html/rfc3986
 * @li http://uriparser.sourceforge.net/
 *
 * @{
 */

/*------------------------------------------------------------------------*/

/** parserd URL, see url_parse() */
struct url {
	/** scheme (protocol) */
	char *scheme;

	/** username for URL */
	char *username;

	/** password for URL */
	char *password;

	/** hostname for URL */
	char *hostname;

	/** port for URL */
	uint16_t port;

	/** path of resource URL */
	char *path;

	/** query to resource */
	char *query;

	/** fragment */
	char *fragment;
};

/**
 * @brief Parse string as URL
 * @param [in] s URL string
 * @param [out] u pointer to URL parsed parts
 * @return pointer to URL structure
 * @retval NULL error occurred
 *
 * Parsed URL will be extracted to struct url.
 *
 * For more details about syntax of URL please take a look:
 * @li https://tools.ietf.org/html/rfc3986
 * @li http://uriparser.sourceforge.net/
 */
struct url *url_parse(const char *s, struct url **u);

/**
 * @brief Free memory used by URL structure
 * @param [in] u pointer to URL structure
 */
void url_free(struct url *u);

/** @} */

#endif /* __TOOLS_URL_H */
