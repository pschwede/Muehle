#!/bin/bash

echo "" > log

i=0;
while [[ $i -lt 100 ]]; do
  make fight | tail >> log;
  sleep 1;
  i=$(($i + 1));
done

echo "$(grep win log | wc -l) $(grep draw log | wc -l) $(grep invalid log | wc -l)"
