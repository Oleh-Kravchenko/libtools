#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/socket.h>

#include "libtools/recvfile.h"
#include "libtools/tools.h"

/*------------------------------------------------------------------------*/

int recvfile(int sockfd, const char *path, size_t size)
{
	assert(path);

	FILE *f;

	if (!(f = fopen(path, "w"))) {
		return (-1);
	}

	char buf[PATH_MAX];

	while (size) {
		ssize_t save = min(sizeof(buf), size);

		/* receive chunk */
		save = recv(sockfd, buf, save, 0);
		if (0 >= save) {
			break;
		}

		/* save it to file */
		if (fwrite(buf, 1, save, f) != (size_t)save) {
			errno = ferror(f);

			break;
		}

		size -= save;
	}

	if (fclose(f)) {
		return (-1);
	}

	return (size);
}
