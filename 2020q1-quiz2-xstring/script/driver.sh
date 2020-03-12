#!/bin/bash 
TEXT=Text.txt
DATA=$1
MODE=$2
CHARS=$(cat $TEXT | wc -m)

MODE=$2
COW=$3
for i in `seq 1 5 $CHARS`
do
    printf "lens: $i "
    ./plot $(cat $TEXT) $i $MODE
done | grep -E "time|lens" | awk '{print $2 " " $4}' >> $DATA