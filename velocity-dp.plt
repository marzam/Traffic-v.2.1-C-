reset
set xrange[0:1]
set xlabel 'Occupation (%)'
set ylabel 'Velocity SD'
set xtic 0.1
set ytic 1
set grid
set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'VelocitySD.eps'
plot 'velocity.distance.SL-STANDARD-HR-1.5-PAPER.txt' u 1:6 w lp lt rgb "red" t'Standard', 'velocity.distance.SL-DARING-HR-1.5-PAPER.txt' u 1:6 w lp lt rgb "green" t'Daring', 'velocity.distance.SL-SLOW-HR-1.5-PAPER.txt' u 1:6 w lp lt rgb "blue" t'Slow'


