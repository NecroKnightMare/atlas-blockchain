#include <openssl/ecdsa.h>
#include <stdlib.h>
#include <string.h>
#include "hblk_crypto.h"

/**
 * ec_sign - sign a message using ECDSA
 * @key: EC key to sign with
 * @msg: Message to sign
 * @msglen: Length of the message
 * @sig: Pointer to the signature structure
 *
 * Return: 1 on success, 0 on failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
size_t msglen, sig_t *sig)
{

	unsigned int sig_len;

	if (!key || !msg || !sig)
		return (NULL);

	sig->sig = NULL;
	sig->len = 0;

	sig_len = ECDSA_size((EC_KEY *)key);
	sig->sig = malloc(sig_len);

	if (!sig->sig)
		return (NULL);

	if (!ECDSA_sign(0, msg, msglen, sig->sig, &sig_len, (EC_KEY *)key))
	{
		free(sig->sig);
		sig->sig = NULL;
		return (NULL);
	}
	sig->len = sig_len;
	/*return (sig->sig);*/
	return ec_sign(key, msg, msglen, sig);
}
