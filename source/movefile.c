#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tools/movefile.h"
#include "tools/tools.h"

/*------------------------------------------------------------------------*/

int movefile(const char *oldpath, const char *newpath)
{
	assert(oldpath);
	assert(newpath);

	/* try common way */
	if (!rename(oldpath, newpath)) {
		return (0);
	}

	int oldfd;
	int newfd = -1;

	/* newpath at another file system or it really error */
	if (EXDEV != errno) {
		return (-1);
	}

	if (-1 == (oldfd = open(oldpath, O_RDONLY)) ||
		-1 == (newfd = creat(newpath, S_IRUSR | S_IWUSR))
	) {
		goto err;
	}

	struct stat stat;

	/* retrive file size */
	if (fstat(oldfd, &stat)) {
		goto err;
	}

	char chunk[PATH_MAX];
	size_t len;

	while (stat.st_size) {
		len = min((size_t)stat.st_size, sizeof(chunk));

		if (read(oldfd, chunk, len) != (ssize_t)len) {
			goto err;
		}

		if (write(newfd, chunk, len) != (ssize_t)len) {
			goto err;
		}

		stat.st_size -= len;
	}

	close(oldfd);
	close(newfd);
	unlink(oldpath);

	return (0);

err:
	/* moving failed, so keep old file .. */
	if (-1 != oldfd) {
		close(oldfd);
	}

	/* and remove new file */
	if (-1 != newfd) {
		close(newfd);
		unlink(newpath);
	}

	return (-1);
}
