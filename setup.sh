#!/bin/bash
sudo apt install gcc

#Builds Database
mkdir db
cd db
touch notes.csv
echo 0 > lines.txt

cd ../src

#Builds Database for Debugging if allowed
read -p "Do you want to build a database for debugging?(y/any key) " ANS

if [[ $ANS == y || $ANS == Y ]]; then
    touch testDB.csv
    echo 0 > testLines.txt
    echo "On debugging compile with 'gcc ... -D DEBUG'"
fi

#Compiles executables
gcc main.c -o clnotesExec

#Systemwide Symlink
cd ..
sudo ln -sf $(pwd)/clnotes /usr/local/bin/clnotes
