#include <sys/stat.h>
#include <unistd.h>

#include "libtools/closeall.h"

/*------------------------------------------------------------------------*/

int closeall(int nostdclose)
{
	/* retrieve maximum fd number */
	int fds = getdtablesize();

	if (-1 == fds) {
		return (-1);
	}

	/* close all fds, except standard (in, out and err) streams */
	for (int fd = nostdclose ? 3 : 0; fd < fds; ++ fd) {
		if (fstat(fd, &(struct stat){0})) {
			/* fd not used */
			continue;
		}

		if (close(fd)) {
			return (-1);
		}
	}

	return (0);
}
