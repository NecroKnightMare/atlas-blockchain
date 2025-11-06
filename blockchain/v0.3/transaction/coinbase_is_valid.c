#include <string.h>
#include "transaction.h"

#define COINBASE_AMOUNT 50

/**
 * coinbase_is_valid - validates a coinbase transaction
 * @coinbase: pointer to the coinbase transaction
 * @block_index: index of the block this transaction belongs to
 *
 * Return: 1 if valid, 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	tx_in_t *in;
	tx_out_t *out;

	if (!coinbase || !coinbase->inputs || !coinbase->outputs)
		return (0);

	if (llist_size(coinbase->inputs) != 1 || llist_size(coinbase->outputs) != 1)
		return (0);

	if (!transaction_hash(coinbase, hash_buf))
		return (0);
	if (memcmp(hash_buf, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	in = llist_get_node_at(coinbase->inputs, 0);
	out = llist_get_node_at(coinbase->outputs, 0);
	if (!in || !out)
		return (0);

	if (memcmp(in->tx_out_hash, &block_index, sizeof(block_index)) != 0)
		return (0);
	if (memcmp(in->block_hash, (uint8_t[SHA256_DIGEST_LENGTH]){0}, SHA256_DIGEST_LENGTH) != 0 ||
		memcmp(in->tx_id, (uint8_t[SHA256_DIGEST_LENGTH]){0}, SHA256_DIGEST_LENGTH) != 0 ||
		in->sig.len != 0)
		return (0);

	if (out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
