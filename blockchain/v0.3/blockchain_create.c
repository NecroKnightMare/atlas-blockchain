#include <stdlib.h>
#include <string.h>
#include "blockchain.h"


/**
 * blockchain_create - Creates a new blockchain and inits the genesis block
 * Return: pointer to the new blockchain or NULL if failed
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis;

	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}
	genesis = genesis_block();
	if (!genesis)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	if (llist_add_node(blockchain->chain, genesis) == -1)
	{
		block_destroy(genesis);
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
