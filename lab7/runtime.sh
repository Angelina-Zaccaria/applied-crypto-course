#!/bin/bash

# create txt files of given sizes
sizes=( 1b 1K 1M 10M 100M )
for size in "${sizes[@]}"
do
	dd if=/dev/zero of=$size.txt bs=$size count=1
done

# set up output file
echo "Encryption alg, Msg size, Key size, real, user, sys" > enc_log.csv
echo "Signature alg, Msg size, real, user, sys" > sig_log.csv

# algorithms=( "-aes-128-cbc" "-aes-256-cbc" "-des-ede-cbc" )
algorithms=()
while read line; do algorithms+=($line); done < algorithms.txt
digests=()
while read line; do digests+=($line); done < digests.txt

for msg_size in "${sizes[@]}"
do
	for key_size in "${sizes[@]}"
	do
		for alg in "${algorithms[@]}"
		do
			echo "$alg msg:$msg_size key:$key_size"
			# time openssl enc -e $alg -k "This's my key" -in $size.txt -out ciphertext.bin
			/usr/bin/time -ao enc_log.csv -f "$alg, $msg_size, $key_size, %e, %U, %S" openssl enc -e -$alg -k $key_size.txt -in $msg_size.txt -out ciphertext.bin
		done
	done
	for dgst in "${digests[@]}"
	do
		echo "$dgst msg:$msg_size"
		/usr/bin/time -ao sig_log.csv -f "$dgst, $msg_size, %e, %U, %S" openssl dgst -$dgst $msg_size.txt
	done
done


