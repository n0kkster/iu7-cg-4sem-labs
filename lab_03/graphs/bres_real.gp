# Gnuplot script for plotting vector data

set xlabel 'Градус наклона (длина = 250)'
set ylabel 'Количество ступенек'
set grid

# Plot 1
set terminal wxt 1 title 'Брез. (real)'
plot '-' with points pointtype 7 pointsize 1 title 'Кол-во ступенек'
0 0
5 22
10 43
15 65
20 86
25 106
30 125
35 143
40 161
45 177
50 161
55 143
60 125
65 106
70 86
75 65
80 43
85 22
e
pause mouse close
