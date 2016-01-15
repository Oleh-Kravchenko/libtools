#include <assert.h>
#include <stdint.h>
#include <sys/socket.h>

#include "tools/recvall.h"

/*------------------------------------------------------------------------*/

ssize_t recvall(int sockfd, void *buf, size_t len, int flags)
{
	assert(sockfd >= 0);
	assert(buf);
	assert(len);

	ssize_t received = 0;

#if __linux__
	flags |= MSG_WAITALL;
#endif /* __linux__ */

	while (len) {
		ssize_t ret;

		/* try to fill buf */
		if ((ret = recv(sockfd, buf, len, flags)) <= 0) {
			/* failed? */
			return(ret);
		}

		/* fix indexes */
		received += ret;
		buf = (uint8_t*)buf + ret;
		len -= ret;
	}

	return (received);
}
