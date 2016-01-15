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

/**
 * @addtogroup strings
 *
 * @{
 */

/*------------------------------------------------------------------------*/

/** unpacked url, see url_unpack() */
struct url {
	/** schema(protocol) */
	char *schema;

	/** username for url */
	char *username;

	/** password for url */
	char *password;

	/** hostname for url */
	char *hostname;

	/** port for url */
	char *port;

	/** path of resource url */
	char *resource;

	/** query to resource */
	char *query;

	/** tag */
	char *tag;
};

/**
 * @brief parse string as url
 * @param [in] s url string
 * @param [out] url unparsed strings of url
 * @return zero is returned
 * @retval -1 error occurred
 *
 * This function parse url of this scheme:
 * [SCHEMA://][USER[:PASS]@]HOST[:PORT][/[PATH][?QUERY][\#tag]]
 */
int url_unpack(char *s, struct url *url);

/** @} */

#endif /* __TOOLS_URL_H */
