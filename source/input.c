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

	if (!fgetz(s, size, stdin)) {
		return (-1);
	}

	/* if empty input, try to use default string */
	if (!*s && strlcpy(s, def, size) >= size) {
		errno = ENOBUFS;

		return (-1);
	}

	return (0);
}

/*------------------------------------------------------------------------*/

int s_prompts(const char *prompt, const char *def, char *s, size_t size)
{
	assert(prompt);
	assert(def);
	assert(s);

	if (def != s) {
		return (prompts(prompt, def, s, size));
	}

	int rc = -1;
	char *buf;

	if (!(buf = malloc(size))) {
		return (rc);
	}

	if (!(rc = prompts(prompt, def, buf, size))) {
		strcpy(s, buf);
	}

	free(buf);

	return (rc);
}

/*------------------------------------------------------------------------*/

int prompti(const char *prompt, int def, int *i)
{
	assert(prompt);
	assert(i);

	printf("%s [%d]: ", prompt, def);

	char s[INTEGER_STRLEN];

	/* read input */
	if (!fgetz(s, sizeof(s), stdin)) {
		return (-1);
	}

	/* if empty input use default integer value */
	if (!*s) {
		*i = def;

		return (0);
	}

	errno = 0;

	char *endptr = s;
	long int val = strtol(s, &endptr, 10);

	/* check strtol() errors */
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
		(errno && !val)) {
		return (-1);
	}

	/* if the string is not parsed, return an error */
	if (s == endptr || *endptr) {
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
