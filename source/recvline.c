#include <assert.h>
#include <sys/socket.h>

#include "libtools/recvline.h"

/*------------------------------------------------------------------------*/

ssize_t recvline(int fd, char *buf, size_t size, const char *d, size_t dlen)
{
	assert(buf);
	assert(d);

	size_t eolindex = 0;
	size_t received = 0;

	while(received < size && eolindex < dlen) {
		char sym;

		ssize_t rc = recv(fd, &sym, sizeof(sym), 0);
		if (0 >= rc) {
			/* connection closed? error? */
			return (rc);
		}

		/* increment or reset delimiter match */
		if (d[eolindex] == sym) {
			++ eolindex;
		} else {
			eolindex = 0;
		}

		buf[received ++] = sym;
	}

	return (received);
}
