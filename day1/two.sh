#!/bin/bash

readarray -t depths < "$1"

len=${#depths[@]}

for (( i=0; i<len-2; i++ ))
do
    (( sum=${depths[$i]} + ${depths[$i+1]} + ${depths[$i+2]} ))
    echo $sum >> tmp
done

./one.sh tmp

rm tmp