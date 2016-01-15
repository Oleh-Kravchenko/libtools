#include <stdlib.h>

#include "tools/mkpasswd.h"
#include "tools/tools.h"

/*------------------------------------------------------------------------*/

char *mkpasswd(char *s, size_t len)
{
	if(!s || !len) {
		return(NULL);
	}

	const static char dict[] = "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	size_t i;

	for (i = 0; i < len - 1; ++ i) {
		s[i] = dict[random() % (countof(dict) - 1)];
	}

	s[i] = 0;

	return(s);
}
