#!/bin/bash


./ramos < ../../AllInstâncias/2_OBJETIVOS/concave/17.conc2.in > Testes/LAE/suplementar/completo/concave/17.conc2.out

./ramos < ../../AllInstâncias/2_OBJETIVOS/concave/20.conc3.in > Testes/LAE/suplementar/completo/concave/20.conc3.out
	
./ramos < ../../AllInstâncias/2_OBJETIVOS/concave/22.conc2.in > Testes/LAE/suplementar/completo/concave/22.conc2.out

for i in 25 27 30 33 35 38 40 42 45 48 50
do
	./ramos < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc1.in > Testes/LAE/suplementar/completo/concave/$i.conc1.out
	echo "Done  $i.1";
	./ramos < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc2.in > Testes/LAE/suplementar/completo/concave/$i.conc2.out
	echo "Done  $i.2";
	./ramos < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc3.in > Testes/LAE/suplementar/completo/concave/$i.conc3.out
	echo "Done  $i.3";
done 
echo "DONE concave"
