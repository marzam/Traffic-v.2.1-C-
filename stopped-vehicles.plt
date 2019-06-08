reset
set xrange[145:1550]
#set xrange[1450:1550]
set xtic 100
set xlabel 'Space in cells'
set ylabel 'Velocity variation'

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'velocity-delta-space.eps'

plot 'vehicle.SL-STANDARD-02-HR-1.5-PAPER-BLOCKED.29.txt' u ($1):($3-$4) w lp lt rgb "red" t'Standard', 'vehicle.SL-DARING-HR-1.5-PAPER-BLOCKED.29.txt' u ($1):($3-$4) w lp lt rgb "green" t'Daring', 'vehicle.SL-SLOW-HR-1.5-PAPER-BLOCKED.29.txt' u ($1):($3-$4) w lp lt rgb "blue" t'Slow'

set ylabel 'Velocity in cells/s'
set output 'velocity-space.eps'

plot 'vehicle.SL-STANDARD-02-HR-1.5-PAPER-BLOCKED.29.txt' u ($1):($3) w lp lt rgb "red" t'Standard', 'vehicle.SL-DARING-HR-1.5-PAPER-BLOCKED.29.txt' u ($1):($3) w lp lt rgb "green" t'Daring', 'vehicle.SL-SLOW-HR-1.5-PAPER-BLOCKED.29.txt' u ($1):($3) w lp lt rgb "blue" t'Slow'
