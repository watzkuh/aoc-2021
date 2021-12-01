#!/bin/bash
previous=9223372036854775807 # sort of MAX_INT
no_of_increases=0

while IFS= read -r depth
do
  if [ "$depth" -gt "$previous" ]
    then
    ((no_of_increases=no_of_increases + 1))
  fi
  previous="$depth"
done < "$1"

echo $no_of_increases