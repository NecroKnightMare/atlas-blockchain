#include <stdlib.h>
#include <string.h>
#include "transaction.h"

#define COINBASE_AMOUNT 50

/**
 * coinbase_create - creates a coinbase transaction
 * @receiver: public key of the miner
 * @block_index: index of the block this transaction belongs to
 *
 * Return: pointer to created transaction, or NULL on failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;
	llist_t *inputs, *outputs;
	uint8_t pub[EC_PUB_LEN];
	if (!receiver)
		return (NULL);

	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
		return (NULL);
	inputs = llist_create(MT_SUPPORT_FALSE);
	outputs = llist_create(MT_SUPPORT_FALSE);
	if (!inputs || !outputs)
    {
		free(tx);
		llist_destroy(inputs, 1, free);
		llist_destroy(outputs, 1, free);
		return (NULL);
    }

	in = calloc(1, sizeof(tx_in_t));
	if (!in)
	{
		free(tx);
		llist_destroy(inputs, 1, free);
		llist_destroy(outputs, 1, free);
		return (NULL);
	}
	memcpy(in->tx_out_hash, &block_index, sizeof(block_index));
	llist_add_node(inputs, in, ADD_NODE_REAR);

	ec_to_pub(receiver, pub);
	out = tx_out_create(COINBASE_AMOUNT, pub);
	if (!out)
	{
		transaction_destroy(tx);
		return (NULL);
	}
	llist_add_node(outputs, out, ADD_NODE_REAR);

	tx->inputs = inputs;
	tx->outputs = outputs;
	transaction_hash(tx, tx->id);

	return (tx);
}
