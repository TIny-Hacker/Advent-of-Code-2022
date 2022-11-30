#!/bin/bash

if [[ ! -f "cookies.txt" ]] # Ensure the user has a cookies.txt file
then
    echo No cookies.txt file. Please generate a cookies.txt file and put it in the same directory as this script.
    exit 1
fi

echo Prepare directories

if [[ ! -f "Day$2" ]] # Check if the directory has already been created, if not, create it
then
    mkdir Day$2
fi

cd Day$2

echo Download input

wget --load-cookies=../cookies.txt "https://adventofcode.com/$1/day/$2/input"   # Download latest input

echo Convert newlines

tr '\n' '\0' < input > input.txt   # Zero terminate newlines

rm input

echo Convert to 8xv

convbin -j bin -k 8xv -i input.txt -o Input.8xv -n Input    # Convert to appvar

echo Success!
exit 0
