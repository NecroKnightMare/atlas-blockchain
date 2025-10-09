#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"
#include "provided/endianness.h"

#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.1"

/**
 * blockchain_serialize - serializes a blockchain into a file
 * @blockchain: pointer to the blockchain to serialize
 * @path: path to the file to write
 *
 * Return: 0 on success, -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fp;
	uint32_t nb_blocks, endian = _get_endianness();
	block_t *block;
	uint32_t i;

	if (!blockchain || !path)
		return (-1);

	fp = fopen(path, "wb");
	if (!fp)
		return (-1);

	fwrite(HBLK_MAGIC, 1, 4, fp);
	fwrite(HBLK_VERSION, 1, 3, fp);
	fwrite(&endian, 1, 1, fp);

	nb_blocks = (uint32_t)llist_size(blockchain->chain);
	fwrite(&nb_blocks, sizeof(nb_blocks), 1, fp);

	for (i = 0; i < nb_blocks; i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		if (!block)
			break;

		fwrite(&block->info, sizeof(block->info), 1, fp);
		fwrite(&block->data.len, sizeof(block->data.len), 1, fp);
		fwrite(block->data.buffer, 1, block->data.len, fp);
		fwrite(block->hash, 1, SHA256_DIGEST_LENGTH, fp);
	}

	fclose(fp);
	return (0);
}
