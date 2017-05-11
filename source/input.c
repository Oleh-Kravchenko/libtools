#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "libtools/input.h"
#include "libtools/string.h"

/*------------------------------------------------------------------------*/

#define EOL ((int)'\n')

/*------------------------------------------------------------------------*/

int prompts(const char *prompt, const char *def, char *s, size_t size)
{
	printf("%s [%s]: ", prompt, def);

	if (!fgets(s, size, stdin)) {
		errno = EIO;

		return (-1);
	}

	char *end = s + strlen(s) - 1;

	/* inputted string don't fit to buffer */
	if (*end != EOL) {
		int c = fgetc(stdin);

		/* if input is ended or stdin closed, return success */
		if (c == EOF || c == EOL) {
			return (0);
		}

		/* read until EOF/EOL and return error */
		do {
			c = fgetc(stdin);
		} while (c != EOL && c != EOF);

		errno = ENOBUFS;

		return (-1);
	}

	/* remove EOL from string */
	*end = '\0';

	/* if empty input, try to use default string */
	if (!*s && strlcpy(s, def, size) >= size) {
		errno = ENOBUFS;

		return (-1);
	}

	return (0);
}
