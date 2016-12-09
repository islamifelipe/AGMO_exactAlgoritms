#!/bin/bash


for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./pugliese < ../../AllInstâncias/3_OBJETIVOS/anticorrelate/$i.anticorr3.in > Testes/LAE/completo/anticorrelate/$i.anticorr3.out
	echo "Done  $i.3";
done 
echo "DONE anticorrelate 3"
