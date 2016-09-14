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

#ifndef __LIBTOOLS_IFACE_H
#define __LIBTOOLS_IFACE_H

#include <net/if.h>
#include <netinet/in.h>

/**
 * @addtogroup network
 *
 * @{
 */

/** expression is true, if address from loopback networks */
#define IN_LOOPBACK(a) ((((in_addr_t)(a)) & 0x7f000000) == 0x7f000000)

/** network interface */
struct iface {
	/** network interface name */
	char name[IFNAMSIZ];

	/** IPv4 address */
	in_addr_t addr;
};

/**
 * @brief list IPv4 network interfaces
 * @param [in] iface_cb callback handler for interface
 * @param [in] priv pointer to private data for callback routine
 * @return on success, zero is returned
 * @retval -1 error occurred
 *
 * If error occurred function update #errno - see errors specified for
 * the routine getifaddrs().
 */
int iface_list(void (*iface_cb)(struct iface*, void*), void *priv);

/** @} */

#endif /* __LIBTOOLS_IFACE_H */
