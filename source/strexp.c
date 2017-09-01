#include <stdlib.h>
#include <string.h>

#include "libtools/strexp.h"

/*------------------------------------------------------------------------*/

int strexp(const char *str, struct strexp *args)
{
	char *token, *saveptr;
	char **argv;

	args->str = strdup(str);
	if (!args->str) {
		return (-1);
	}

	args->argc = 0;
	args->argv = NULL;

	token = strtok_r(args->str, " ", &saveptr);

	while (token) {
		++ args->argc;

		argv = realloc(args->argv, sizeof(*argv) * args->argc);
		if (!argv) {
			strexp_free(args);

			return (-1);
		}

		args->argv = argv;
		args->argv[args->argc - 1] = token;

		token = strtok_r(NULL, " ", &saveptr);
	}

	return (0);
}

/*------------------------------------------------------------------------*/

void strexp_free(struct strexp *args)
{
	free(args->argv);
	free(args->str);
}
