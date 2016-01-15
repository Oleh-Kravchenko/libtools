#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "tools/trim.h"

/*------------------------------------------------------------------------*/

void trim(char *s)
{
	assert(s);

	size_t len = strlen(s);
	size_t r, l, i;

	if (!len) {
		return;
	}

	/* trim from right */
	r = i = len;

	do {
		if (!isspace(s[-- i])) {
			break;
		}
	} while((r = i));

	if (r < len) {
		s[r] = 0;
	}

	/* trim from left */
	for (l = 0; l < r && isspace(s[l]); ++ l);

	if (l > 0) {
		memmove(s, s + l, r - l + 1);
	}
}

/*------------------------------------------------------------------------*/

void ltrim(char *s)
{
	assert(s);

	size_t len = strlen(s);
	size_t l;

	if (!len) {
		return;
	}

	for (l = 0; l < len && isspace(s[l]); ++ l);

	if (l > 0) {
		memmove(s, s + l, len - l + 1);
	}
}

/*------------------------------------------------------------------------*/

void rtrim(char *s)
{
	assert(s);

	size_t len = strlen(s);
	size_t r, i;

	if (!len) {
		return;
	}

	r = i = len;

	do {
		if (!isspace(s[-- i])) {
			break;
		}
	} while((r = i));

	if (r < len) {
		s[r] = 0;
	}
}
