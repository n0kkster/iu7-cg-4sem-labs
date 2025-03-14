# Gnuplot script for histogram

set xlabel 'Алгоритмы'
set ylabel 'Время'
set grid
set style data histograms
set style histogram clustered gap 1
set style fill solid 1.0
set boxwidth 0.9
set yrange [0:1]
set terminal wxt 1 title 'Звмер времени работы алгоритмов'
plot '-' using 2:xtic(1) title 'мкс.'
'ЦДА' 0.473
'Брез.(real)' 0.678
'Брез.(int)' 0.511
'Брез.(smooth)' 0.534
'Ву' 0.556
e
pause mouse close
