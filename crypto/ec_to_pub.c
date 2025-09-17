#include "hblk_crypto.h"

/**
 * ec_to_pub - converts an EC_KEY private key to a public key
 * @key: pointer to the EC_KEY structure
 * @pub: address to store the public key
 * Return: pointer to the public key on success, NULL if fail
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *pub_key;
	const EC_GROUP *group;

	if (!key || !pub)
		return (NULL);
	pub_key = EC_KEY_get0_public_key(key);
	group = EC_KEY_get0_group(key);

	if (!pub_key || !group)
		return (NULL);

	if (!EC_POINT_point2oct(group, pub_key,
		POINT_CONVERSION_UNCOMPRESSED, pub, EC_PUB_LEN, NULL))
		return (NULL);
	return (pub);
}
