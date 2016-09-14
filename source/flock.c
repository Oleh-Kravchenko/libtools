#include <assert.h>
#include <sys/file.h>

#include "libtools/flock.h"

/*------------------------------------------------------------------------*/

FILE *flopen(const char *path, const char *mode, FILE **fp)
{
	assert(path);
	assert(mode);
	assert(fp);

	if (!(*fp = fopen(path, mode))) {
		return (*fp);
	}

	int fd;

	/* place an exclusive lock on file */
	if (-1 == (fd = fileno(*fp)) || -1 == flock(fd, LOCK_EX)) {
		fclose(*fp);

		*fp = NULL;
	}

	return (*fp);
}

/*------------------------------------------------------------------------*/

FILE *fltryopen(const char *path, const char *mode, FILE **fp)
{
	assert(path);
	assert(mode);
	assert(fp);

	if (!(*fp = fopen(path, mode))) {
		return (*fp);
	}

	int fd;

	/* try place an exclusive lock on file */
	if (-1 == (fd = fileno(*fp)) || -1 == flock(fd, LOCK_EX | LOCK_NB)) {
		fclose(*fp);

		*fp = NULL;
	}

	return (*fp);
}

/*------------------------------------------------------------------------*/

int flclose(FILE *fp)
{
	if (!fp) {
		return (0);
	}

	int fd;

	/* remove exclusive lock from file */
	if (-1 == (fd = fileno(fp)) || -1 == flock(fd, LOCK_UN)) {
		return (-1);
	}

	return (fclose(fp));
}
