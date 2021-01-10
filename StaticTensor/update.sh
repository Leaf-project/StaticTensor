#!/bin/bash
clear
echo "Make sure to use bash"

for file in programs/*.c
do
	filename=${file#*/}		#cut of 'programs/'
	echo -e "[\e[96mCompiling\e[0m] $filename ..."
	gcc $file -o "bin/"${filename/".c"/".exe"} -lm -Wall -Wextra
	#gcc -shared $file -o "lib/"${filename/".c"/".so"} -lm -fPIC
done