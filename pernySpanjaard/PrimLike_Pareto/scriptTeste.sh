#!/bin/bash

#for i in 5 7 10 12 15 
#do
#	.././prim < ../InstânciasRelacaoPareto/correlated/$i.corr1.in > Supercojunto/Correlate/$i.corr_par.out
#	echo "Done  $i";
#done 
#echo "DONE correlated"

#for i in 5 7 10 12 15 
#do
#	.././prim < ../InstânciasRelacaoPareto/anticorrelated/$i.anticorr1.in > Supercojunto/Anticorrelate/$i.anticorr_par.out
#	echo "Done  $i";
#done 
#echo "DONE anticorrelated"

#for i in 5 7 10 12 15 
#do
#	.././prim < ../InstânciasRelacaoPareto/concave/$i.conc1.in > Supercojunto/Concave/$i.conc_par.out
#	echo "Done  $i";
#done 
#echo "DONE concave"


for i in  30 
do
	.././prim < ../InstânciasRelacaoPareto/concave/$i.conc1.in > Supercojunto/Concave/$i.conc_par.out
	echo "Done  $i concave";
	#.././prim < ../InstânciasRelacaoPareto/correlated/$i.corr1.in > Supercojunto/Correlate/$i.corr_par.out
	#echo "Done  $i Correlated";
	#.././prim < ../InstânciasRelacaoPareto/anticorrelated/$i.anticorr1.in > Supercojunto/Anticorrelate/$i.anticorr_par.out
	#echo "Done  $i Anticorrelated";
done 

#for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
#do
#	.././prim < ../InstânciasRelacaoPareto/Grid/Correlated/$i.gridcorr1.in > Supercojunto/Grid/Correlated/$i.gridcorr1_par.out
#	echo "Done  $i";
#done 
#echo "DONE correlated"


#for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
#do
#	.././prim < ../InstânciasRelacaoPareto/Grid/Anticorrelated/$i.gridanticorr1.in > Supercojunto/Grid/Anticorrelated/$i.gridanticorr1_par.out
#	echo "Done  $i";
#done 
#echo "DONE anticorrelated"
