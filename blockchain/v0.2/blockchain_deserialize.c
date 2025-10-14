#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"
#include "provided/endianness.h"


#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.1"

/**
 * blockchain_deserialize - loads a blockchain from a file
 * @path: path to the file to read
 *
 * Return: pointer to the loaded blockchain, or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fp;
	blockchain_t *blockchain;
	block_t *block;
	uint8_t magic[4], version[3], endian_byte;
	uint32_t nb_blocks, i, data_len;
	uint8_t endian_file, endian_host;
	size_t read;

	if (!path)
		return (NULL);

	fp = fopen(path, "rb");
	if (!fp)
		return (NULL);

	read = fread(magic, 1, 4, fp);
	if (read != 4 || memcmp(magic, HBLK_MAGIC, 4) != 0)
		return (fclose(fp), NULL);

	read = fread(version, 1, 3, fp);
	if (read != 3 || memcmp(version, HBLK_VERSION, 3) != 0)
		return (fclose(fp), NULL);

	if (fread(&endian_byte, 1, 1, fp) != 1)
		return (fclose(fp), NULL);

	endian_file = endian_byte;
	endian_host = _get_endianness();

	if (fread(&nb_blocks, sizeof(nb_blocks), 1, fp) != 1)
		return (fclose(fp), NULL);

	if (endian_file != endian_host)
		nb_blocks = __builtin_bswap32(nb_blocks);

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (fclose(fp), NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
		return (fclose(fp), free(blockchain), NULL);

	for (i = 0; i < nb_blocks; i++)
	{
		block = calloc(1, sizeof(*block));
		if (!block)
			return (fclose(fp), blockchain_destroy(blockchain), NULL);

		if (fread(&block->info, sizeof(block->info), 1, fp) != 1)
			return (fclose(fp), block_destroy(block), blockchain_destroy(blockchain), NULL);

		if (endian_file != endian_host)
		{
			block->info.index = __builtin_bswap32(block->info.index);
			block->info.difficulty = __builtin_bswap32(block->info.difficulty);
			block->info.timestamp = __builtin_bswap64(block->info.timestamp);
			block->info.nonce = __builtin_bswap64(block->info.nonce);
		}

		if (fread(&data_len, sizeof(data_len), 1, fp) != 1)
			return (fclose(fp), block_destroy(block), blockchain_destroy(blockchain), NULL);

		if (endian_file != endian_host)
			data_len = __builtin_bswap32(data_len);

		block->data.len = data_len;

		if (fread(block->data.buffer, 1, data_len, fp) != data_len)
			return (fclose(fp), block_destroy(block), blockchain_destroy(blockchain), NULL);

		if (fread(block->hash, 1, SHA256_DIGEST_LENGTH, fp) != SHA256_DIGEST_LENGTH)
			return (fclose(fp), block_destroy(block), blockchain_destroy(blockchain), NULL);

		if (llist_add_node(blockchain->chain, block, ADD_NODE_REAR) == -1)
			return (fclose(fp), block_destroy(block), blockchain_destroy(blockchain), NULL);
	}

	fclose(fp);
	return (blockchain);
}
