#include <assert.h>
#include <stdint.h>

#include "libtools/hex.h"
#include "libtools/tools.h"

/*------------------------------------------------------------------------*/

size_t bin2hex(const void *data, size_t len, char *hex, size_t hex_len)
{
	assert(data);
	assert(hex);

	uint8_t *b, h, l;

	b = (uint8_t*)data;
	len = min(len, (hex_len - 1) / 2);

	for (size_t i = 0; i < len; ++i) {
		h = b[i] >> 4;
		l = b[i] & 0x0f;

		*hex++ = (h < 0xa ? '0' : 'a' - 0xa) + h;
		*hex++ = (l < 0xa ? '0' : 'a' - 0xa) + l;
	}

	if (len) {
		*hex = '\0';
	}

	return (len);
}

/*------------------------------------------------------------------------*/

int hex2int(char ch)
{
	int d = -1;

	if ('0' <= ch && ch <= '9') {
		d = ch - '0';
	} else if ('a' <= ch && ch <= 'f') {
		d = ch - 'a';
	} else if ('A' <= ch && ch <= 'F') {
		d = ch - 'A';
	}

	return (d);
}

/*------------------------------------------------------------------------*/

size_t hex2bin(const char *hex, void *data, size_t data_len)
{
	assert(hex);
	assert(data);

	uint8_t	h, l;
	uint8_t *b = (uint8_t*)data;
	size_t	ret = 0;

	while (*hex && ret < data_len) {
		h = hex2int(*hex++);

		if (!*hex) {
			break;
		}

		l = hex2int(*hex++);

		*b++ = (h << 4) | l;
		++ret;
	}

	return (ret);
}
