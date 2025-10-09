#include <string.h>
#include "blockchain.h"

/**
 * block_hash - computes the SHA-256 hash of a block
 * @block: pointer to the block to hash
 * @hash_buf: buffer to store the hash result
 *
 * Return: pointer to hash_buf
 */
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX ctx;
	block_info_t temp_info;

	if (!block || !hash_buf)
		return (NULL);
		
	memcpy(&temp_info, &block->info, sizeof(temp_info));
	temp_info.timestamp = 1537577995;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &block->info, sizeof(block->info));
	SHA256_Update(&ctx, &block->data, sizeof(block->data));
	SHA256_Final(hash_buf, &ctx);

	return (hash_buf);
}
