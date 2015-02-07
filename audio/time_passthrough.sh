#!/bin/sh

NS="1 2 4 8 16 32 64 128 256 512 1024"; # What to measure over
for n in $NS; do
    echo $n;
    # /usr/bin/time cat /dev/zero | head -c 1000000 | ./a.out $n > /dev/null
    # echo $(( n * 4000  )) 
    # cat /dev/zero | head -c 10240000 | ./a.out $n > /dev/null
    cat /dev/zero | head -c 1000000 | ./a.out $n > /dev/null
    # 10240000
    # cat /dev/zero | head -c $(( n * 102400000  )) | ./passthrough $n > /dev/null
    printf "\n";
done
