#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "libtools/directory.h"

/*------------------------------------------------------------------------*/

int mkdirectory(const char *path, mode_t mode)
{
	assert(path);

	char *s;

	if (!(s = strndup(path, PATH_MAX))) {
		return (-1);
	}

	/* don't verify root '/', it always exist :) */
	char *slash = s + !!('/' == *s);
	char *chunk = s;
	int rc = 0;

	/* split path by '/' */
	while ((slash = strchr(slash, '/'))) {
		/* truncate path */
		*slash = '\0';

		/* verify and create directory, if it don't exist */
		if (stat(s, &(struct stat){0}) && mkdir(s, mode)) {
			rc = -1;

			goto done;
		}

		/* restore truncated path */
		*slash = '/';

		chunk = ++ slash;
	}

	/* verify, if path not ended by slash */
	if (*chunk && stat(s, &(struct stat){0}) && mkdir(s, mode)) {
		rc = -1;
	}

done:
	free(s);

	return (rc);
}
