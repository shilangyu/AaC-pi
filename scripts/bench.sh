#!/bin/sh

out=bench.csv
outtmp=bench-out.txt

make clean release

# warmup
for i in {1..5}; do
	./build/apps/pi generate /dev/null 100
done

echo 'n,time[ms]' > $out

for n in {1..8}; do
	ts=$(date +%s%N)
	./build/apps/pi generate $outtmp $((10**$n))
	echo "$n,$((($(date +%s%N) - $ts)/1000000))" >> $out 
done


rm $outtmp || true
