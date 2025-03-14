# Gnuplot script for plotting vector data

set xlabel 'Градус наклона (длина = 250)'
set ylabel 'Количество ступенек'
set grid

# Plot 1
set terminal wxt 1 title 'ЦДА'
plot '-' with points pointtype 7 pointsize 1 title 'Кол-во ступенек'
0 0
5 21
10 43
15 64
20 85
25 105
30 125
35 143
40 160
45 176
50 160
55 143
60 125
65 105
70 85
75 64
80 43
85 21
e
pause mouse close
