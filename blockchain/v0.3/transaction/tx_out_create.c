#include <stdlib.h>
#include <string.h>
#include "transaction.h"
#include "hblk_crypto.h"
#include "llist.h"

/**
 * tx_out_create - creates and initializes a transaction output structure
 * @amount: amount of the transaction
 * @pub: public key of the transaction receiver
 *
 * Return: pointer to the created transaction output, or NULL on failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *out;

	if (!pub || amount == 0)
		return (NULL);

	out = calloc(1, sizeof(tx_out_t));
	if (!out)
		return (NULL);

	out->amount = amount;
	memcpy(out->pub, pub, EC_PUB_LEN);

	if (!sha256((int8_t const *)out, sizeof(uint32_t) + EC_PUB_LEN, out->hash))
	{
		free(out);
		return (NULL);
	}

	return (out);
}
