/**
 * @file
 *
 * Copyright (C) 2016  Oleh Kravchenko <oleg@kaa.org.ua>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libtools/closeall.h"
#include "libtools/popen2.h"

/*------------------------------------------------------------------------*/

/** pipefd[0] refers to the read end of the pipe */
#define PIPE_READ 0

/** pipefd[1] refers to the write end of the pipe */
#define PIPE_WRITE 1

/*------------------------------------------------------------------------*/

/**
 * @brief Prepare child process standard file descriptors
 * @param [in] path child process executable filename
 * @param [in] infd file descriptor for standard input
 * @param [in] outfd file descriptor for standard output
 * @param [in] errfd file descriptor for error output
 */
static void child(const char *path, int infd, int outfd, int errfd)
{
	assert(path);
	assert(infd != -1);
	assert(outfd != -1);
	assert(errfd != -1);

	do {
		if (dup2(infd, STDIN_FILENO) == -1) {
			break;
		}

		if (dup2(outfd, STDOUT_FILENO) == -1) {
			break;
		}

		if (dup2(errfd, STDERR_FILENO) == -1) {
			break;
		}

		/* close opened fd's, except standards */
		if (closeall(1)) {
			break;
		}

		/* execl() don't return, if successful */
		execl(path, path, NULL);
	} while (0);

	/* report about failure to parent process */
	exit(EXIT_FAILURE);
}

/*------------------------------------------------------------------------*/

/**
 * @brief Run process and open standard streams
 * @param [in] path executable filename
 * @param [out] in pointer to stdin stream for child process
 * @param [out] out pointer to stdout stream for child process
 * @param [out] err pointer to stderr stream for child process
 * @return pid_t
 * @retval -1 error occurred
 */
pid_t popen2(const char *path, FILE **in, FILE **out, FILE **err)
{
	assert(path);
	assert(in);
	assert(out);
	assert(err);

	pid_t res = -1;

	/* create pipes */
	int infd[2], outfd[2], errfd[2];

	if (pipe(infd)) {
		return (res);
	}

	if (pipe(outfd)) {
		close(infd[0]);
		close(infd[1]);

		return (res);
	}

	if (pipe(errfd)) {
		close(infd[0]);
		close(infd[1]);

		close(outfd[0]);
		close(outfd[1]);

		return (res);
	}

	do {
		/* associate with streams */
		*in = fdopen(infd[PIPE_WRITE], "w");
		*out = fdopen(outfd[PIPE_READ], "r");
		*err = fdopen(errfd[PIPE_READ], "r");

		/* check, if errors */
		if (!*in || !*out || !*err) {
			if (*in) {
				fclose(*in);
			} else {
				close(infd[PIPE_WRITE]);
			}

			if (*out) {
				fclose(*out);
			} else {
				close(outfd[PIPE_READ]);
			}

			if (*err) {
				fclose(*err);
			} else {
				close(errfd[PIPE_READ]);
			}

			break;
		}

		res = fork();

		if (-1 == res) {
			fclose(*in);
			fclose(*out);
			fclose(*err);

			break;
		} else if (!res) {
			child(path,
				infd[PIPE_READ],
				outfd[PIPE_WRITE],
				errfd[PIPE_WRITE]
			);
		}
	} while (0);

	/* closing other fd's */
	close(infd[PIPE_READ]);
	close(outfd[PIPE_WRITE]);
	close(errfd[PIPE_WRITE]);

	return (res);
}
