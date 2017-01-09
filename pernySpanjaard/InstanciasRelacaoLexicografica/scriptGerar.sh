#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	.././lexicographicRelation < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr2.in > completo/correlated/$i.corr2.in
	echo "Done  $i.2";

	.././lexicographicRelation < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr3.in > completo/correlated/$i.corr3.in
	echo "Done  $i.3";
done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do


	.././lexicographicRelation < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr2.in > completo/anticorrelated/$i.anticorr2.in
	echo "Done  $i.2";

	.././lexicographicRelation < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr3.in > completo/anticorrelated/$i.anticorr3.in
	echo "Done  $i.3";

done 
echo "DONE anticorrelated"


for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do

	.././lexicographicRelation < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc2.in > completo/concave/$i.conc2.in
	echo "Done  $i.2";

	.././lexicographicRelation < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc3.in > completo/concave/$i.conc3.in
	echo "Done  $i.3";

done 
echo "DONE concave"

#for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
#do
#	.././dominanceRelation < ../../GridInstâncias/correlate/$i.gridcorr1.in > grid/Correlated/$i.gridcorr1.in
#	echo "Done  $i";
#done 
#echo "DONE correlated"


#for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
#do
#	.././dominanceRelation < ../../GridInstâncias/anticorrelate/$i.gridanticorr1.in > grid/Anticorrelated/$i.gridanticorr1.in
#	echo "Done  $i";
#done 
#echo "DONE anticorrelated"
