#MIHAI ALEXANDRU STEFANOIU 


set output "quickSort.png"

set xlabel "size of arrays"
set ylabel "Millions of program steps"
set title "Quicksort"

plot "qs-vs-hs.Msteps" using 1:2 with linespoints title "random arrays", \
     "qs-vs-hs.Msteps" using 1:4 with linespoints title "sorted arrays", \
     "qs-vs-hs.Msteps" using 1:6 with linespoints title "reverse-sorted arrays"


set output "heapSort.png"
set title "Heapsort"
plot "qs-vs-hs.Msteps" using 1:3 with linespoints title "random arrays", \
     "qs-vs-hs.Msteps" using 1:5 with linespoints title "sorted arrays", \
     "qs-vs-hs.Msteps" using 1:7 with linespoints title "reverse-sorted arrays"
     
     
     

set terminal png
set output "qs-vs-hs-RA.png"

set title "Quicksort versus Heapsort (random arrays)"

f_quick(x) = a * x * log(x) + b 
fitFunctionQuick = "Fitting Quicksort time values to f(n) = n log(n)+k"
fit f_quick(x) "qs-vs-hs.Msteps" using 1:2 via a, b

f_heap(x) = c * x * log(x) + d 
fitFunctionHeap = "Fitting Heapsort time values to f(n) = n log(n)+k"
fit f_heap(x) "qs-vs-hs.Msteps" using 1:3 via c, d

# Dibujar la gráfica
plot "qs-vs-hs.Msteps" using 1:2 with linespoints title "Quicksort", \
     "qs-vs-hs.Msteps" using 1:3 with linespoints title "Heapsort", \
     f_quick(x) title fitFunctionQuick, \
     f_heap(x) title fitFunctionHeap



#---------------------------------

set terminal png
set output "qs-vs-hs-SA.png"

set title "Quicksort versus Heapsort (sorted arrays)"
f_quick(x) = a * x * log(x) + b 
fitFunctionQuick = "Fitting Quicksort time values to f(n) = n log(n)+k"
fit f_quick(x) "qs-vs-hs.Msteps" using 1:4 via a, b

f_heap(x) = c * x * log(x) + d 
fitFunctionHeap = "Fitting Heap time values to f(n) = n log(n)+k"
fit f_heap(x) "qs-vs-hs.Msteps" using 1:5 via c, d

# Dibujar la gráfica
plot "qs-vs-hs.Msteps" using 1:4 with linespoints title "Quicksort", \
     "qs-vs-hs.Msteps" using 1:5 with linespoints title "Heapsort", \
     f_quick(x) title fitFunctionQuick, \
     f_heap(x) title fitFunctionHeap
#-----------------------------------------------------------------------

set terminal png
set output "qs-vs-hs-RSA.png"

set title "Quicksort versus Heapsort (reverse-sorted arrays)"

f_quick(x) = a * x * log(x) + b 
fitFunctionQuick = "Fitting Quicksort time values to f(n) = n log(n)+k"
fit f_quick(x) "qs-vs-hs.Msteps" using 1:6 via a, b

f_heap(x) = c * x * log(x) + d 
fitFunctionHeap = "Fitting Heap time values to f(n) = n log(n)+k"
fit f_heap(x) "qs-vs-hs.Msteps" using 1:7 via c, d

# Dibujar la gráfica
plot "qs-vs-hs.Msteps" using 1:6 with linespoints title "Quicksort", \
     "qs-vs-hs.Msteps" using 1:7 with linespoints title "Heapsort", \
     f_quick(x) title fitFunctionQuick, \
     f_heap(x) title fitFunctionHeap

