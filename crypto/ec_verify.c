#include <openssl/ecdsa.h>
#include <string.h>
#include "hblk_crypto.h"

/**
 * ec_verify - verifies the signature of a given set of bytes
 * @key: pointer to EC KEY struct with public key
 * @msg: pointer to message bytes
 * @msglen: length of message
 * @sig: pointer to struct with signature
 * Return: 1 if valid, otherwise 0
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg,
	size_t msglen, sig_t const *sig)
{
	if (!key || !msg || !sig || !sig->sig || sig->len == 0)
		return (0);

	return (ECDSA_verify(0, msg, msglen, sig->sig, sig->len,
		(EC_KEY *)key));
}
