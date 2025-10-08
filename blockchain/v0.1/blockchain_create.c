#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

static const block_t genesis_block = {
    .info = {
        .index = GENESIS_INDEX,
        .difficulty = GENESIS_DIFFICULTY,
        .timestamp = GENESIS_TIMESTAMP,
        .nonce = GENESIS_NONCE,
        .prev_hash = GENESIS_PREV_HASH
    },
    .data = {
        .buffer = GENESIS_DATA,
        .len = GENESIS_DATA_LEN
    },
    .hash = GENESIS_HASH
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
