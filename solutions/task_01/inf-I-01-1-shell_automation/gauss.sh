!usr/bin/env bash


declare -A matrix
rows=0
columns=0


function swap_lines(){
    first=$1
    second=$2
    size=$3
    for (( i=0; i<$size; i++ ))
    do
        tmp=${matrix[$first, $i]
        matrix[$first, $i]=${matrix[$second, $i]}
        matrix[$second, $i]=$tmp
    done
}



while read row
do
    IFS=","
    i=0
    for elem in row
    do
        matrix[$rows, $i]=$elem
        i=$((i+1))
    done
    rows=$((rows+1))
done < $1

