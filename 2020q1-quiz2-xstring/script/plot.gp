set title "run time"
set xlabel "string lens"
set ylabel "time (ms)"
set terminal png font " Times_New_Roman,12 "
set key left
set output "result.png"

plot \
"xstr.txt" using 1:2 with linespoints linewidth 2 title "xscpy w/o CoW", \
"strcpy.txt" using 1:2 with linespoints linewidth 2 title "strcpy", \
"xstrcow.txt" using 1:2 with linespoints linewidth 2 title "xscpy with CoW"