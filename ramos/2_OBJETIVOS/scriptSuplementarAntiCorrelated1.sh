#!/bin/bash

for i in 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./ramos < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr1.in > Testes/LAE/suplementar/completo/anticorrelate/$i.anticorr1.out
	echo "Done  $i.1";
	./ramos < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr2.in > Testes/LAE/suplementar/completo/anticorrelate/$i.anticorr2.out
	echo "Done  $i.2";
	./ramos < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr3.in > Testes/LAE/suplementar/completo/anticorrelate/$i.anticorr3.out
	echo "Done  $i.3";
done 
echo "DONE anticorrelated"
