// Compile: gcc -g ex1.c -o ex1 -lcrypto
// Run: ./ex1

#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

int main() {

	unsigned char cipherB64[] = "hTpVHO39rnpFyThzbcI+gg0rdBcbHL+5OqWFFY8QDF6T/nX4+O1lXFgJnDbRFaC1lL5hFY3uVsbQ8mP14yeSDNnD2dFnhBZLbkjqriE8IwJfcA3yL1Q3LhKQVUWPgPrZVvL98RtKl9ZEYCW/Sb7egw==";
	unsigned char key[EVP_MAX_KEY_LENGTH+1] = "Have you failed?";
	unsigned char iv[EVP_MAX_IV_LENGTH+1] = "This is your IV!";
	unsigned char plaintext[1024];

	printf("The ciphertext is %s\n", cipherB64);

	// Convert Base64 ciphertext back to binary for decryption
	unsigned char cipher[1024];
	int in_len = EVP_DecodeBlock(cipher, cipherB64, strlen((char *)cipherB64));
	// Don't count padding bits in the decoded ciphertext
	if (in_len % 4 != 0)
		in_len -= in_len % 4;	

	int out_len;

	// Perform decryption
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit(ctx, EVP_aes_128_cbc(), key, iv);
	EVP_DecryptUpdate(ctx, plaintext, &out_len, cipher, in_len);
	EVP_DecryptFinal(ctx, plaintext + out_len, &out_len);

	printf("\nThe plaintext is %s\n", plaintext);

	// Clean up
	EVP_CIPHER_CTX_cleanup(ctx);
	return 0;
}
