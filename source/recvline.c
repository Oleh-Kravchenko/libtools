#include <assert.h>
#include <sys/socket.h>

#include "tools/recvline.h"

/*------------------------------------------------------------------------*/

ssize_t recvline(int fd, char *buf, size_t size, const char *d, size_t dlen)
{
	assert(buf);
	assert(d);

	size_t eolindex = 0;
	size_t received = 0;

	char c;
	ssize_t rc;

	while(received < size && eolindex < dlen) {
		if ((rc = recv(fd, &c, sizeof(c), 0)) <= 0) {
			/* connection closed? error? */
			return (rc);
		}

		/* increment or reset delimiter match */
		if (d[eolindex] == c) {
			++ eolindex;
		} else {
			eolindex = 0;
		}

		buf[received ++] = c;
	}

	return (received);
}
