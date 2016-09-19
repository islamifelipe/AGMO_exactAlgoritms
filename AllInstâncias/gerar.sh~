#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./padronizador < ../../Instâncias/instPatMestrado/concave/$i.1/$i.conc1.in > concave/$i.conc1.in
	echo "Done  $i.1 anticorrelated";

	./padronizador < ../../Instâncias/instPatMestrado/concave/$i.2/$i.conc2.in > concave/$i.conc2.in
	echo "Done  $i.2 anticorrelated";

	./padronizador < ../../Instâncias/instPatMestrado/concave/$i.3/$i.conc3.in > concave/$i.conc3.in
	echo "Done  $i.3 anticorrelated";
done 
echo "DONE correlated"
