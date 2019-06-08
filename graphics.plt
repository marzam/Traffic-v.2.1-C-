reset
set yrange[0:3200]
set xrange[0:133]
set ytic 500
set xtic 10
set xlabel 'density (vehicle/km)'
set terminal postscript eps font 'Helvetica,10'
set output 'FD-standard.eps'
plot 'statistic.photo.SL-STANDARD-HR-1.5-PAPER.txt' u ($4*133):(($3/5)*3600) w p notitle
set output 'FD-daring.eps'
plot 'statistic.photo.SL-DARING-HR-1.5-PAPER.txt' u ($4*133):(($3/5)*3600) w p notitle
set output 'FD-slow.eps'
plot 'statistic.photo.SL-SLOW-HR-1.5-PAPER.txt' u ($4*133):(($3/5)*3600) w p notitle
reset
set xrange[0:100]
set xlabel 'distance (m)'
 set ylabel 'frequence'
set terminal postscript  eps enhanced color  font 'Helvetica,10'
 set output 'adjstment-cluster.eps'
plot 'cluster.adjusted.SL-STANDARD-HR-1.5-PAPER.txt' u (1000/($1*133)):($4) w lp t'Standard', 'cluster.adjusted.SL-SLOW-HR-1.5-PAPER.txt' u (1000/($1*133)):($4) w lp t'Slow', 'cluster.adjusted.SL-DARING-HR-1.5-PAPER.txt' u (1000/($1*133)):($4) w lp t'Daring'
