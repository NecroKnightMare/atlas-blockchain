#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"
#include "provided/endianness.h"

#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.1"

/**
 * blockchain_deserialize - loads a blockchain from a file
 * @path: path to the file to read
 * Return: pointer to the reconstructed blockchain, NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fp;
	blockchain_t *blockchain;
	block_t *block;
	transaction_t *tx;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	unspent_tx_out_t *unspent;
	uint8_t magic[4], version[3], endian_byte;
	uint32_t nb_blocks, nb_unspent, i, j, k;
	uint8_t endian_file, endian_host;
	uint32_t data_len, tx_count, nb_inputs, nb_outputs, amount;

	if (!path || !(fp = fopen(path, "rb")))
		return (NULL);

	if (fread(magic, 1, 4, fp) != 4 || memcmp(magic, HBLK_MAGIC, 4) != 0)
		return (fclose(fp), NULL);

	if (fread(version, 1, 3, fp) != 3 || memcmp(version, HBLK_VERSION, 3) != 0)
		return (fclose(fp), NULL);

	if (fread(&endian_byte, 1, 1, fp) != 1)
		return (fclose(fp), NULL);

	endian_file = endian_byte;
	endian_host = _get_endianness();

	if (fread(&nb_blocks, sizeof(nb_blocks), 1, fp) != 1 ||
		fread(&nb_unspent, sizeof(nb_unspent), 1, fp) != 1)
		return (fclose(fp), NULL);

	if (endian_file != endian_host)
	{
		nb_blocks = __builtin_bswap32(nb_blocks);
		nb_unspent = __builtin_bswap32(nb_unspent);
	}

	blockchain = calloc(1, sizeof(*blockchain));
	if (!blockchain)
		return (fclose(fp), NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain || !blockchain->unspent)
		return (fclose(fp), blockchain_destroy(blockchain), NULL);

	for (i = 0; i < nb_blocks; i++)
	{
		block = calloc(1, sizeof(*block));
		if (!block)
			return (fclose(fp), blockchain_destroy(blockchain), NULL);

		if (fread(&block->info, sizeof(block->info), 1, fp) != 1)
			return (fclose(fp), block_destroy(block),
			blockchain_destroy(blockchain), NULL);

		if (endian_file != endian_host)
		{
			block->info.index = __builtin_bswap32(block->info.index);
			block->info.difficulty = __builtin_bswap32(block->info.difficulty);
			block->info.timestamp = __builtin_bswap64(block->info.timestamp);
			block->info.nonce = __builtin_bswap64(block->info.nonce);
		}

		if (fread(&data_len, sizeof(data_len), 1, fp) != 1)
			return (fclose(fp), block_destroy(block),
			blockchain_destroy(blockchain), NULL);

		if (endian_file != endian_host)
			data_len = __builtin_bswap32(data_len);

		block->data.len = data_len;

		if (fread(block->data.buffer, 1, data_len, fp) != data_len ||
			fread(block->hash, 1, SHA256_DIGEST_LENGTH, fp) !=
			SHA256_DIGEST_LENGTH)
			return (fclose(fp), block_destroy(block),
			blockchain_destroy(blockchain), NULL);

		if (fread(&tx_count, sizeof(tx_count), 1, fp) != 1)
			return (fclose(fp), block_destroy(block),
			blockchain_destroy(blockchain), NULL);

		if (endian_file != endian_host)
			tx_count = __builtin_bswap32(tx_count);

		if (tx_count == (uint32_t)-1)
			block->transactions = NULL;
		else
		{
			block->transactions = llist_create(MT_SUPPORT_FALSE);
			for (j = 0; j < tx_count; j++)
			{
				tx = calloc(1, sizeof(*tx));
				if (!tx || fread(tx->id, 1, SHA256_DIGEST_LENGTH,
					fp) != SHA256_DIGEST_LENGTH)
					return (fclose(fp), block_destroy(block),
					blockchain_destroy(blockchain), NULL);

				if (fread(&nb_inputs, sizeof(nb_inputs), 1, fp) != 1 ||
					fread(&nb_outputs, sizeof(nb_outputs), 1, fp) != 1)
					return (fclose(fp), block_destroy(block),
					blockchain_destroy(blockchain), NULL);
				if (endian_file != endian_host)
				{
					nb_inputs = __builtin_bswap32(nb_inputs);
					nb_outputs = __builtin_bswap32(nb_outputs);
				}

				tx->inputs = llist_create(MT_SUPPORT_FALSE);
				tx->outputs = llist_create(MT_SUPPORT_FALSE);

				for (k = 0; k < nb_inputs; k++)
				{
					tx_in = calloc(1, sizeof(*tx_in));
					if (!tx_in ||
						fread(tx_in->block_hash, 1, SHA256_DIGEST_LENGTH,
						fp) != SHA256_DIGEST_LENGTH ||
						fread(tx_in->tx_id, 1, SHA256_DIGEST_LENGTH,
						fp) != SHA256_DIGEST_LENGTH ||
						fread(tx_in->tx_out_hash, 1, SHA256_DIGEST_LENGTH,
						fp) != SHA256_DIGEST_LENGTH ||
						fread(tx_in->sig.sig, 1, SIGNATURE_MAX_LEN,
						fp) != SIGNATURE_MAX_LEN ||
						fread(&tx_in->sig.len, 1, 1, fp) != 1)
						return (fclose(fp), block_destroy(block),
						blockchain_destroy(blockchain), NULL);

					llist_add_node(tx->inputs, tx_in, ADD_NODE_REAR);
				}

				for (k = 0; k < nb_outputs; k++)
				{
					tx_out = calloc(1, sizeof(*tx_out));
					if (!tx_out || fread(&amount, sizeof(amount), 1, fp) != 1)
						return (fclose(fp), block_destroy(block),
						blockchain_destroy(blockchain), NULL);

					if (endian_file != endian_host)
						amount = __builtin_bswap32(amount);
					tx_out->amount = amount;

					if (fread(tx_out->pub, 1, EC_PUB_LEN, fp) != EC_PUB_LEN ||
						fread(tx_out->hash, 1, SHA256_DIGEST_LENGTH,
						fp) != SHA256_DIGEST_LENGTH)
						return (fclose(fp), block_destroy(block),
						blockchain_destroy(blockchain), NULL);

					llist_add_node(tx->outputs, tx_out, ADD_NODE_REAR);
				}

				llist_add_node(block->transactions, tx, ADD_NODE_REAR);
			}
		}

		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}

	for (i = 0; i < nb_unspent; i++)
	{
		unspent = calloc(1, sizeof(*unspent));
		if (!unspent || fread(unspent->block_hash, 1, SHA256_DIGEST_LENGTH,
			fp) != SHA256_DIGEST_LENGTH ||
			fread(unspent->tx_id, 1, SHA256_DIGEST_LENGTH,
			fp) != SHA256_DIGEST_LENGTH ||
			fread(&amount, sizeof(amount), 1, fp) != 1)
			return (fclose(fp), blockchain_destroy(blockchain), NULL);

		if (endian_file != endian_host)
			amount = __builtin_bswap32(amount);
		unspent->out.amount = amount;

		if (fread(unspent->out.pub, 1, EC_PUB_LEN, fp) != EC_PUB_LEN ||
			fread(unspent->out.hash, 1, SHA256_DIGEST_LENGTH,
			fp) != SHA256_DIGEST_LENGTH)
			return (fclose(fp), blockchain_destroy(blockchain), NULL);

		llist_add_node(blockchain->unspent, unspent, ADD_NODE_REAR);
	}

	fclose(fp);
	return (blockchain);
}
