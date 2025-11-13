#include "blockchain.h"
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
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
	EC_KEY *pub_key;
	int i, j;
	int input_count, output_count, unspent_count;
	uint32_t total_in = 0, total_out = 0;

	if (!transaction || !all_unspent)
		return (0);

	transaction_hash(transaction, hash_buf);
	if (memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	input_count = llist_size(transaction->inputs);
	output_count = llist_size(transaction->outputs);
	unspent_count = llist_size(all_unspent);

	for (i = 0; i < input_count; i++)
	{
		tx_in = llist_get_node_at(transaction->inputs, i);
		if (!tx_in)
			return (0);

		unspent = NULL;
		for (j = 0; j < unspent_count; j++)
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
		pub_key = ec_from_pub(unspent->out.pub);
		if (!pub_key || !ec_verify(pub_key, transaction->id, SHA256_DIGEST_LENGTH, &tx_in->sig))
		{
			if (pub_key)
				EC_KEY_free(pub_key);
			return (0);
		}
		EC_KEY_free(pub_key);

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
