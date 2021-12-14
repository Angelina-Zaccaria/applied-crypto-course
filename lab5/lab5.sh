#!/bin/bash

read -p "To run the next command hit ENTER"

commands=()
while IFS= read -r line; do commands+=($line); done < commands.txt

for command in "${commands[@]}"
do
	read -p "$ $command"
	$command
done
