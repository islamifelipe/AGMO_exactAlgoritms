#!/bin/bash

#for i in 5 7 10 12 15 
#do
#	.././kruskal < ../InstânciasRelacaoPareto/correlated/$i.corr1.in > Supercojunto/Correlate/$i.corr_par1.out
#	echo "Done  $i";
#done 
#echo "DONE correlated"


#for i in 5 7 10 12 
#do
#	.././kruskal < ../InstânciasRelacaoPareto/Anticorrelated/$i.anticorr1.in > Supercojunto/Anticorrelate/$i.anticorr_par1.out
#	echo "Done  $i";
#done 
#echo "DONE anticorrelated"

#for i in 5 7 10 12
#do
#	.././kruskal < ../InstânciasRelacaoPareto/correlated/$i.corr1.in > Supercojunto/Correlate/$i.corr_par1.out
#	echo "Done  $i correlated";
#	.././kruskal < ../InstânciasRelacaoPareto/concave/$i.conc1.in > Supercojunto/Concave/$i.conc_par2.out
#	echo "Done  $i concave";
#	.././kruskal < ../InstânciasRelacaoPareto/Anticorrelated/$i.anticorr1.in > Supercojunto/Anticorrelate/$i.anticorr_par1.out
#	echo "Done  $i Anticorrelated";
#done 


for i in 15  
do
	.././kruskal < ../InstânciasRelacaoPareto/correlated/$i.corr1.in > Supercojunto/Correlate/$i.corr_par1.out
	echo "Done  $i correlated";
	.././kruskal < ../InstânciasRelacaoPareto/concave/$i.conc1.in > Supercojunto/Concave/$i.conc_par2.out
	echo "Done  $i concave";
done 


for i in 17 20  
do
	.././kruskal < ../InstânciasRelacaoPareto/concave/$i.conc1.in > Supercojunto/Concave/$i.conc_par2.out
	echo "Done  $i concave";
done 

for i in 17 20  
do
	.././kruskal < ../InstânciasRelacaoPareto/correlated/$i.corr1.in > Supercojunto/Correlate/$i.corr_par1.out
	echo "Done  $i correlated";
done 
