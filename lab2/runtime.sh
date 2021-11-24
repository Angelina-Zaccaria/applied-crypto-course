#!/bin/bash

# create txt files of given sizes
sizes=( 1b 1K 1M 10M 100M )
for size in "${sizes[@]}"
do
	dd if=/dev/zero of=$size.txt bs=$size count=1
done

# set up output file
echo "Algorithm, Msg size, Key size, real, user, sys" > log.csv

# algorithms=( "-aes-128-cbc" "-aes-256-cbc" "-des-ede-cbc" )
algorithms=()
while read line; do algorithms+=($line); done < algorithms.txt
digests=()
while read line; do digests+=($line); done < digests.txt

for msg_size in "${sizes[@]}"
do
	for alg in "${algorithms[@]}"
	do
		for key_size in "${sizes[@]}"
		do
			echo "$alg msg:$msg_size key:$key_size"
		# time openssl enc -e $alg -k "This's my key" -in $size.txt -out ciphertext.bin
		/usr/bin/time -ao log.csv -f "$alg, $msg_size, $key_size, %e, %U, %S" openssl enc -e -$alg -k $key_size.txt -in $msg_size.txt -out ciphertext.bin >> log.csv
		done
	done
done


