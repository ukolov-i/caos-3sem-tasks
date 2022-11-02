#!/usr/bin/env bash


declare -A mat
rows=0
columns=0


function gauss(){
    curr_row=0
    curr_column=0

    while [[ $curr_row -lt $rows ]] && [[ $curr_column -lt $columns ]]
    do
        local pivot=$curr_row
        while [[ $pivot -lt $rows ]] && [[ $(echo "${mat[$pivot, $curr_column]}==0" | bc) -eq 1 ]]
        do
            pivot=$((pivot+1))
        done

        if [[ $pivot -eq $rows ]]
        then
            curr_column=$((curr_column+1))
            continue
        fi

        for (( i=0; i<$columns; i++ ))
        do
            tmp=${mat[$pivot, $i]}
            mat[$pivot, $i]=${mat[$curr_row, $i]}
            mat[$curr_row, $i]=$tmp
        done

        for (( k=0; k<$rows; k++ ))
        do
            if [[ ! $k -eq $curr_row ]]
            then
                ratio=$(echo "${mat[$k, $curr_column]}/${mat[$curr_row, $curr_column]}" | bc -l)
                for (( l=$curr_column; l<$columns; l++ ))
                do
                    substract=$(echo "$ratio*${mat[$curr_row, $l]}" | bc -l)
                    substract=$(echo -1*$substract| bc)
                    mat[$k, $l]=$(echo "${mat[$k, $l]}+$substract" | bc)
                done
            fi
        done       
        curr_column=$((curr_column+1))
        curr_row=$((curr_row+1))
    done

    for (( i=0; i < $rows; i++ ))
    do
        echo "${mat[$i, $rows]}/${mat[$i, $i]}" | bc -l
    done
}


while read row
do
    IFS=","
    columns=0
    for elem in $row
    do
        mat[$rows, $columns]=$elem
        columns=$((columns+1))
    done
    rows=$((rows+1))
done < $1

gauss

