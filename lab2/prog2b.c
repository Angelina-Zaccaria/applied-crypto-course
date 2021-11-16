#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

int main() {
	EVP_PKEY_CTX *ctx;
	EVP_PKEY *pkey = NULL;
	ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

	EVP_PKEY_keygen_init(ctx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);

	/* Generate key */
	if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
		printf("%s\n", ERR_error_string(ERR_peek_error(), NULL));
	else
		printf("Private key successfully generated!\n");

	EVP_PKEY_free(pkey);
	EVP_PKEY_CTX_free(ctx);
	CRYPTO_cleanup_all_ex_data();
	return 0;
}

/*
 * To run:
 * gcc -g prog2b.c -o prog2b -lcrypto
 * ./prog2b
 *
 */