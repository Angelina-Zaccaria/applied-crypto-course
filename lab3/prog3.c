#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

int main() {
	
	char msg[] = "The test message is:\nHello World!";
	int msg_size = strlen(msg);
	unsigned char digest[EVP_MAX_MD_SIZE];
	int dgst_len;

	EVP_MD_CTX *ctx = EVP_MD_CTX_create();
	EVP_DigestInit(ctx, EVP_md5());
	EVP_DigestUpdate(ctx, msg, msg_size);
	EVP_DigestFinal(ctx, digest, &dgst_len);

	if (digest == NULL) {
		printf("ERROR: Hash function failed.\n");
		printf("%s\n", ERR_error_string(ERR_peek_error(), NULL));
		EVP_MD_CTX_destroy(ctx);
		return 1;
	}
	digest[dgst_len] = '\0';

	printf("Original text: \"%s\"\n", msg);
	printf("Hash in binary: %s\n", digest);
	unsigned char encoded[1024];
	EVP_EncodeBlock(encoded, digest, dgst_len);
	printf("Hash in Base64: %s\n", encoded);

	EVP_MD_CTX_destroy(ctx);
	return 0;
}

/*
 * To run:
 * gcc -g prog3.c -o prog3 -lcrypto
 * ./prog3
 *
 */