#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
int main() {

    // Open file containing public key
    FILE *in_pub = fopen("pub_key1.pem", "r");
    if (in_pub < 0) {
        printf("ERROR: Public key file not found.\n");
        return 1;
    }

    // Read key into PKEY structure
    EVP_PKEY *pub_key;
    pub_key = PEM_read_PUBKEY(in_pub, NULL, NULL, NULL);
    if (pub_key == NULL)
        printf("ERROR: Couldn't read public key.\n");
    else
        printf("Key successfully obtained.\n");

    // Note that you can encrypt at most 117 bytes with RSA
    unsigned char plaintext[] = "The surprise party starts at 7.";
    printf("The plaintext is %s\n", plaintext);
    size_t out_len, in_len = strlen((char *) plaintext);

    EVP_PKEY_CTX *ctx;
    ctx = EVP_PKEY_CTX_new(pub_key, NULL);
    EVP_PKEY_encrypt_init(ctx);
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    // Check what the output size will be so that we allocate enough buffer
    if (EVP_PKEY_encrypt(ctx, NULL, &out_len, plaintext, in_len) <= 0) {
        printf("ERROR: Couldn't get out_len from initial encrypt.\n");
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pub_key);
        CRYPTO_cleanup_all_ex_data();
        return 1;
    }
    unsigned char cipher[out_len];

    if (EVP_PKEY_encrypt(ctx, cipher, &out_len, plaintext, in_len) <= 0) {
        printf("%s\n", ERR_error_string(ERR_peek_error(), NULL));
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pub_key);
        CRYPTO_cleanup_all_ex_data();
        return 1;
    }
    printf("The ciphertext is %s\n", cipher);

    // Clear the structures and prepare for decrypt
    fclose(in_pub);
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pub_key);
    in_len = out_len;

    // Open file containing private key
    FILE *in_priv = fopen("key1.pem", "r");
    if (in_priv < 0) {
        printf("ERROR: Private key file not found.\n");
        return 1;
    }
    printf("Private key file opened.\n");

    // Read key into PKEY structure
    EVP_PKEY *priv_key;
    priv_key = PEM_read_PrivateKey(in_priv, NULL, NULL, NULL);
    if (priv_key == NULL) {
        printf("ERROR: Couldn't read private key.\n");
        printf("%s\n", ERR_error_string(ERR_peek_error(), NULL));
        return 1;
    }
    printf("Private key successfully obtained.\n");
    fclose(in_priv);

    ctx = EVP_PKEY_CTX_new(priv_key, NULL);
    EVP_PKEY_decrypt_init(ctx);
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    // Check what the output size will be so that we allocate enough buffer
    if (EVP_PKEY_decrypt(ctx, NULL, &out_len, cipher, in_len) <= 0) {
        printf("ERROR: Couldn't get out_len from initial decrypt.\n");
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(priv_key);
        CRYPTO_cleanup_all_ex_data();
        return 1;
    }
    unsigned char decrypted[out_len];

    if (EVP_PKEY_decrypt(ctx, decrypted, &out_len, cipher, in_len) <= 0)
        printf("%s\n", ERR_error_string(ERR_peek_error(), NULL));
    else
        printf("The decrypted plaintext is %s\n", decrypted);

    // Final clean up
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(priv_key);
    CRYPTO_cleanup_all_ex_data();
    return 0;
}

/*
 * To run:
 * gcc -g prog2.c -o prog2 -lcrypto
 * ./prog2
 *
 */
