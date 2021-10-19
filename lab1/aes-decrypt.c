#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

int main() {
	unsigned char ciphertext[1024] = "hTpVHO39rnpFyThzbcI+gg0rdBcbHL+5OqWFFY8QDF6T/nX4+O1lXFgJnDbRFaC1lL5hFY3uVsbQ8mP14yeSDNnD2dFnhBZLbkjqriE8IwJfcA3yL1Q3LhKQVUWPgPrZVvL98RtKl9ZEYCW/Sb7egw==";
	unsigned char plaintext[1024];
	unsigned char key[EVP_MAX_KEY_LENGTH+1] = "Have you failed?";
	unsigned char iv[EVP_MAX_IV_LENGTH+1] = "This is your IV!";
	int in_len, msg_size, out_len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	msg_size = in_len = strlen((char *)ciphertext);
	printf("Ciphertext = %s\n", ciphertext);
	
	EVP_DecryptInit(ctx, EVP_aes_128_cbc(), key, iv);
	EVP_DecryptUpdate(ctx, plaintext, &in_len, ciphertext, msg_size);
	EVP_DecryptFinal(ctx, &plaintext[in_len], &in_len);
	printf("Decrypted plaintext = %s\n", plaintext);

	EVP_CIPHER_CTX_cleanup(ctx);
	return 0;
}

