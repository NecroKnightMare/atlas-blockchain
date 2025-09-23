#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <limits.h>
#include "hblk_crypto.h"

/**
 * ec_load - loads EC key pair from disk
 * @folder: folder containing key files
 * Return: pointer to EC on success, NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
	FILE *fp;
	char priv_path[PATH_MAX];
	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);

	snprintf(priv_path, PATH_MAX, "%s/key.pem", folder);

	fp = fopen(priv_path, "r");
	if (!fp)
		return (NULL);
	key = PEM_read_ECPrivateKey(fp, NULL, NULL, NULL);
	fclose(fp);

	return (key);
}
