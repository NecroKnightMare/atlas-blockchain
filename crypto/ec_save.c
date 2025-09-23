#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "hblk_crypto.h"

/**
 * ec_save - Save a private EC key in PEM format to a file
 * @key: The EC key to save
 * @folder: The folder where to save the key
 * Return: 1 on success, 0 failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *fp;
	char priv_path[PATH_MAX], pub_path[PATH_MAX];

	if (!key || !folder)
		return (0);

	if (mkdir(folder, 0700) == -1 && errno != EEXIST)
		return (0);

	snprintf(priv_path, PATH_MAX, "%s/key.pem", folder);
	snprintf(pub_path, PATH_MAX, "%s/key_pub.pem", folder);

	fp = fopen(priv_path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);
	fp = fopen(pub_path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_EC_PUBKEY(fp, key))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);
	return (1);
}
