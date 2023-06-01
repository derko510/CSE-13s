#!/bin/bash

# Compares // reversed
for i in {10..10000}
do 
    ./sorting -b -n $i -p 0 | awk '{print $3,$7}' >> batcher.dat
    ./sorting -s -n $i -p 0 | awk '{print $3,$7}' >> shell.dat
    ./sorting -h -n $i -p 0 | awk '{print $3,$7}' >> heap.dat
    ./sorting -q -n $i -p 0 | awk '{print $3,$7}' >> quick.dat

done

gnuplot <<END
set terminal pdf
set output "Compares reversed.pdf"
set title "Compares reversed"
set xrange [10:10000]
set ylabel "Compares"
set xlabel "Elements"
set logscale y 10
set logscale x 10
plot "shell.dat" with line lt rgb "purple", \
     "batcher.dat" with line lt rgb "red", \
     "heap.dat" with line lt rgb "green", \
     "quick.dat" with line lt rgb "blue"
END

echo "done plotting"