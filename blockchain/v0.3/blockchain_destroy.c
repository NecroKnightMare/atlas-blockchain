#include <stdlib.h>
#include "blockchain.h"

/**
 * blockchain_destroy - frees a blockchain and all its blocks
 * @blockchain: pointer to the blockchain to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
	llist_destroy(blockchain->unspent, 1, (node_dtor_t)free);
	free(blockchain);
}
