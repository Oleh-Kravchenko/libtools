#include <ctype.h>
#include <openssl/bn.h>
#include <string.h>

#include "tools/hex.h"

/*------------------------------------------------------------------------*/

char *bin2hex(const void *data, size_t len, char *hex, size_t hex_len)
{
	BIGNUM bnObject;
	char *bnHex;
	int i;

	BN_init(&bnObject);

	if(!BN_bin2bn(data, len, &bnObject)) {
		return(NULL);
	}

	if(!(bnHex = BN_bn2hex(&bnObject))) {
		BN_free(&bnObject);
		return(NULL);
	}

	BN_free(&bnObject);

	/* hash string to lower case */
	for(i = 0; i < strlen(bnHex); ++ i) {
		bnHex[i] = tolower(bnHex[i]);
	}

	strncpy(hex, bnHex, hex_len - 1);
	hex[hex_len - 1] = 0;

	OPENSSL_free(bnHex);

	return(hex);
}

/*------------------------------------------------------------------------*/

size_t hex2bin(const char *hex, void *data, size_t data_len)
{
	BIGNUM* bnObject;
	size_t ret;
	void *out;

	bnObject = NULL;

	if(!BN_hex2bn(&bnObject, hex)) {
		return(0);
	}

	if(!(out = malloc(BN_num_bytes(bnObject)))) {
		BN_free(bnObject);
		return(0);
	}

	ret = BN_bn2bin(bnObject, out);
	BN_free(bnObject);

	ret = ret <= data_len ? ret : data_len;
	memcpy(data, out, ret);
	free(out);

	return(ret);
}
