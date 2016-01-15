#include <assert.h>
#include <errno.h>
#include <mysql/mysql.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/mysql.h"

/*------------------------------------------------------------------------*/

char *mysql_vasprintf(MYSQL *mysql, char **str, const char *format, va_list args)
{
	assert(mysql);
	assert(str);
	assert(format);

	const char *fmt = format;
	va_list ap;

	size_t outlen = 1;
	char *out;


	/* first pass - calculate length of output string */
	va_copy(ap, args);

	while (*fmt) {
		if ('%' != *fmt) {
			++ fmt;
			++ outlen;

			continue;
		}

		/* process format and calculate output strings */
		if (!strncmp(fmt, "%int%", strlen("%int%"))) {
			int i = va_arg(ap, int);

			fmt += strlen("%int%");
			outlen += (i ? snprintf(NULL, 0, "%d", i) : strlen("NULL"));
		} else if (!strncmp(fmt, "%INT%", strlen("%INT%"))) {
			fmt += strlen("%INT%");
			outlen += snprintf(NULL, 0, "%d", va_arg(ap, int));
		} else if (!strncmp(fmt, "%double%", strlen("%double%"))) {
			double d = va_arg(ap, double);

			fmt += strlen("%double%");
			outlen += (0 != d ? snprintf(NULL, 0, "%f", d) : strlen("NULL"));
		} else if (!strncmp(fmt, "%DOUBLE%", strlen("%DOUBLE%"))) {
			fmt += strlen("%DOUBLE%");
			outlen += snprintf(NULL, 0, "%f", va_arg(ap, double));
		} else if (!strncmp(fmt, "%bool%", strlen("%bool%"))) {
			fmt += strlen("%bool%");
			outlen += (va_arg(ap, int) ? strlen("TRUE") : strlen("FALSE"));
		} else if(!strncmp(fmt, "%str%", strlen("%str%"))) {
			const char *s = va_arg(ap, const char*);
			size_t len = s ? strlen(s) : 0;

			fmt += strlen("%str%");

			if (len) {
				/*
				* MySQL man says: you must allocate the to buffer to be at least
				* length * 2 + 1 bytes, with quotes result string,
				* should be (length * 2 + 1 + 2),
				* but we already haave space for \0, so (length * 2 + 2)
				*/
				outlen += len * 2 + 2;
			} else {
				outlen += strlen("NULL");
			}
		} else if(!strncmp(fmt, "%STR%", strlen("%STR%"))) {
			const char *s = va_arg(ap, const char*);
			size_t len = s ? strlen(s) : 0;

			fmt += strlen("%STR%");
			outlen += len * 2 + 2;
		} else if (!strncmp(fmt, "%RSTR%", strlen("%rstr"))) {
			const char *s = va_arg(ap, const char*);
			size_t len = s ? strlen(s) : 0;

			fmt += strlen("%RSTR%");
			outlen += len * 2;
		} else if (!strncmp(fmt, "%time%", strlen("%time%"))) {
			time_t t = va_arg(ap, time_t);

			fmt += strlen("%time%");
			outlen += (t ? snprintf(NULL, 0, "FROM_UNIXTIME(%lu)", t) : strlen("NULL"));
		} else if (!strncmp(fmt, "%%", strlen("%%"))) {
			fmt += strlen("%%");
			++ outlen;
		} else {
			errno = EINVAL; /* unknown format? */
			va_end(ap);

			return (NULL);
		}
	}

	va_end(ap);


	/* now allocate memory for output string */
	if (!(*str = out = malloc(outlen))) {
		return (NULL);
	}

	fmt = format;


	/* second pass - assemble output string */
	va_copy(ap, args);

	while (*fmt) {
		if (*fmt != '%') {
			*out ++ = *fmt;
			++ fmt;

			continue;
		}

		if (!strncmp(fmt, "%int%", strlen("%int%"))) {
			int i = va_arg(ap, int);

			fmt += strlen("%int%");
			out += (i ? sprintf(out, "%d", i) : sprintf(out, "NULL"));
		} else if (!strncmp(fmt, "%INT%", strlen("%INT%"))) {
			fmt += strlen("%INT%");
			out += sprintf(out, "%d", va_arg(ap, int));
		} else if (!strncmp(fmt, "%double%", strlen("%double%"))) {
			double d = va_arg(ap, double);

			fmt += strlen("%double%");
			out += (0 != d ? sprintf(out, "%f", d) : sprintf(out, "NULL"));
		} else if (!strncmp(fmt, "%DOUBLE%", strlen("%DOUBLE%"))) {
			fmt += strlen("%DOUBLE%");
			out += sprintf(out, "%f", va_arg(ap, double));
		} else if (!strncmp(fmt, "%bool%", strlen("%bool%"))) {
			fmt += strlen("%bool%");
			out += sprintf(out, "%s", va_arg(ap, int) ? "TRUE" : "FALSE");
		} else if (!strncmp(fmt, "%str%", strlen("%str%"))) {
			const char *s = va_arg(ap, const char*);
			size_t len = s ? strlen(s) : 0;

			fmt += strlen("%str%");

			if (len) {
				*out ++ = '\'';
				out += mysql_real_escape_string(mysql, out, s, len);
				*out ++ = '\'';
			} else {
				out += sprintf(out, "NULL");
			}
		} else if (!strncmp(fmt, "%STR%", strlen("%STR%"))) {
			const char *s = va_arg(ap, const char*);
			size_t len = s ? strlen(s) : 0;

			fmt += strlen("%STR%");

			*out ++ = '\'';
			out += mysql_real_escape_string(mysql, out, s, len);
			*out ++ = '\'';
		} else if (!strncmp(fmt, "%RSTR%", strlen("%RSTR%"))) {
			const char *s = va_arg(ap, const char*);
			size_t len = s ? strlen(s) : 0;

			fmt += strlen("%RSTR%");
			out += mysql_real_escape_string(mysql, out, s, len);
		} else if (!strncmp(fmt, "%time%", strlen("%time%"))) {
			time_t t = va_arg(ap, time_t);

			fmt += strlen("%time%");
			out += (t ? sprintf(out, "FROM_UNIXTIME(%lu)", t) : sprintf(out, "NULL"));
		} else if (!strncmp(fmt, "%%", strlen("%%"))) {
			fmt += strlen("%%");
			*out ++ = '%';
		}
	}

	va_end(ap);

	/* terminate output string by '\0' */
	*out = '\0';

	return (*str);
}

/*------------------------------------------------------------------------*/

char *mysql_asprintf(MYSQL *mysql, char **str, const char *format, ...)
{
	va_list ap;
	char *res;

	va_start(ap, format);
	res = mysql_vasprintf(mysql, str, format, ap);
	va_end(ap);

	return (res);
}

/*------------------------------------------------------------------------*/

int mysql_transaction(MYSQL *mysql)
{
	assert(mysql);

	if (mysql_query(mysql, "START TRANSACTION")) {
		return (-1);
	}

	return (0);
}

/*------------------------------------------------------------------------*/

int mysql_qprintf(MYSQL *mysql, const char *format, ...)
{
	assert(mysql);
	assert(format);

	int rc = -1;

	va_list ap;

	/* parse and format query */
	va_start(ap, format);

	do {
		/* format SQL query */
		char *query;

		if (!mysql_vasprintf(mysql, &query, format, ap)) {
			break;
		}

		/* execute SQL query */
		do {
			if (mysql_query(mysql, query)) {
				errno = EIO;

				break;
			}

			/* query successfully executed */
			rc = 0;
		} while (0);

		free(query);
	} while (0);

	va_end(ap);

	return (rc);
}
