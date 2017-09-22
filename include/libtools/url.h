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

#ifndef __LIBTOOLS_URL_H
#define __LIBTOOLS_URL_H

#include <stdint.h>
#include <sys/socket.h>

/**
 * @addtogroup strings
 * @defgroup url URL parsing
 * @ingroup tools
 * @brief Just wrapper around http://uriparser.sourceforge.net/
 *
 * For more details about syntax of URL please take a look:
 * @li https://tools.ietf.org/html/rfc3986
 * @li http://uriparser.sourceforge.net/
 *
 * @{
 */

/*------------------------------------------------------------------------*/

/** parsed URL, see url_parse() */
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
 * @retval NULL error occurred, @p u will be assigned to NULL
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
 * @param [in] u pointer to URL structure (can be NULL)
 */
void url_free(struct url *u);

/**
 * @brief Set socket address according to URL
 * @param [in] u pointer to URL structure
 * @param [out] sa pointer to socket address structure
 * @return on success, zero is returned
 * @retval -1 error occurred
 *
 * Support next schemes:
 * @li @p http Internet sockets, example: http://localhost/
 * @li @p unix sockets for local interprocess communication,
 * example: unix:/var/run/server.sock
 * @li @p local alias for @p unix sockets
 */
int url2sockaddr(struct url *u, struct sockaddr_storage *sa);

/**
 * @brief Return real size of socket address
 * @param [in] sa pointer to socket address storage
 * @return real size of socket address
 */
socklen_t sockaddr_size(const struct sockaddr_storage *sa);

/** @} */

#endif /* __LIBTOOLS_URL_H */
