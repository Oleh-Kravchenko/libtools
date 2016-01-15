#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/socket.h>

#include "tools/sendfile.h"
#include "tools/tools.h"

/*------------------------------------------------------------------------*/

int sendfile(int sockfd, const char *path, size_t size)
{
	assert(path);

	FILE *f;

	/* try to open file */
	if (!(f = fopen(path, "r"))) {
		return (-1);
	}

	/* read and send chunks */
	while (size && !feof(f)) {
		char buf[PATH_MAX];
		ssize_t sent;

		/* read chunk from file */
		sent = min(size, sizeof(buf));
		sent = fread(buf, 1, sent, f);

		/* read failed? */
		if (!sent) {
			errno = EIO;

			break;
		}

		/* send chunk */
		if ((sent = send(sockfd, buf, sent, 0)) <= 0) {
			/* connection lost? */
			if (!sent) {
				errno = EPIPE;
			}

			break;
		}

		size -= sent;
	}

	if (fclose(f)) {
		return (-1);
	}

	/* verify for unexpected file end */
	return (size ? -1 : 0);
}
