#!/bin/bash
sudo apt install gcc

#Builds Database
mkdir db
cd db
touch notes.csv
touch lines.txt
echo 0 > lines.txt

#Compiles executables
cd ../src
gcc main.c -o clnotesExec

#Systemwide Symlink
cd ..
sudo ln -sf $(pwd)/clnotes /usr/local/bin/clnotes
