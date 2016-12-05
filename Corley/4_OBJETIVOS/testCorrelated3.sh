#!/bin/bash


for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./corley < ../../AllInstâncias/4_OBJETIVOS/correlate/$i.corr3.in > Testes/LAE/completo/correlate/$i.corr3.out
	echo "Done  $i.3";
done 
echo "DONE correlate 3"
