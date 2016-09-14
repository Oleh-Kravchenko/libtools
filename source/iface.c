#include <assert.h>
#include <ifaddrs.h>
#include <string.h>

#include "libtools/iface.h"

/*------------------------------------------------------------------------*/

int iface_list(void (*iface_cb)(struct iface*, void*), void *priv)
{
	assert(iface_cb);

	struct ifaddrs *ifa;

	if (getifaddrs(&ifa)) {
		return (-1);
	}

	for (struct ifaddrs *i = ifa; i; i = i->ifa_next) {
		if (!i->ifa_addr) {
			/* i->ifa_addr is NULL for ppp interfaces, so skip it */
			continue;
		}

		/* skip non IPv4 interfaces */
		if (AF_INET != i->ifa_addr->sa_family) {
			continue;
		}

		struct sockaddr_in *sa = (struct sockaddr_in*)i->ifa_addr;

		/* skip loopback interfaces */
		if (IN_LOOPBACK(ntohl(sa->sin_addr.s_addr))) {
			continue;
		}

		struct iface iface;

		/* prepare interface info */
		strncpy(iface.name, i->ifa_name, sizeof(iface.name) - 1);
		iface.name[sizeof(iface.name) - 1] = 0;
		iface.addr = sa->sin_addr.s_addr;

		/* call interface handler */
		iface_cb(&iface, priv);
	}

	freeifaddrs(ifa);

	return (0);
}
