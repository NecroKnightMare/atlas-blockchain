#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <string.h>
/*added header for task 2 main*/
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>


/*task 0*/
#define SHA256_DIGEST_LENGTH 32

/*task 1*/
#define EC_CURVE NID_secp256k1


/*prototypes*/
uint8_t *sha256(int8_t const *s, size_t len,
	uint8_t digest[SHA256_DIGEST_LENGTH]);
EC_KEY *ec_create(void);


#endif /* HBLK_CRYPTO_H */
