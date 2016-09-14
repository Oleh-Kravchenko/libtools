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

#ifndef __TOOLS_MAILBOX_H
#define __TOOLS_MAILBOX_H

/**
 * @addtogroup tools
 *
 * @{
 */

/** mailbox handle */
typedef struct mailbox *mailbox_t;

/**
 * @brief allocate new mailbox
 * @param [out] mb pointer to new mailbox
 * @param [in] len length of mailbox
 * @return pointer to new mailbox
 * @retval NULL error occurred
 *
 * Function update #errno if error occurred.
 */
mailbox_t mailbox_alloc(mailbox_t *mb, unsigned len)
	__attribute__((warn_unused_result));

/**
 * @brief destroy mailbox
 * @param [in] mb pointer to mailbox
 *
 * If @p mailbox is NULL, no operation is performed.
 */
void mailbox_destroy(mailbox_t mb);

/**
 * @brief post message into mailbox
 * @param [in] mb pointer to mailbox
 * @param [in] data message
 * @return on success, zero is returned
 * @retval -1 error occured
 *
 * If the mailbox is full, the function will wait for the availability of space.
 */
int mailbox_post(mailbox_t mb, void *data);
/**
 * @brief retrive message from mailbox
 * @param [in] mb pointer to mailbox
 * @param [out] data message
 * @return on success, zero is returned
 * @retval -1 error occured
 *
 * If the mailbox is empty, the function will wait for message.
 */
int mailbox_wait(mailbox_t mb, void **data);

/**
 * @brief retrive message from mailbox with or wait for message
 * @param [in] mb pointer to mailbox
 * @param [out] data message
 * @param [in] timeout timeout for message
 * @return on success, zero is returned
 * @retval -1 error occured
 *
 * If the mailbox is empty, the function will wait for message until timeout
 * expired.
 */
int mailbox_timedwait(mailbox_t mb, void **data, unsigned timeout);

/** @} */

#endif /* __TOOLS_MAILBOX_H */
