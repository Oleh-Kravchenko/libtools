#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "tools/dump.h"

/*------------------------------------------------------------------------*/

/** length of column for cdump8() */
#define __CDUMP8_ALIGN 12

/** length of column for hexdump8() */
#define __HEXDUMP8_ALIGN 16

/** length of group for hexdump8() */
#define __HEXDUMP8_GROUP 4

/*------------------------------------------------------------------------*/

void strdump(const void *buf, size_t len)
{
	assert(buf);

	const uint8_t *str = buf;

	if (!len) {
		return;
	}

	while (len --) {
		putchar(isprint(*str) ? *str : '.');

		++ str;
	}
}

/*------------------------------------------------------------------------*/

void hexdump8(const void *buf, size_t len)
{
	assert(buf);

	const uint8_t *data = buf;
	size_t i, spaces, tail;

	if (!len) {
		return;
	}

	for (i = 0; i < len; ++ i) {
		/* print data like a string */
		if (i % __HEXDUMP8_ALIGN == 0) {
			if (i) {
				printf(" | ");
				strdump(data - __HEXDUMP8_ALIGN, __HEXDUMP8_ALIGN);
				putchar('\n');
			}

			printf("%p |", data);
		}

		/* group by __HEXDUMP8_GROUP bytes */
		if (i % __HEXDUMP8_GROUP == 0) {
			putchar(' ');
		}

		printf("%02x", *data ++);
	}

	/* calculate size of align */
	spaces = len;
	tail = len % __HEXDUMP8_ALIGN;

	if (tail) {
		spaces += __HEXDUMP8_ALIGN - tail;
	} else {
		tail = __HEXDUMP8_ALIGN;
	}

	/* print spaces to align tail */
	while (i < spaces) {
		/* group by __HEXDUMP8_GROUP byte */
		if(i % __HEXDUMP8_GROUP == 0) {
			putchar(' ');
		}

		++ i;

		printf("  ");
	}

	/* print data tail */
	printf(" | ");
	strdump(data - tail, tail);
	putchar('\n');
}

/*------------------------------------------------------------------------*/

void cdump8(const char *name, const void *buf, size_t len)
{
	assert(name);
	assert(buf);

	const uint8_t *data = buf;
	size_t i;

	if (!len) {
		return;
	}

	printf("uint8_t %s[%zu] = {\n\t0x%02x", name, len, *data ++);

	for (i = 1; i < len; ++ i) {
		printf((i % __CDUMP8_ALIGN) ? ", " : ",\n\t");

		printf("0x%02x", *data ++);
	}

	printf("\n};\n");
}
