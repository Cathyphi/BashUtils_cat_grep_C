#!/bin/bash

filename1=test1.txt

flags=("" "-e" "-i" "-l" "-n" "-s" "-v" "-c" "-h")
flagscomb=("-nc" "-iv" "-ls" "-hc" "-ln" "-il" "-ns" "-hl" "-hv")

make

for ((i=0; i <=8; i++))
do
    n=$((i+1))
    echo -e "\n\nTEST $n \n---------\n">>results21grep.txt
    ./s21_grep ${flags[$i]} cat "$filename1">>results21grep.txt
    echo -e "\n\nTEST $n \n---------\n">>resultgrep.txt
    grep ${flags[$i]} cat "$filename1">>resultgrep.txt
    
    echo -e "\n\nTEST $n \n---------\n">>results21grep.txt
    ./s21_grep ${flagscomb[$i]} cat "$filename1">>results21grep.txt
    echo -e "\n\nTEST $n \n---------\n">>resultgrep.txt
    grep ${flagscomb[$i]} cat "$filename1">>resultgrep.txt

    if diff results21grep.txt resultgrep.txt
    then
        echo ${flags[$i]} SUCCESS
        echo ${flagscomb[$i]} SUCCESS
    else
        echo ${flags[$i]} FAIL
        echo ${flagscomb[$i]} FAIL
    fi
done
