#include <openssl/sha.h>
#include "hblk_crypto.h"

/**
 * sha256 - Computes the SHA-256 hash of a given input
 * @s: pointer to input data
 * @len: pointer at length of input data
 * @digest: pointer to buffer where to store the hash
 * Return: pointer to the hash (digest), NULL if NULL
 */
uint8_t *sha256(int8_t const *s, size_t len,
	uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest || !s)
		return (NULL);

	SHA256((unsigned char const *)s, len, digest);
	return (digest);
}
