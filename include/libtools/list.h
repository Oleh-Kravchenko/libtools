/**
 * @file
 *
 * Copyright (C) 2015  Oleh Kravchenko <oleg@kaa.org.ua>
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

#ifndef __TOOLS_LIST_H
#define __TOOLS_LIST_H

#include <stddef.h>

/**
 * @defgroup list List macros
 * @ingroup tools
 *
 * Lite list implementation from Linux kernel.
 *
 * @{
 */

/** node of double-linked list */
struct list {
	/** pointer to previous node */
	struct list *prev;

	/** pointer to next node */
	struct list *next;
};

/*------------------------------------------------------------------------*/

/** should be used to static initialization */
#define list_initializer(list) { .prev = list, .next = list, }

/*------------------------------------------------------------------------*/

/** should be used to runtime initialization */
#define list_init(list)                                                   \
	do {                                                              \
		(list)->prev = (list)->next = (list);                     \
	} while (0)

/*------------------------------------------------------------------------*/

/** return pointer to struct of type, which hold field */
#define containerof(ptr, type, field)                                     \
	((type*)((size_t)(ptr) - offsetof(type, field)))

/*------------------------------------------------------------------------*/

/** iterate over all list nodes */
#define list_foreach(list, node, type, field)                             \
	for((node) = containerof((list)->next, type, field);              \
		&(node)->field != (list);                                 \
		(node) = containerof((node)->field.next, type, field))

/*------------------------------------------------------------------------*/

/**
 * @def list_foreach_safe
 * @brief iterate over all list nodes, also allow node deletion
 * @param [in] l pointer to struct list
 * @param [out] i iterator of list
 * @param [out] t temporary pointer for safe list manipulation
 * @param [in] type type of list
 * @param [in] f name of struct list field in @p type
 */
#define list_foreach_safe(l, i, t, type, f)                               \
	for (                                                             \
		(i) = containerof((l)->next, type, f),                    \
		(t) = containerof((i)->f.next, type, f);                  \
		&(i)->f != (l);                                           \
		i = (t), (t) = containerof((t)->f.next, type, f)          \
	)

/*------------------------------------------------------------------------*/

/** add node to head of list */
#define list_add_head(list, node)                                         \
	do {                                                              \
		(node)->next = (list)->next;                              \
		(node)->prev = (list);                                    \
		(list)->next->prev = (node);                              \
		(list)->next = (node);                                    \
	} while(0);                                                       \

/*------------------------------------------------------------------------*/

/** add node to tail of list */
#define list_add_tail(list, node)                                         \
	do {                                                              \
		(node)->prev = (list)->prev;                              \
		(node)->next = (list);                                    \
		(list)->prev->next = (node);                              \
		(list)->prev = (node);                                    \
	} while(0);                                                       \

/*------------------------------------------------------------------------*/

/** remove node from list */
#define list_del_node(node)                                               \
	do {                                                              \
		(node)->next->prev = (node)->prev;                        \
		(node)->prev->next = (node)->next;                        \
		(node)->prev = (node)->next = NULL;                       \
	} while(0);

/*------------------------------------------------------------------------*/

/** expression is true, if list is empty */
#define list_is_empty(list)                                               \
	((list)->prev == (list) && (list) == (list)->next)

/** @} */

#endif /* __TOOLS_LIST_H */
