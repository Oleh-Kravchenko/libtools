#include <assert.h>
#include <regex.h>
#include <stdlib.h>

#include "libtools/re.h"

/*------------------------------------------------------------------------*/

int re_strcmp(const char *s, const char *mask)
{
	assert(s);
	assert(mask);

	regex_t re;
	int ret;

	if (regcomp(&re, mask, REG_EXTENDED)) {
		return (-1);
	}

	ret = regexec(&re, s, 0, NULL, 0);
	regfree(&re);

	return (ret);
}
