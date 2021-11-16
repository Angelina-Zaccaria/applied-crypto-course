#!/bin/bash
echo "Hello World"
dd if=/dev/zero of=10mb.txt bs=1M count=10
echo "aes 128 cbc"
time openssl enc -e -aes-128-cbc -k "This's my key" -in 10mb.txt -out ciphertext.bin
echo "des-ede-cbc"
time openssl enc -e -des-ede-cbc -k "This's my key" -in 10mb.txt -out ciphertext.bin
