#!/bin/bash

    #figure 2
    # returns: 1 (blue) if the point is in the circle, 0 (red) otherwise
    ./monte_carlo -n 10000 | tail -n +2 > /tmp/store.dat
    awk '$5 != 1{print $3,$4}' /tmp/store.dat > /tmp/red.dat
    awk '$5 != 0{print $3,$4}' /tmp/store.dat > /tmp/blue.dat

    #figure 3
    # pi - columnn 2 gives error
    ./monte_carlo -r 1 -n 20000 | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est1.dat
    ./monte_carlo -r 2 -n 20000 | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est2.dat
    ./monte_carlo -r 3 -n 20000 | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est3.dat
    ./monte_carlo -r 4 -n 20000 | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est4.dat
    ./monte_carlo -r 5 -n 20000 | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est5.dat

    #interesting graph
    # pi error within the cirle
    ./monte_carlo -n 20000 -r 1 | tail -n +2 > /tmp/store2.dat
    awk '$5 != 0{print $1,$2}' /tmp/store2.dat > /tmp/blue2.dat
    awk '$5 != 1{print $1,$2}' /tmp/store2.dat > /tmp/red2.dat
    # blue error data
    cat /tmp/blue2.dat | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est6.dat
    # red error data 
    cat /tmp/red2.dat | tail -n +2 | awk '{print $1,3.14159265358979-$2}' > /tmp/error_est7.dat

gnuplot <<END
set terminal pdf
set title "figure 2"
set output "figure2.pdf"
set size square
set xrange [0:1]
set yrange[0:1]
set key off
set xtics auto
set ytics auto
set object circle at 0,0 size 1 fc rgb "black" 
plot "/tmp/blue.dat" with points pt 0 lt rgb "blue", \
     "/tmp/red.dat" with points pt 0 lt rgb "red"
END

gnuplot <<END
set terminal pdf
set output "figure3.pdf"
set title "Monte Carlo Error Estimation"
set xrange [1:16384]
set yrange[-1:1]
set ylabel "Erorr"
set key off
set logscale x 2
set ytics auto
plot "/tmp/error_est1.dat" with lines lt rgb "red", \
     "/tmp/error_est2.dat" with lines lt rgb "blue", \
     "/tmp/error_est3.dat" with lines lt rgb "yellow", \
     "/tmp/error_est4.dat" with lines lt rgb "green", \
     "/tmp/error_est5.dat" with lines lt rgb "orange"
END

gnuplot <<END
set terminal pdf
set output "inter.pdf"
set title "Interesting Graph"
set xrange [1:16384]
set yrange[-1:1]
set ylabel "Error"
set key off
set logscale x 2
set ytics auto
plot "/tmp/error_est6.dat" with lines lt rgb "black", \
     "/tmp/error_est7.dat" with lines lt rgb "green", \
     "/tmp/error_est1.dat" with lines lt rgb "yellow"
END

echo "Done Plotting" 
