#!/bin/bash
output="series,time,record\n"
for (( i = 0; i <= 9; i++ ))
do
for (( series = 0; series <= 9; series++ ))
do
row="\"series $series run $i:\","
./genetic_algorithm $i $series
time="\"$(cat time.txt)\""
record="\"$(cat record.txt)\""
row+="$time,$record\n"
output+=$row
done
done
output+="\n"
echo -e $output >> results.csv

