#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

int main() {
	unsigned char plaintext[1024] = "Here's a secret.";
	unsigned char ciphertext[1024];
	unsigned char key[EVP_MAX_KEY_LENGTH+1] = "This is a key";
	unsigned char iv[EVP_MAX_IV_LENGTH+1] = "My iv";
	int in_len, msg_size, out_len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	msg_size = in_len = strlen((char *)plaintext);
	printf("Plaintext = %s\n", plaintext);

	EVP_EncryptInit(ctx, EVP_rc4(), key, iv);
	EVP_EncryptUpdate(ctx, ciphertext, &out_len, plaintext, in_len);
	EVP_EncryptFinal(ctx, &ciphertext[out_len], &out_len);
	printf("Ciphertext = %s\n", ciphertext);

	memset(plaintext, 0, msg_size);
//	printf("Plaintext = %s\n", plaintext);

	EVP_DecryptInit(ctx, EVP_rc4(), key, iv);
	EVP_DecryptUpdate(ctx, plaintext, &in_len, ciphertext, msg_size);
	EVP_DecryptFinal(ctx, &plaintext[in_len], &in_len);
	printf("Decrypted plaintext = %s\n", plaintext);

	EVP_CIPHER_CTX_cleanup(ctx);
	return 0;
}

/*
 * To run:
 * gcc -g prog1.c -o prog1 -lcrypto
 * ./prog1
 *
 */