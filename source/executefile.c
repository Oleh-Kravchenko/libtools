#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libtools/closeall.h"
#include "libtools/executefile.h"

/*------------------------------------------------------------------------*/

int executefile(const char *path)
{
	assert(path);

	pid_t pid;

	/* fork before replacing current process */
	if ((pid = fork())) {
		if (-1 == pid) {
			return (-1);
		}

		int status;

		/* wait until child process finish */
		while (-1 == waitpid(pid, &status, 0)) {
			/* ignore POSIX signals */
			if (EINTR != errno) {
				return (-1);
			}
		}

		/* return status code */
		return (WEXITSTATUS(status));
	}

	/* close all file descriptors to avoid fd leaks */
	if (closeall(1)) {
		exit(EXIT_FAILURE);
	}

	/* execute requested program */
	execl(path, path, NULL);

	/* execl() don't return, if successful */
	exit(EXIT_FAILURE);
}
