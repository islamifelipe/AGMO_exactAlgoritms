#!/bin/bash
#5 7 10 12 15 17 20 22 25 27
for i in 30 33 35 38 40 42 45 48 50
do
	.././ranking <  ../Instâncias/completo/correlated/$i.corr1.in > completo/correlated/$i.corr1.out
	echo "Done  correlated $i.1";

done 


for i in 25 27 30 33 35 38 40 42 45 48 50
do
	

	.././ranking <  ../Instâncias/completo/anticorrelated/$i.anticorr1.in > completo/anticorrelated/$i.anticorr1.out
	echo "Done  anticorrelated $i.1";


done 



for i in 27 30 33 35 38 40 42 45 48 50
do
	


	.././ranking <  ../Instâncias/completo/concave/$i.conc1.in > completo/concave/$i.conc1.out
	echo "Done  concave $i.1";

done 
echo "DONE completo"



#for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
#do
#	.././ranking <  ../Instâncias/grid/correlated/$i.gridcorr1.in > grid/correlated/$i.gridcorr1.out
#	echo "Done  correlated $i.1";

#	.././ranking <  ../Instâncias/grid/anticorrelated/$i.gridanticorr1.in > grid/anticorrelated/$i.gridanticorr1.out
#	echo "Done  anticorrelated $i.1";


#done 
#echo "DONE grid"

