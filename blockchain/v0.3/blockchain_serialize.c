#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"
#include "provided/endianness.h"

#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.1"

/**
 * blockchain_serialize - serializes a blockchain into a file
 * @blockchain: pointer to the blockchain to serialize
 * @path: path to the file to write
 * Return: 0 on success, -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fp;
	uint32_t nb_blocks, nb_unspent, endian = _get_endianness();
	block_t *block;
	transaction_t *tx;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	unspent_tx_out_t *unspent;
	int i, j, k;

	if (!blockchain || !path)
		return (-1);

	fp = fopen(path, "wb");
	if (!fp)
		return (-1);

	fwrite(HBLK_MAGIC, 1, 4, fp);
	fwrite(HBLK_VERSION, 1, 3, fp);
	fwrite(&endian, 1, 1, fp);

	nb_blocks = _htole32(llist_size(blockchain->chain));
	nb_unspent = _htole32(llist_size(blockchain->unspent));
	fwrite(&nb_blocks, sizeof(nb_blocks), 1, fp);
	fwrite(&nb_unspent, sizeof(nb_unspent), 1, fp);

	/* Serialize each block */
	for (i = 0; i < llist_size(blockchain->chain); i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		if (!block)
			break;

		fwrite(&block->info, sizeof(block->info), 1, fp);

		uint32_t data_len_le = _htole32(block->data.len);
		fwrite(&data_len_le, sizeof(data_len_le), 1, fp);
		fwrite(block->data.buffer, 1, block->data.len, fp);
		fwrite(block->hash, 1, SHA256_DIGEST_LENGTH, fp);

		int nb_tx = block->transactions ? llist_size(block->transactions) : -1;
		uint32_t tx_count_le = _htole32(nb_tx);
		fwrite(&tx_count_le, sizeof(tx_count_le), 1, fp);

		if (nb_tx > 0)
		{
			for (j = 0; j < nb_tx; j++)
			{
				tx = llist_get_node_at(block->transactions, j);
				fwrite(tx->id, 1, SHA256_DIGEST_LENGTH, fp);

				uint32_t nb_inputs = _htole32(llist_size(tx->inputs));
				uint32_t nb_outputs = _htole32(llist_size(tx->outputs));
				fwrite(&nb_inputs, sizeof(nb_inputs), 1, fp);
				fwrite(&nb_outputs, sizeof(nb_outputs), 1, fp);

				for (k = 0; k < llist_size(tx->inputs); k++)
				{
					tx_in = llist_get_node_at(tx->inputs, k);
					fwrite(tx_in->block_hash, 1, SHA256_DIGEST_LENGTH, fp);
					fwrite(tx_in->tx_id, 1, SHA256_DIGEST_LENGTH, fp);
					fwrite(tx_in->tx_out_hash, 1, SHA256_DIGEST_LENGTH, fp);
					fwrite(tx_in->sig.sig, 1, SIGNATURE_MAX_LEN, fp);
					fwrite(&tx_in->sig.len, 1, 1, fp);
				}

				for (k = 0; k < llist_size(tx->outputs); k++)
				{
					tx_out = llist_get_node_at(tx->outputs, k);
					uint32_t amount_le = _htole32(tx_out->amount);
					fwrite(&amount_le, sizeof(amount_le), 1, fp);
					fwrite(tx_out->pub, 1, EC_PUB_LEN, fp);
					fwrite(tx_out->hash, 1, SHA256_DIGEST_LENGTH, fp);
				}
			}
		}
	}

	for (i = 0; i < llist_size(blockchain->unspent); i++)
	{
		unspent = llist_get_node_at(blockchain->unspent, i);
		uint32_t amount_le = _htole32(unspent->out.amount);

		fwrite(unspent->block_hash, 1, SHA256_DIGEST_LENGTH, fp);
		fwrite(unspent->tx_id, 1, SHA256_DIGEST_LENGTH, fp);
		fwrite(&amount_le, sizeof(amount_le), 1, fp);
		fwrite(unspent->out.pub, 1, EC_PUB_LEN, fp);
		fwrite(unspent->out.hash, 1, SHA256_DIGEST_LENGTH, fp);
	}

	fclose(fp);
	return (0);
}