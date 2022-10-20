#!/usr/bin/env bash
arch=".3.gz"
man="man3"

while read func
do  
    found=false
    # echo "MANPATH"
    # echo $MANPATH
    for dir in $MANPATH
    do  
        # echo "dir"
        # echo $dir
        if [ -e $dir/$man ]; then
            # echo "here 0"
            way=$dir
            if [ -f $way/$man/$func$arch ]; then
                # if sed  '/#include/!{q1};' $way/$man/$func$arch
                # if zgrep -q '#include' $way/$man/$func$arch
                if (gunzip -c $way/$man/$func$arch | sed -n -e '#include')
                # if (gunzip -c $way/$man/$func$arch | sed -n -e '#include')
                
                then
                    # echo "here"
                    # echo $?
                    #uncompressed=gunzip -c $way/$man/$func$arch
                    # line="$(gunzip -c $way/$man/$func$arch | grep -m 1 '#include')"
                    line="$(gunzip -c $way/$man/$func$arch | sed -n -e '/^.*\#include/p')"
                    # line="$(cat ./test.txt | sed '#include')"
                    # line="$(cat ./test.txt | sed -n -e '/^.*\#include/p')"
                    # line="$(grep -m 1 '#include' $uncompressed)"
                    # line="$(sed '/#include' $uncompressed)"
                    echo $line | sed 's/.B #include <//' | sed 's/>//'
                    found=true
                    break
                fi
            elif [ -f $way/$man/$func$notarch ]; then
                # echo "here 2"
                # if zgrep -q '#include' $way/$man/$func$notarch
                if (echo $way/$man/$func$notarch | sed -n -e '#include')
                then
                    # echo "here3"
                    # line="$( zgrep -m 1 '#include' $way/$man/$func$notarch)"
                    # echo $line | sed 's/.B #include <//' | sed 's/>//'
                    # found=true
                    # break
                    line="$(echo $way/$man/$func$notarch | sed -n -e '/^.*\#include/p')"
                    echo $line | sed 's/.B #include <//' | sed 's/>//'
                    found=true
                    break
                fi
            fi
        fi
    done
    if [[ $found == false ]]; then
        echo "---"
    fi
    # echo "done"
done