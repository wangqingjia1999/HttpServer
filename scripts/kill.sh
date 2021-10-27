#!/bin/bash

# kill all processes

process_name="word_finder"

for pid in $(pgrep "$process_name")
do
    kill $pid
    echo "kill: " $pid
done