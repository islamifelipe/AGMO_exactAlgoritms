#!/bin/bash


for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./corley < ../../AllInstâncias/3_OBJETIVOS/correlate/$i.corr1.in > Testes/LAE/completo/correlate/$i.corr1.out
	echo "Done  $i.1";
done 
echo "DONE correlate 1"
