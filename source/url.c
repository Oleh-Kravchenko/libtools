#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "tools/url.h"

/*------------------------------------------------------------------------*/

static int isschema(int c)
{
	return ('+' == c || '-' == c || '.' == c || islower(c) || isdigit(c));
}

/*------------------------------------------------------------------------*/

static int isuserinfo(int c)
{
	return (isalnum(c) ||
		'\'' == c ||
		'$' == c ||
		'-' == c ||
		'_' == c ||
		'.' == c ||
		'+' == c ||
		'!' == c ||
		'*' == c ||
		'(' == c ||
		')' == c ||
		',' == c ||
		';' == c ||
		'?' == c ||
		'&' == c ||
		'=' == c ||
		'%' == c
	);
}

/*------------------------------------------------------------------------*/

static int ishostname(int c)
{
	return (isalnum(c) || '.' == c || '-' == c);
}

/*------------------------------------------------------------------------*/

static int ispath(int c)
{
	return (isuserinfo(c) && '?' != c && '#' != c);
}

/*------------------------------------------------------------------------*/

static int isquery(int c)
{
	return (isuserinfo(c) && '#' != c);
}

/*------------------------------------------------------------------------*/

int url_unpack(char *s, struct url *url)
{
	assert(s);
	assert(url);

	char *head = s;
	char *tail = s;

	memset(url, 0, sizeof(*url));

	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 * ^
	 * schema always starts from lower case character
	 */
	if (islower(*tail)) {
		do {
			++ tail;
		} while (isschema(*tail));

		/* schema should be end by "://" */
		if (!strncmp(tail, "://", strlen("://"))) {
			*tail = '\0';

			url->schema = head;
			head = tail += strlen("://");
		} else {
			/* no schema, reset state */
			tail = head;
		}
	}


	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *          ^
	 */
	if (isuserinfo(*tail)) {
		do {
			++ tail;
		} while (isuserinfo(*tail));

		/* username should be ended by ":" or "@" */
		if ('@' == *tail || ':' == *tail) {
			url->username = head;

			int next_ishost = '@' == *tail;

			*tail = '\0';
			head = ++ tail;

			if (next_ishost) {
				goto hostname;
			}
		} else {
			tail = head;
		}
	}


	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *               ^
	 */
	if (url->username && isuserinfo(*tail)) {
		do {
			++ tail;
		} while (isuserinfo(*tail));

		/* username should be ended by "@" */
		if ('@' == *tail) {
			url->password = head;

			*tail = '\0';
			head = ++ tail;
		} else {
			tail = head;

			/* username is a hostname */
			url->hostname = url->username;
			url->username = NULL;

			goto port;
		}
	}


	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *                    ^
	 */
hostname:
	if (!isalnum(*tail)) {
		/* if we don't have hostname, it end */
		return (-1);
	}

	do {
		++ tail;
	} while (ishostname(*tail));

	/* hostname should end by ":", "/" or by "\0" */
	if (':' == *tail || '/' == *tail || !*tail) {
		url->hostname = head;

		if (!*tail) {
			goto done;
		}

		int next_isresource = '/' == *tail;

		*tail = '\0';
		head = ++ tail;

		if (next_isresource) {
			goto resource;
		}
	} else {
		/* invalid hostname? */
		return (-1);
	}


	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *                         ^
	 */
port:
	if (isdigit(*tail)) {
		do {
			++ tail;
		} while (isdigit(*tail));

		/* port should be ended by "/" or "\0" */
		if ('/' == * tail || !*tail) {
			url->port = head;

			if (!*tail) {
				goto done;
			}

			*tail = '\0';
			head = ++ tail;
		} else {
			/* invalid port? */
			return (-1);
		}
	}


	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *                              ^
	 */
resource:
	if (ispath(*tail)) {
		do {
			++ tail;
		} while (ispath(*tail));

		/* path should be ended by "?", "#" or "\0" */
		if ('?' == * tail || '#' == *tail || !*tail) {
			url->resource = head;

			if (!*tail) {
				goto done;
			}

			int next_istag = '#' == *tail;

			*tail = '\0';
			head = ++ tail;

			if (next_istag) {
				goto tag;
			}
		} else {
			tail = head;
		}
	}


	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *                                  ^
	 */
	if (isquery(*tail)) {
		do {
			++ tail;
		} while (isquery(*tail));

		/* query should be ended by "#" or "\0" */
		if ('#' == *tail || !*tail) {
			/* path can be empty and query can start from "?" */
			if ('?' == *head) {
				++ head;
			}

			url->query = head;

			if (!*tail) {
				goto done;
			}

			*tail = '\0';
			head = ++ tail;
		}
	}

	/* we are here:
	 * SCHEMA://USER:PASS@HOST:PORT/PATH?QUERY#TAG
	 *                                        ^
	 */
tag:
	if (*tail) {
		/* query can be empty and tag can start from "#" */
		if ('#' == *head) {
			++ head;
		}

		url->tag = head;
	}

done:
	return (0);
}
