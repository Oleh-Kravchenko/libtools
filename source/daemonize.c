#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libtools/closeall.h"
#include "libtools/daemonize.h"
#include "libtools/flock.h"

/*------------------------------------------------------------------------*/

pid_t daemonize(const char *path, char *const argv[], int nochdir, int noclose)
{
	assert(path);

	pid_t pid;
	int fds[2];

	/* create pipe to retrive pid of daemon */
	if (pipe(fds)) {
		return (-1);
	}

	/* first fork */
	if ((pid = fork())) {
		/* close pipe for writting now to avoid deadlock, if child failed */
		close(fds[1]);

		if (-1 == pid) {
			/* can't fork, return error */
			goto done;
		}

		int status;
		pid_t rc;

		/* wait until child process finish with daemon startup */
		while (-1 == (rc = waitpid(pid, &status, 0))) {
			/* ignore POSIX signals */
			if (EINTR != errno) {
				pid = -1;

				goto done;
			}
		}

		/* daemon startup failed */
		if (EXIT_FAILURE == WEXITSTATUS(status)) {
			pid = -1;

			goto done;
		}

		/* read pid of daemon from pipe */
		if (sizeof(pid) != read(fds[0], &pid, sizeof(pid))) {
			pid = -1;
		}

done:
		close(fds[0]);

		return (pid);
	}

	/* second fork for daemon startup */
	if ((pid = fork())) {
		if (-1 == pid) {
			/* fail to fork, report about it */
			exit(EXIT_FAILURE);
		}

		/* report about successful attempt running of daemon */
		exit(EXIT_SUCCESS);
	}

	/* now detach to init process */
	if (-1 == setsid()) {
		exit(EXIT_FAILURE);
	}

	/* now we should report our pid... */
	pid = getpid();

	/* to avoid duplicates of daemon */
	if (sizeof(pid) != write(fds[1], &pid, sizeof(pid))) {
		exit(EXIT_FAILURE);
	}

	/* now we should close all fds */
	if (closeall(noclose)) {
		exit(EXIT_FAILURE);
	}

	/* and change current directory to "/" */
	if (!nochdir && chdir("/")) {
		exit(EXIT_FAILURE);
	}

	/* execute requested program */
	execv(path, argv);

	/* execl() don't return, if successful */
	exit(EXIT_FAILURE);
}

/*------------------------------------------------------------------------*/

int pidlock_set(const char *path)
{
	assert(path);

	/* try open and lock file */
	FILE *f;

	while (!fltryopen(path, "r+", &f) && ENOENT == errno) {
		/* file don't exist, try create it by appending */
		if (!flopen(path, "a", &f)) {
			/* failed to create file */
			break;
		}

		flclose(f);
	}

	/* can't create pid file */
	if (!f) {
		return (-1);
	}


	/* read pid from file */
	pid_t pid;

	if (fscanf(f, "%16d\n", &pid) != 1) {
		pid = -1;
	}


	/* pidlock is already owned by current process */
	if (getpid() == pid) {
		flclose(f);

		return (0);
	}


	/* verify process by sending signal */
	if (pid > 0 && !kill(pid, 0)) {
		flclose(f);

		/* pidlock already owned by another process */
		return (-1);
	}


	/* save our pid to file */
	rewind(f);

	if (fprintf(f, "%d\n", getpid()) <= 0) {
		flclose(f);

		return (-1);
	}

	/* truncate file, because previous string can be bigger than current */
	if (ftruncate(fileno(f), ftello(f))) {
		flclose(f);

		return (-1);
	}

	return (flclose(f));
}

/*------------------------------------------------------------------------*/

pid_t pidlock_get(const char *path)
{
	assert(path);

	/* open and lock file */
	FILE *f;

	if (!flopen(path, "r", &f)) {
		return (ENOENT == errno ? 0 : -1);
	}


	/* read pid from file or leave pid in error state */
	pid_t pid;

	if (fscanf(f, "%16d\n", &pid) != 1) {
		pid = -1;
	}

	flclose(f);


	/* verify process by pid */
	if (pid > 0 && kill(pid, 0)) {
		/* process don't exist */
		return (0);
	}

	return (pid);
}
