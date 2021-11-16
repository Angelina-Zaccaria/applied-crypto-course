// Compile: gcc -g ex3.c -o ex3 -lcrypto
// Run: ./ex3 username password

#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

int main(int argc, char const *argv[]) {

	unsigned char cipher1_B64[] = "o9k5Gm1iUvNQJ5wtLPl+dIMk/bmxe9RY";
	unsigned char cipher2_B64[] = "H5t2slzx1alYS91tOGesOubScL3R1UIq";
	unsigned char iv[EVP_MAX_IV_LENGTH+1] = "My iv";

	// Checking input values
	if (argc != 3) {
		printf("ERROR: Incorrect number of arguments\n");
		printf("Usage: ./ex3 username password\n");
		return 0;
	}

	// Prepare username
	int uname_len = strlen(argv[1]);
	if (uname_len > 16) {
		printf("ERROR: Username must be 16 characters or less\n");
		return 0;
	}
	unsigned char username[17];
	memcpy(username, argv[1], uname_len);
	if (uname_len < 16) {
		// Pad username field if necessary
		int i;
		for (i = uname_len; i < 16; i++) {
			username[i] = ' ';
		}
	}
	username[16] = '\0';

	// Prepare key
	int pswd_len = strlen(argv[2]);
	unsigned char key[pswd_len];
	if (pswd_len > 16) {
		printf("ERROR: Password length is maximum 16 characters\n");
		return 0;
	}
	memcpy(key, argv[2], pswd_len);
	key[pswd_len] = '\0';

	// Encrypt the given username with the password
	unsigned char ciphertext[1024];
	unsigned char cipherB64[1024]; 
	int out_len, tmp;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit(ctx, EVP_des_cbc(), key, iv);
	EVP_EncryptUpdate(ctx, ciphertext, &out_len, username, 16);
	EVP_EncryptFinal(ctx, &ciphertext[out_len], &tmp);
	out_len += tmp;

	// Compare to known ciphertexts
	EVP_EncodeBlock(cipherB64, ciphertext, strlen((char *)ciphertext));
	if (strncmp(cipherB64, cipher1_B64, 32) == 0) {
		printf("Welcome %s!\n", argv[1]);
	}
	else if (strncmp(cipherB64, cipher2_B64, 32) == 0) {
		printf("Welcome %s!\n", argv[1]);
	}
	else
		printf("Error: Incorrect username and/or password.\n");

	EVP_CIPHER_CTX_cleanup(ctx);
	return 0;
}
