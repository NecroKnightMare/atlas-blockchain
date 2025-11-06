#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * match_unspent - helper to find matching unspent output by hash
 * @node: pointer to unspent_tx_out_t
 * @hash: pointer to tx_out_hash to match
 *
 * Return: 1 if match, 0 otherwise
 */
static int match_unspent(void *node, void *hash)
{
	unspent_tx_out_t *unspent = node;

	return (memcmp(unspent->out.hash, hash, SHA256_DIGEST_LENGTH) == 0);
}

/**
 * tx_in_sign - signs a transaction input
 * @in: pointer to transaction input to sign
 * @tx_id: ID of the transaction containing this input
 * @sender: sender's private key
 * @all_unspent: list of all unspent transaction outputs
 *
 * Return: pointer to signature structure, NULL on failure
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
    EC_KEY const *sender, llist_t *all_unspent)
{
	unspent_tx_out_t *ref;
	uint8_t pub[EC_PUB_LEN];

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	ref = llist_find_node(all_unspent,	match_unspent, (void *)in->tx_out_hash);
    
	if (!ref)
		return (NULL);

	if (!ec_to_pub(sender, pub))
		return (NULL);

	if (memcmp(pub, ref->out.pub, EC_PUB_LEN) != 0)
		return (NULL);

	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}
