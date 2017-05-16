#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libtools/input.h"
#include "libtools/string.h"
#include "libtools/tools.h"

/*------------------------------------------------------------------------*/

#define EOL		((int)'\n')
#define INTEGER_STRLEN	12

/*------------------------------------------------------------------------*/

char *fgetz(char *s, int n, FILE *stream)
{
	assert(s);
	assert(n > 0);
	assert(stream);

	if (!fgets(s, n, stream)) {
		errno = EIO;

		return (NULL);
	}

	char *end = s + strlen(s) - 1;

	/* inputted string fit in buffer */
	if (*end == EOL) {
		/* remove EOL from string */
		*end = '\0';

		return (s);
	}

	int c = fgetc(stdin);

	/* input is ended or stdin closed, return success */
	if (c == EOF || c == EOL) {
		return (s);
	}

	/* read until EOF/EOL and return error */
	do {
		c = fgetc(stdin);
	} while (c != EOL && c != EOF);

	errno = ENOBUFS;

	return (NULL);
}

/*------------------------------------------------------------------------*/

int prompts(const char *prompt, const char *def, char *s, size_t size)
{
	assert(prompt);
	assert(def);
	assert(s);

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

/*------------------------------------------------------------------------*/

int prompti(const char *prompt, int def, int *i)
{
	assert(prompt);
	assert(i);

	char defstr[INTEGER_STRLEN];
	ssize_t deflen;

	/* convert default integer value to default string for prompt */
	deflen = snprintf(defstr, sizeof(defstr), "%d", def);

	unused(deflen);
	assert(deflen < (ssize_t)sizeof(defstr));

	char str[INTEGER_STRLEN];

	/* read input */
	if (prompts(prompt, defstr, str, sizeof(str))) {
		return (-1);
	}

	/* if empty input use default integer value */
	if (!*str) {
		*i = def;

		return (0);
	}

	errno = 0;

	char *endptr = str;
	long int val = strtol(str, &endptr, 10);

	/* check strtol() errors */
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
		(errno && !val)) {
		return (-1);
	}

	/* if the string is not parsed, return an error */
	if (str == endptr || *endptr) {
		errno = EINVAL;

		return (-1);
	}

	/* check integer underflow or overflow */
	if (val < INT_MIN || val > INT_MAX) {
		errno = ERANGE;

		return (-1);
	}

	*i = val;

	return (0);
}
