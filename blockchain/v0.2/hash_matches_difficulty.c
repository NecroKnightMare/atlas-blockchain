#include <stdint.h>
#include <openssl/sha.h>
#include "blockchain.h"

/**
 * hash_matches_difficulty - Check if a given hash
 * meets specified difficulty level
 * @hash: pointer to the hash to be checked (32 bytes)
 * @difficulty: difficulty level
 * Return: 1 if the hash meets the difficulty, otherwise 0
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
	uint32_t difficulty)
{
	uint32_t i, bits;

	if (!hash || difficulty > SHA256_DIGEST_LENGTH * 8)
		return (0);

	for (i = 0; i < SHA256_DIGEST_LENGTH && difficulty > 0; i++)
	{
		if (difficulty >= 8)
		{
			if (hash[i] != 0)
				return (0);
			difficulty -= 8;
		}
		else
		{
			bits = 8 - difficulty;
			if ((hash[i] >> bits) != 0)
				return (0);
			difficulty = 0;
		}
	}
	return (1);
}
