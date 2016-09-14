#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "libtools/string.h"

/*------------------------------------------------------------------------*/

char *str_cat(char **dest, const char *src)
{
	assert(dest);

	size_t dlen = *dest ? strlen(*dest) : 0;
	size_t slen = src ? strlen(src) : 0;
	size_t tlen = dlen + slen;

	char *s = realloc(*dest, tlen + 1);

	if (s) {
		/* suppress __nonnull warning */
		if (src) {
			memcpy(s + dlen, src, slen);
		}

		s[tlen] = 0;
		*dest = s;
	}

	return (s);
}

/*------------------------------------------------------------------------*/

char *str_ncat(char **dest, const char *src, size_t n)
{
	assert(dest);

	size_t dlen = *dest ? strlen(*dest) : 0;
	size_t slen = src ? strnlen(src, n) : 0;
	size_t tlen = dlen + slen;

	char *s = realloc(*dest, tlen + 1);

	if (s) {
		/* suppress __nonnull warning */
		if (src) {
			memcpy(s + dlen, src, slen);
		}

		s[tlen] = 0;
		*dest = s;
	}

	return (s);
}

/*------------------------------------------------------------------------*/

char *str_replace(const char *search, const char *replace, const char *subject)
{
	assert(search && *search);
	assert(replace);
	assert(subject);

	size_t reslen = 0;
	char *res = NULL;

	const size_t searchlen = strlen(search);
	const size_t replacelen = strlen(replace);

	/* search needle and replace it */
	char *p, *needle;

	while ((needle = strstr(subject, search))) {
		const size_t headlen = needle - subject;

		p = realloc(res, reslen + headlen + replacelen + 1);

		if (!p) {
			free(res);

			return (NULL);
		}

		res = p;
		memcpy(res + reslen, subject, headlen);
		memcpy(res + reslen + headlen, replace, replacelen);

		subject = needle + searchlen;
		reslen += headlen + replacelen;
	}

	/* concatenate result with tail */
	if (subject) {
		const size_t taillen = strlen(subject);

		if (!(p = realloc(res, reslen + taillen + 1))) {
			free(res);

			return (NULL);
		}

		res = p;
		memcpy(res + reslen, subject, taillen);
		reslen += taillen;
	}

	res[reslen] = 0;

	return (res);
}

/*------------------------------------------------------------------------*/

size_t strlcpy(char *dst, const char *src, size_t size)
{
	assert(dst);
	assert(src);

	char *d = dst;
	const char *s = src;
	size_t n = size;

	if (n != 0 && --n != 0) {
		do {
			if ((*d ++ = *s ++) == 0)
				break;
		} while (--n != 0);
	}

	if (n == 0) {
		if (size != 0) {
			*d = '\0';
		}

		while (*s ++);
	}

	return(s - src - 1);
}

/*------------------------------------------------------------------------*/

size_t strlcat(char *dst, const char *src, size_t size)
{
	assert(dst);
	assert(src);

	char *d = dst;
	const char *s = src;
	size_t n = size;
	size_t dlen;

	while (*d != '\0' && n-- != 0) {
		++ d;
	}

	dlen = d - dst;
	n = size - dlen;

	if (n == 0) {
		return(dlen + strlen(s));
	}

	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			-- n;
		}

		++ s;
	}

	*d = '\0';

	return(dlen + (s - src));
}
