#!/bin/bash


#for i in 5 7 10 12 15 17 
#do
#	./ramos < ../Instâncias/correlated/$i.1/$i.corr1.in > Testes/correlated/$i.corr_NOVO1.out
#	echo "Done  $i";
#done 
#echo "DONE correlated"

#for i in 5 7 10 12 
#do
#	./ramos < ../Instâncias/anticorrelated/$i.1/$i.anticorr1.in > Testes/anticorrelated/$i.anticorr_NOVO1.out
#	echo "Done  $i";
#done 
#echo "DONE anticorrelated"

for i in 5 7 10 12 15 17 
do
	./ramos < ../Instâncias/concave/$i.1/$i.conc1.in > Testes/concave/$i.conc_NOVO1.out
	echo "Done  $i";
done 
echo "DONE concave"

for i in 15 17  
do
	./ramos < ../Instâncias/anticorrelated/$i.1/$i.anticorr1.in > Testes/anticorrelated/$i.anticorr_NOVO1.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"