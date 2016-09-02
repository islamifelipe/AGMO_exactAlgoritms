#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./steinerRadzik < ../Instâncias/correlated/$i.1/$i.corr1.in > Testes/correlated/$i.corr1_12.out
	echo "Done  $i";
done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./steinerRadzik < ../Instâncias/anticorrelated/$i.1/$i.anticorr1.in > Testes/anticorrelated/$i.anticorr2.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./steinerRadzik < ../Instâncias/concave/$i.1/$i.conc1.in > Testes/concave/$i.conc1.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"
