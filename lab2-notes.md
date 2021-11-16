Part 3:
- int out_len on line 26 needs to be changed to size_t - results in oaep (padding) decoding error

TODO - tail end of Part 3 and all of Part 4:
- fix padding issue in prog2.c https://linux.die.net/man/3/evp_pkey_decrypt
- bash script to compare algorithm runtimes
- client/server key exchange
