#!/bin/bash

echo "" > log

i=0;
while [[ $i -lt 10 ]]; do
  make fight | tail >> log;
  sleep 1;
  i=$(($i + 1));
done

echo "$(grep win log | wc -l) gewonnen, $(grep draw log | wc -l) unentschieden, $(grep invalid log | wc -l) invalid moves"
