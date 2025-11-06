#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * transaction_hash - computes the hash nID of a transaction
 * @transaction: pointer to the transaction to compute the hash for buffer
 * @hash_buf: buffer to store the computed hash
 *
 * Return: pointer to hash_buf
 */
uint8_t *transaction_hash(transaction_t const *transaction,
    uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t input_count, output_count, i;
	size_t buf_size;
	uint8_t *buf;
	tx_in_t *in;
	tx_out_t *out;

	if (!transaction || !hash_buf)
		return (NULL);

	input_count = llist_size(transaction->inputs);
	output_count = llist_size(transaction->outputs);
	buf_size = input_count * (SHA256_DIGEST_LENGTH * 3) +
		output_count * SHA256_DIGEST_LENGTH;

	buf = malloc(buf_size);
	if (!buf)
		return (NULL);

	uint8_t *cursor = buf;

	for (i = 0; i < input_count; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		if (!in)
			continue;

		memcpy(cursor, in->block_hash, SHA256_DIGEST_LENGTH);
        cursor += SHA256_DIGEST_LENGTH;
		memcpy(cursor, in->tx_id, SHA256_DIGEST_LENGTH);
        cursor += SHA256_DIGEST_LENGTH;
		memcpy(cursor, in->tx_out_hash, SHA256_DIGEST_LENGTH);
        cursor += SHA256_DIGEST_LENGTH;
	}

	for (i = 0; i < output_count; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		if (!out)
			continue;

        memcpy(cursor, out->hash, SHA256_DIGEST_LENGTH);
        cursor += SHA256_DIGEST_LENGTH;
    }
	sha256((int8_t const *)buf, buf_size, hash_buf);
	free(buf);
	return (hash_buf);
}
