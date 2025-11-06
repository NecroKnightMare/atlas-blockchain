#include <stdlib.h>
#include "transaction.h"

/**
 * transaction_destroy - frees a transaction and its contents
 * @transaction: pointer to the transaction to free
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	llist_destroy(transaction->inputs, 1, free);
	llist_destroy(transaction->outputs, 1, free);
	free(transaction);
}
