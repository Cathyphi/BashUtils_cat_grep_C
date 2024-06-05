#!/bin/bash

filename1=test1.txt
filename2=test2.txt

flags=("" "-b" "-e" "-n" "-s" "-t")


make

for ((i=0; i <=5; i++))
do
    n=$((i+1))
    echo "\n\nTEST $n \n---------\n">>results21cat.txt
    ./s21_cat ${flags[$i]} "$filename1">>results21cat.txt
    echo "\n\nTEST $n \n---------\n">>resultcat.txt
    cat ${flags[$i]} "$filename1">>resultcat.txt
    echo "\n\nTEST $n \n---------\n">>results21cat.txt
    ./s21_cat ${flags[$i]} "$filename2">>results21cat.txt
    echo "\n\nTEST $n \n---------\n">>resultcat.txt
    cat ${flags[$i]} "$filename2">>resultcat.txt

    if diff results21cat.txt resultcat.txt
    then
        echo ${flags[$i]} SUCCESS
    else
        echo ${flags[$i]} FAIL
    fi
done
