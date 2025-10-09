#include <stdlib.h>
#include "blockchain.h"

/**
 * block_destroy - frees memory allocated for a block
 * @block: pointer to the block to delete
 */
void block_destroy(block_t *block)
{
	if (block)
		free(block);
}
