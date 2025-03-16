set terminal png size 800,600
set output './sample_charts/png/o_n_times.png'
set title 'o(n)'
set xlabel 'dataset size'
set ylabel 'avg. runtime'
set xrange[:]
set yrange[:]
plot './sample_charts/dat/o_n_times.dat' using 1:2 with points title 'Data'
