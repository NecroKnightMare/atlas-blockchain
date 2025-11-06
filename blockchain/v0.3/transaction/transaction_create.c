#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * transaction_destroy - frees a transaction
 * @tx: the transaction to free
 */
void transaction_destroy(transaction_t *tx)
{
	if (!tx)
		return;
	llist_destroy(tx->inputs, 1, free);
	llist_destroy(tx->outputs, 1, free);
	free(tx);
}

/**
 * match_sender_unspent - helper to match unspent outputs owned by sender
 */
static int match_sender_unspent(void *node, void *key)
{
	unspent_tx_out_t *unspent = node;
	uint8_t pub[EC_PUB_LEN];

	if (!ec_to_pub((EC_KEY *)key, pub))
		return (0);

	return (memcmp(unspent->out.pub, pub, EC_PUB_LEN) == 0);
}

/**
 * transaction_create - creates a transaction
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
                                  uint32_t amount, llist_t *all_unspent)
{
	transaction_t *tx;
	llist_t *inputs, *outputs;
	uint8_t sender_pub[EC_PUB_LEN], receiver_pub[EC_PUB_LEN];
	uint32_t total = 0;
	size_t i, count;
	unspent_tx_out_t *unspent;
	tx_in_t *in;
	tx_out_t *out;
	if (!sender || !receiver || !all_unspent)
		return (NULL);
	inputs = llist_create(MT_SUPPORT_FALSE);
	if (!inputs)
		return (NULL);

	count = llist_size(all_unspent);
for (i = 0; i < count; i++)
	{
		unspent = llist_get_node_at(all_unspent, i);
		if (!match_sender_unspent(unspent, (void *)sender))
			continue;
		in = tx_in_create(unspent);
		if (!in || llist_add_node(inputs, in, ADD_NODE_REAR) == -1)
		{
			free(in);
			llist_destroy(inputs, 1, free);
			return (NULL);
		}

		total += unspent->out.amount;
		if (total >= amount)
			break;
	}

	if (total < amount)
	{
		llist_destroy(inputs, 1, free);
		return (NULL);
	}
	outputs = llist_create(MT_SUPPORT_FALSE);
	if (!outputs)
	{
		llist_destroy(inputs, 1, free);
		return (NULL);
	}
	ec_to_pub(receiver, receiver_pub);
	out = tx_out_create(amount, receiver_pub);
	if (!out || llist_add_node(outputs, out, ADD_NODE_REAR) == -1)
    {
		free(out);
		llist_destroy(inputs, 1, free);
		llist_destroy(outputs, 1, free);
		return (NULL);
	}
	if (total > amount)
	{
		ec_to_pub(sender, sender_pub);
		out = tx_out_create(total - amount, sender_pub);
		if (!out || llist_add_node(outputs, out, ADD_NODE_REAR) == -1)
		{
			free(out);
			llist_destroy(inputs, 1, free);
			llist_destroy(outputs, 1, free);
			return (NULL);
		}
	}
	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
	{
		llist_destroy(inputs, 1, free);
		llist_destroy(outputs, 1, free);
	return (NULL);
	}

	tx->inputs = inputs;
	tx->outputs = outputs;
	transaction_hash(tx, tx->id);

	for (size_t i = 0; i < (size_t)llist_size(inputs); i++)
	{
		in = llist_get_node_at(inputs, i);
		if (!tx_in_sign(in, tx->id, sender, all_unspent))
		{
			transaction_destroy(tx);
			return (NULL);
		}
	}

	return (tx);
}
