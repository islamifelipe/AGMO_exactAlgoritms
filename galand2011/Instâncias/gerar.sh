#!/bin/bash




for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do


	./padronizador < ../../GridInstâncias/correlate/$i.gridcorr1.in > grid/correlated/$i.gridcorr1.in
	echo "Done  $i.1 correlated";


	./padronizador < ../../GridInstâncias/anticorrelate/$i.gridanticorr1.in > grid/anticorrelated/$i.gridanticorr1.in
	echo "Done  $i.1 anticorrelated";


done 
echo "DONE grid"
