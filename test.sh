#!/bin/bash

if [ $1 ]; then
  limit=$1
else
  limit=10
fi

if [ $2 ]; then
  delay=$2
else
  delay=1
fi

echo "" > log

i=0;
while [[ $i -lt $limit ]]; do
  make fight | tail >> log;
  sleep $delay;
  i=$(($i + 1));
done

echo "$(grep win log | wc -l) gewonnen, $(grep draw log | wc -l) unentschieden, $(grep invalid log | wc -l) invalid moves"
