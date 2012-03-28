#!/bin/bash

if [ $1 ]; then
  limit=$1
else
  limit=10
fi

echo "" > log

i=0;
while [[ $i -lt $limit ]]; do
  make fight | tail >> log;
  sleep 1;
  i=$(($i + 1));
done

echo "$(grep win log | wc -l) gewonnen, $(grep draw log | wc -l) unentschieden, $(grep invalid log | wc -l) invalid moves"
