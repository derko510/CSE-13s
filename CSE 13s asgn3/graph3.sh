#!/bin/bash

# low elements
for i in {10..100}
do 
    ./sorting -b -n $i -p 0 | awk '{print $3,$5}' >> batcher3.dat
    ./sorting -s -n $i -p 0 | awk '{print $3,$5}' >> shell3.dat
    ./sorting -h -n $i -p 0 | awk '{print $3,$5}' >> heap3.dat
    ./sorting -q -n $i -p 0 | awk '{print $3,$5}' >> quick3.dat

done

gnuplot <<END
set terminal pdf
set output "low elements.pdf"
set title "low elements"
set ylabel "Moves"
set xlabel "Elements"
set xtics auto
set ytics auto
plot "shell3.dat" with lines lt rgb "purple", \
     "batcher3.dat" with lines lt rgb "red", \
     "heap3.dat" with lines lt rgb "green", \
     "quick3.dat" with lines lt rgb "blue"
END

echo "done plotting"