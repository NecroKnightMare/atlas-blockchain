#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

	static const block_t genesis_block = {
		.info = {
			.index = 0,
			.difficulty = 0,
			.timestamp = 1537578000,
			.nonce = 0,
			.prev_hash = {0}
		},
		.data = {
			.buffer = "Holberton School",
			.len = 16
		},
		.hash = {
			0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39, 0x63, 0x5d, 0x8e,
			0xdf, 0x2a, 0x97, 0xd4, 0x8d, 0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17,
			0xf2, 0xb1, 0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03
		}/* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */
	};

/**
 * blockchain_create - Creates a new blockchain and inits the genesis block
 * Return: pointer to the new blockchain or NULL if failed
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis;

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_TRUE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}
	genesis = malloc(sizeof(*genesis));
	if (!genesis)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}
	memcpy(genesis, &genesis_block, sizeof(block_t));
	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_REAR) == -1)
	{
		free(genesis);
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
