set terminal pngcairo enhanced font "arial,32" fontscale 1.0 size 2000,2000
set output 'simulation.png'
set grid lw 4
set title "System state"

set style line 5 lt rgb "red" lw 5
set style line 6 lt rgb "green" lw 5
set style line 7 lt rgb "blue" lw 5

set ylabel "Temperatura"
set xlabel "Tiempo (seg)"

plot 'a.dat' with lines ls 7 title 'Temp. Placa', 'b.dat' with lines ls 5 title 'Temp. Calentador', 'z.dat' with lines ls 6 title 'Temp. Referencia'

replot

set title "Heatplate Heatmap"
unset key
unset ylabel
unset xlabel
set tic scale 0
set size square
set palette defined (0 "white", 1 "yellow", 2 "orange", 3 "red", 4 "black")
set cbrange [0:50]
set cblabel "Temperature"

set xrange[-0.5:39]
set yrange[-0.5:39]

set view map
set terminal gif animate enhanced font "arial,16" fontscale 1.0 delay 1 size 1200,800
set output 'heatmap.gif'
