#include <stdint.h>
#include <stdio.h>

#include "tools/sha1.h"

/*------------------------------------------------------------------------*/

int sha1_file(const char *path, sha1_t *hash)
{
	uint8_t buf[4 << 10];
	size_t buf_len;
	int ret = -1;
	SHA_CTX ctx;
	FILE* f;

	if(!SHA1_Init(&ctx)) {
		return(ret);
	}

	if(!(f = fopen(path, "rb"))) {
		return(ret);
	}

	while(!feof(f)) {
		if((buf_len = fread(buf, 1, sizeof(buf), f))) {
			if(!SHA1_Update(&ctx, buf, buf_len)) {
				goto done;
			}
		}
	}

	if(!SHA1_Final((unsigned char*)hash, &ctx)) {
		goto done;
	}

	/* successfully */
	ret = 0;

done:
	fclose(f);

	return(ret);
}
