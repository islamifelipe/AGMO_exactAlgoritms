#!/bin/bash

#6 12 18 20 24 27 30 33 36 38 40 42 44 48 
for i in 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	.././ranking <  ../Instâncias/grid/correlated/$i.gridcorr1.in > grid/correlated/$i.gridcorr1.out
	echo "Done  correlated $i.1";

done 

for i in 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do

	.././ranking <  ../Instâncias/grid/anticorrelated/$i.gridanticorr1.in > grid/anticorrelated/$i.gridanticorr1.out
	echo "Done  anticorrelated $i.1";

done 

echo "DONE grid"

