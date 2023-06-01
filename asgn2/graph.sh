#!/bin/bash

gnuplot <<END

    set terminal pdf
    set output "madhava.pdf"
    set title "madhava.c"
    set xrange [0:10]
    set xtics auto
    set ytics auto
    set xlabel "iterations"


    plot "madhava.dat" title "" with linespoints lc rgb "blue", \
    "pi.dat" title "pi" with lines lc rgb "red"

END