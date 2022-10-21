#!/usr/bin/env bash

arch=".gz"
man="man3"

while read func
do  
    found=false

    IFS=":" # ffffffffffffffffffffffffffffk
    for dir in $MANPATH
    do  
        for subdir in $dir
        do
            file="$subdir/$man/$func.3"
            arch_file="$file$arch"
            line=""
            if [ -f  "$file" ]; then
                line=$(sed '' "$file")
            elif [ -f  "$arch_file" ]; then
                line=$(gunzip -c "$arch_file")
            fi
            if [ ! -z  "$line" ]; then
                echo $(echo "$line" | sed -n "s/^\.B #include <\([a-zA-Z0-9._]*\)>/\1/p")
                found=true
            fi
        done
    done
    if [[ $found == false ]]; then
        echo "---"
    fi
done
