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

#ifndef __LIBTOOLS_SHA1_H
#define __LIBTOOLS_SHA1_H

#include <openssl/sha.h>

/**
 * @addtogroup tools
 *
 * @{
 */

/** length of SHA1 string */
#define SHA1_STRING_LENGTH (SHA_DIGEST_LENGTH * 2 + 1)

/** variable type for storing SHA1 hash */
typedef uint8_t sha1_t[SHA_DIGEST_LENGTH];

/**
 * @brief calculate SHA1 hash for specified file
 * @param [in] path filename
 * @param [out] hash SHA1 hash sum
 * @return on success, zero is returned
 * @retval -1 error occurred
 */
int sha1_file(const char *path, sha1_t *hash);

/** @} */

#endif /* __LIBTOOLS_SHA1_H */
