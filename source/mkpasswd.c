#include <assert.h>
#include <stdlib.h>

#include "tools/mkpasswd.h"
#include "tools/tools.h"

/*------------------------------------------------------------------------*/

char *mkpasswd(char *s, size_t size)
{
	assert(s);

	if (!size) {
		return(NULL);
	}

	/* keep space for \0 */
	-- size;

	static const char dict[] = "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (size_t i = 0; i < size; ++ i) {
		s[i] = dict[random() % (countof(dict) - 1)];
	}

	s[size] = 0;

	return(s);
}
