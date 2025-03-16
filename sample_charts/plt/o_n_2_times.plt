set terminal png size 800,600
set output './sample_charts/png/o_n_2_times.png'
set title 'o(n^2)'
set xlabel 'dataset size'
set ylabel 'avg. runtime'
set xrange[:]
set yrange[:]
set grid
f(x)=a*x**2+b
a = 2.0
b = 5.0

fit f(x) './sample_charts/dat/o_n_2_times.dat' using 1:2 via a,b
plot './sample_charts/dat/o_n_2_times.dat' using 1:2 with points title 'Data', \
     f(x) with lines title 'Fit'
