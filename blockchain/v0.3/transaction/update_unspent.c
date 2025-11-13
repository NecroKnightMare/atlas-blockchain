#include <stdlib.h>
#include <string.h>
#include "blockchain.h"
#include "transaction.h"

/**
 * update_unspent - Updates the list of unspent transaction outputs
 * @transactions: list of validated transactions
 * @block_hash: hash of the block containing these transactions
 * @all_unspent: current list of unspent transaction outputs
 * Return: new list of unspent transaction outputs, or NULL on failure
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent)
{
	llist_t *new_unspent;
	transaction_t *tx;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	unspent_tx_out_t *unspent;
	int i, j, unspent_count;

	if (!transactions || !block_hash || !all_unspent)
		return (NULL);

	new_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!new_unspent)
		return (NULL);

	unspent_count = llist_size(all_unspent);

	for (i = 0; i < unspent_count; i++)
	{
		unspent = llist_get_node_at(all_unspent, i);
		if (!unspent)
			continue;

		int spent = 0;
		for (j = 0; j < llist_size(transactions); j++)
		{
			tx = llist_get_node_at(transactions, j);
			if (!tx)
				continue;

			for (int k = 0; k < llist_size(tx->inputs); k++)
			{
				tx_in = llist_get_node_at(tx->inputs, k);
				if (!tx_in)
					continue;

				if (memcmp(tx_in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH) == 0 &&
					tx_in->out_index == unspent->out.index)
				{
					spent = 1;
					break;
				}
			}
			if (spent)
				break;
		}

		if (!spent)
			llist_add_node(new_unspent, unspent, ADD_NODE_REAR);
		else
			free(unspent);
	}

	for (i = 0; i < llist_size(transactions); i++)
	{
		tx = llist_get_node_at(transactions, i);
		if (!tx)
			continue;

		for (j = 0; j < llist_size(tx->outputs); j++)
		{
			tx_out = llist_get_node_at(tx->outputs, j);
			if (!tx_out)
				continue;

			unspent = unspent_tx_out_create(block_hash, tx->id, tx_out);
			if (!unspent)
			{
				llist_destroy(new_unspent, 1, free);
				return (NULL);
			}

			llist_add_node(new_unspent, unspent, ADD_NODE_REAR);
		}
	}

	llist_destroy(all_unspent, 1, free);
	return (new_unspent);
}
