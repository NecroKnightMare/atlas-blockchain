#include "blockchain.h"
#include "transaction.h"

/**
 * transaction_is_valid - verifies a transaction’s integrity and authenticity
 * @transaction: pointer to the transaction to verify
 * @all_unspent: list of all unspent transaction outputs
 * Return: 1 if valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction, llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	unspent_tx_out_t *unspent;
	uint32_t i, j;
	int input_count, output_count;
	uint32_t total_in = 0, total_out = 0;

	if (!transaction || !all_unspent)
		return (0);

	/* 1. Verify transaction hash */
	transaction_hash(transaction, hash_buf);
	if (memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	input_count = llist_size(transaction->inputs);
	output_count = llist_size(transaction->outputs);

	/* 2. Validate each input */
	for (i = 0; i < input_count; i++)
	{
		tx_in = llist_get_node_at(transaction->inputs, i);
		if (!tx_in)
			return (0);

		unspent = NULL;
		for (j = 0; j < llist_size(all_unspent); j++)
		{
			unspent_tx_out_t *u = llist_get_node_at(all_unspent, j);
			if (!u)
				continue;

			if (memcmp(tx_in->block_hash, u->block_hash, SHA256_DIGEST_LENGTH) == 0 &&
				memcmp(tx_in->tx_id, u->tx_id, SHA256_DIGEST_LENGTH) == 0 &&
				memcmp(tx_in->tx_out_hash, u->out.hash, SHA256_DIGEST_LENGTH) == 0)
			{
				unspent = u;
				break;
			}
		}

		if (!unspent)
			return (0);

		if (!ec_verify(unspent->out.pub, tx_in->sig.sig, tx_in->sig.len, transaction->id))
			return (0);

		total_in += unspent->out.amount;
	}

	for (i = 0; i < output_count; i++)
	{
		tx_out = llist_get_node_at(transaction->outputs, i);
		if (!tx_out)
			return (0);
		total_out += tx_out->amount;
	}

	if (total_in != total_out)
		return (0);

	return (1);
}
