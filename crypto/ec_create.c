#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include "hblk_crypto.h"

/**
 * ec_create - creates new ec pair using secp256k1 curve
 * Return: pointer to the new EC_KEY pair, NULL if failed
 */
EC_KEY *ec_create(void)
{
	EC_KEY *key = NULL;

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	if (!EC_KEY_generate_key(key))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	return (key);
}
