#!/bin/bash

# Moves // reversed
for i in {10..10000}
do 
    ./sorting -b -n $i -p 0 | awk '{print $3,$5}' >> batcher1.dat
    ./sorting -s -n $i -p 0 | awk '{print $3,$5}' >> shell1.dat
    ./sorting -h -n $i -p 0 | awk '{print $3,$5}' >> heap1.dat
    ./sorting -q -n $i -p 0 | awk '{print $3,$5}' >> quick1.dat

done

gnuplot <<END
set terminal pdf
set output "Moves reversed.pdf"
set title "Moves reversed"
set xrange [10:10000]
set ylabel "Moves"
set xlabel "Elements"
set logscale y 10
set logscale x 10
plot "shell1.dat" with lines lt rgb "purple", \
     "batcher1.dat" with lines lt rgb "red", \
     "heap1.dat" with lines lt rgb "green", \
     "quick1.dat" with lines lt rgb "blue"
END

echo "done plotting"