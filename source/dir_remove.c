#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libtools/dir_remove.h"

/*------------------------------------------------------------------------*/

int dir_remove(const char *path)
{
	assert(path);

	struct stat st;

	if (stat(path, &st)) {
		return (-1);
	}

	if (!S_ISDIR(st.st_mode)) {
		return (unlink(path));
	}

	DIR *dir;

	if (!(dir = opendir(path))) {
		return (-1);
	}

	struct dirent *saveptr;
	struct dirent ent;
	char *buf;

	int ret = 0;

	/* Please see details here: https://lwn.net/Articles/696474/ */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	while (!readdir_r(dir, &ent, &saveptr) && saveptr) {
	#pragma GCC diagnostic pop

		if (!strcmp(ent.d_name, ".") || !strcmp(ent.d_name, "..")) {
			continue;
		}

		if (-1 == asprintf(&buf, "%s/%s", path, ent.d_name)) {
			errno = ENOMEM;
			ret = -1;

			break;
		}

		if (dir_remove(buf)) {
			ret = -1;
		}

		free(buf);
	}

	closedir(dir);

	if (rmdir(path)) {
		ret = -1;
	}

	return (ret);
}
