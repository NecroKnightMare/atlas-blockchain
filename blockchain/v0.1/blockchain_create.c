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
	memcpy(genesis, &_genesis, sizeof(block_t));
	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_REAR) == -1)
	{
		free(genesis);
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
