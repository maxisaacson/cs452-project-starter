# Gnuplot script for plotting data in "data.dat"
set autoscale
unset log
unset label
set title "Time to Sort vs. Number of Threads"
set xlabel "Number of Threads"
set ylabel "Time to Sort (milliseconds)"
set style data linespoints
set term png
set output filename
plot "data.dat" using 2:1 title "Time to Sort" with linespoints
