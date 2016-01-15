#include <assert.h>
#include <sys/socket.h>

#include "tools/recvline.h"

/*------------------------------------------------------------------------*/

ssize_t recvline(int sockfd, char *buf, size_t size, const char *s, size_t len)
{
	assert(sockfd >= 0);
	assert(buf);
	assert(size);
	assert(s);
	assert(len);

	size_t received = 0;
	size_t eolindex = 0;
	char c;

	do {
		ssize_t ret;

		if ((ret = recv(sockfd, &c, sizeof(c), 0)) <= 0) {
			/* connection closed? error? */
			return (ret);
		}

		/* end of line check */
		eolindex = (s[eolindex] == c ? eolindex + 1 : 0);

		buf[received ++] = c;
	} while(received < size && eolindex < len);

	return (received);
}
