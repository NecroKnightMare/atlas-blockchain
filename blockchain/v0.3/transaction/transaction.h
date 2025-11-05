#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include "hblk_crypto.h"

typedef struct transaction_s transaction_t;
typedef struct tx_in_s tx_in_t;
typedef struct tx_out_s tx_out_t;


/**
 * struct tx_out_s - Transaction output structure
 *
 * @amount: Amount transferred in this output
 * @pub: Receiver's public key
 * @hash: SHA-256 hash of the output (amount + pub)
 */
typedef struct tx_out_s
{
	uint32_t amount;
	uint8_t pub[EC_PUB_LEN];
	uint8_t hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * struct unspent_tx_out_s - Unspent transaction output
 *
 * @block_hash: Hash of the block containing the transaction
 * @tx_id: ID of the transaction containing the output
 * @out: The actual transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t out;
} unspent_tx_out_t;

/**
 * 
 */
struct transaction_s {
    llist_t *inputs;
    llist_t *outputs;
    uint8_t id[SHA256_DIGEST_LENGTH];
}; transaction_t

/**
 * 
 */
struct tx_in_s {
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t tx_id[SHA256_DIGEST_LENGTH];
    uint8_t tx_out_hash[SHA256_DIGEST_LENGTH];
    EC_KEY *sig_pub;
    sig_t sig;
}; tx_in_t


/*v0.3 prototypes*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
unspent_tx_out_t *unspent_tx_out_create(uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);

#endif /* TRANSACTION_H */
