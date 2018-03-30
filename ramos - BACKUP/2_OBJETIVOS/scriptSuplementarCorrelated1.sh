#!/bin/bash

for i in 25 27 30 33 35 38 40 42 45 48 50
do
	./ramos < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr1.in > Testes/LAE/suplementar/completo/correlate/$i.corr1.out
	echo "Done  $i.1";
	./ramos < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr2.in > Testes/LAE/suplementar/completo/correlate/$i.corr2.out
	echo "Done  $i.2";
	./ramos < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr3.in > Testes/LAE/suplementar/completo/correlate/$i.corr3.out
	echo "Done  $i.3";
done 
echo "DONE correlated"
