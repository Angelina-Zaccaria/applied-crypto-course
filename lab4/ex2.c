// Compile: gcc -g ex2.c -o ex2 -lcrypto
// Run: ./ex2

#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

int main() {

	// Make sure key array has size 16
	unsigned char key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0x1a, 0xbc, 0xde, 0xf0, 0x01, 0x23, 0x45, 0x67, 0x00, 0x00, 0x00};
	unsigned char iv[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0x00};
	unsigned char known[] = "The unknown message is:";
	unsigned char cipherB64[] = "A8K+2+60yXYmEBQHOW4BN/2f/ubS5JMHy/B4hpGiaHDbR4qGLBL0AiwvUKi/th+lHt2meNQ82Zmfmnk2+rqd6dBERgBe82v7Smvri2T3VNuhu00h42l6CWHfPguguX5Ya1MNvmBxvMWygk21q+t0nA==";

	printf("The ciphertext is %s\n\n", cipherB64);

	unsigned char ciphertext[1024];
	int in_len = strlen((char *) known);
	int out_len, tmp, found = 0;

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	int i, j, k;

	// Iterate through all possible key values
	for (i = 0; i < 256; i++) {
		key[13] = i;
		for (j = 0; j < 256; j++) {
			key[14] = j;
			for (k = 0; k < 256; k++) {
				key[15] = k;

				EVP_EncryptInit(ctx, EVP_aes_128_cbc(), key, iv);
				EVP_EncryptUpdate(ctx, ciphertext, &out_len, known, in_len);
			// Don't do final encrypt because padding changes the ciphertext
				unsigned char encoded[1024];
				int enc_len = EVP_EncodeBlock(encoded, ciphertext, out_len);
				// The encoded value is padded with == at the end
				encoded[enc_len-2] = '\0';

				// Compare partial cipher to given cipher
				// But don't include the null terminator at the end
				int comp = strncmp(cipherB64, encoded, enc_len-3);
				if (comp == 0) {
					found = 1;
					EVP_CIPHER_CTX_cleanup(ctx);
					break;
				}

				// Clean up for next iteration
				EVP_CIPHER_CTX_cleanup(ctx);
			}

			if (found == 1)
				break;
		}

		if (found == 1)
			break;
	}

	if (found == 1) {
		printf("The last 3 bytes of the key are %02x, %02x, %02x\n", i, j, k);
		
		// Now find the plaintext
		unsigned char plaintext[1024];
		unsigned char decoded[1024];
		int b64_len = strlen((char *)cipherB64);
		in_len = EVP_DecodeBlock(decoded, cipherB64, b64_len);
		// Don't count padding bits in the decoded ciphertext
		if (in_len % 4 != 0)
			in_len -= in_len % 4;
		out_len = 0;

		EVP_DecryptInit(ctx, EVP_aes_128_cbc(), key, iv);
		EVP_DecryptUpdate(ctx, plaintext, &out_len, decoded, in_len);
		EVP_DecryptFinal(ctx, plaintext + out_len, &tmp);
		out_len += tmp;
		plaintext[out_len] = '\0';

		printf("\nThe plaintext is %s\n", plaintext);

		EVP_CIPHER_CTX_cleanup(ctx);
	}
	else
		printf("No match found\n");

	return 0;
}
