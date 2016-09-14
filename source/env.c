#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "libtools/env.h"

/*------------------------------------------------------------------------*/

char *read_env(const char *env, char *s, size_t len)
{
	assert(env);
	assert(s);

	char *val;

	if(!(val = getenv(env))) {
		return(NULL);
	}

	strncpy(s, val, len - 1);
	s[len - 1] = 0;

	return(s);
}
