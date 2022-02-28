#!/bin/bash
sudo apt install gcc
mkdir db
cd db
touch notes.csv
touch lines.txt
echo 0 > lines.txt
cd ../src
gcc main.c -o clnotesExec
cd ..
sudo ln -sf $(pwd)/clnotes /usr/local/bin/clnotes
