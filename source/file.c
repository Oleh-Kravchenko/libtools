#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "libtools/file.h"

/*------------------------------------------------------------------------*/

int put_file_contents(const char *path, const void *buf, size_t len)
{
	assert(path);
	assert(buf);

	size_t ret;
	FILE *f;

	if (!(f = fopen(path, "wb"))) {
		return(-1);
	}

	ret = fwrite(buf, 1, len, f);
	fclose(f);

	return (len == ret ? 0 : -1);
}

/*------------------------------------------------------------------------*/

void *get_file_contents(const char *path, void **buf, size_t *len)
{
	assert(path);
	assert(buf);
	assert(len);

	struct stat st;
	FILE *f;

	if (stat(path, &st)) {
		return (NULL);
	}

	if (!(f = fopen(path, "rb"))) {
		return (NULL);
	}

	if (!(*buf = malloc(st.st_size))) {
		fclose(f);

		return (NULL);
	}

	if (fread(*buf, 1, st.st_size, f) != (size_t)st.st_size) {
		free(*buf);
		fclose(f);

		return (*buf = NULL);
	}

	fclose(f);

	/* return file size */
	*len = st.st_size;

	/* return file data */
	return (*buf);
}
