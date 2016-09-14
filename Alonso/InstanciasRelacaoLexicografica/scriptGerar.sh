#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	.././lexicographicRelation < ../../Instâncias/correlated/$i.1/$i.corr1.in > completo/correlated/$i.corr1.in
	echo "Done  $i";
done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	.././lexicographicRelation < ../../Instâncias/anticorrelated/$i.1/$i.anticorr1.in > completo/anticorrelated/$i.anticorr1.in
	echo "Done  $i";
done 
echo "DONE anticorrelated"


for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	.././lexicographicRelation < ../../Instâncias/concave/$i.1/$i.conc1.in > completo/concave/$i.conc1.in
	echo "Done  $i";
done 
echo "DONE concave"

for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	.././dominanceRelation < ../../GridInstâncias/correlate/$i.gridcorr1.in > grid/Correlated/$i.gridcorr1.in
	echo "Done  $i";
done 
echo "DONE correlated"


for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	.././dominanceRelation < ../../GridInstâncias/anticorrelate/$i.gridanticorr1.in > grid/Anticorrelated/$i.gridanticorr1.in
	echo "Done  $i";
done 
echo "DONE anticorrelated"
