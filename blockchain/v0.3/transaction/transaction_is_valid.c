#include <stdlib.h>
#include <string.h>
#include "transaction.h"
#include "hblk_crypto.h"

/**
 * match_unspent - helper to find matching unspent output by hash
 */
static int match_unspent(void *node, void *hash)
{
	unspent_tx_out_t *unspent = node;
	return (memcmp(unspent->out.hash, hash, SHA256_DIGEST_LENGTH) == 0);
}

/**
 * transaction_is_valid - verifies a transaction's integrity and authenticity
 */
int transaction_is_valid(transaction_t const *transaction, llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	size_t i, in_count, out_count;
	tx_in_t *in;
	tx_out_t *out;
	unspent_tx_out_t *ref;
	uint32_t total_in = 0, total_out = 0;
	if (!transaction || !all_unspent)
		return (0);

	if (!transaction_hash(transaction, hash_buf))
		return (0);

	if (memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	in_count = llist_size(transaction->inputs);
	out_count = llist_size(transaction->outputs);

	for (i = 0; i < in_count; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		if (!in)
			return (0);

		ref = llist_find_node(all_unspent, match_unspent, in->tx_out_hash);
		if (!ref)
			return (0);
	EC_KEY *pub_key = ec_from_pub(ref->out.pub);
	if (!pub_key)
		return (0);

	if (!ec_verify(pub_key, transaction->id, SHA256_DIGEST_LENGTH, &in->sig))
	{
		EC_KEY_free(pub_key);
		return (0);
	}
	EC_KEY_free(pub_key);

		total_in += ref->out.amount;
	}
	for (i = 0; i < out_count; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		if (!out)
			return (0);
		total_out += out->amount;
	}

	return (total_in == total_out);
}
