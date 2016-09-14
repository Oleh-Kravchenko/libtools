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

#ifndef __TOOLS_DAEMONIZE_H
#define __TOOLS_DAEMONIZE_H

#include <unistd.h>

/**
 * @addtogroup io
 *
 * @{
 */

/**
 * @brief run program as daemon
 * @param [in] path path to program file
 * @param [in] argv arguments for program (last item should be NULL)
 * @param [in] nochdir don't change current directory to '/' for daemon
 * @param [in] noclose don't close stdio streams for daemon
 * @return pid of started daemon
 * @retval -1 error occured
 */
pid_t daemonize(const char *path, char *const argv[], int nochdir, int noclose);

/**
 * @brief try to save pid of calling process to file
 * @param [in] path name of pid file
 * @return on success, zero is returned
 * @retval -1 error occurred
 *
 * Function fail, if another process already set pidlock.
 *
 * If error occurred function update #errno - see errors specified for
 * the routines fltryopen(), flclose(), fscanf(3), fprintf(3), ftruncate(2)
 * and kill(2).
 */
int pidlock_set(const char *path);

/**
 * @brief read pid from file and verify it
 * @param [in] path name of pid file
 * @return pid of process
 * @retval 0 process don't exist
 * @retval -1 error occurred
 *
 * If error occurred function update #errno - see errors specified for
 * the routines flopen(), flclose(), fscanf(3) and kill(2).
 */
pid_t pidlock_get(const char *path);

/** @} */

#endif /* __TOOLS_DAEMONIZE_H */
