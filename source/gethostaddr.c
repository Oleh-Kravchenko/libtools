#include <arpa/inet.h>
#include <assert.h>
#include <limits.h>
#include <netdb.h>
#include <stddef.h>

#include "tools/gethostaddr.h"

/*------------------------------------------------------------------------*/

struct in_addr *gethostaddr(const char *hostname, struct in_addr *addr)
{
	assert(hostname);
	assert(addr);

	/* try as IPv4 address */
	if (!inet_aton(hostname, addr)) {
		struct hostent h, *rh;
		char t[PATH_MAX];
		int h_errno;

		/* try to resolve as hostname */
		if (gethostbyname_r(hostname, &h, t, sizeof(t), &rh, &h_errno)) {
			return(NULL);
		}

		*addr = *((struct in_addr*)h.h_addr);
	}

	return (addr);
}
