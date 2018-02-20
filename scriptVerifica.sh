
for k in 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50

do
	echo "Verificando $k.1 ..."
	./verificadorPareto SourdSpanjaard/LAE_SS/completo/concave/"$k".conc1.out Conjunto1/concave/"$k".conc1.out 

	echo "Verificando $k.2 ..."

	./verificadorPareto SourdSpanjaard/LAE_SS/completo/concave/"$k".conc2.out Conjunto1/concave/"$k".conc2.out 

	echo "Verificando $k.3 ..."
	./verificadorPareto SourdSpanjaard/LAE_SS/completo/concave/"$k".conc3.out Conjunto1/concave/"$k".conc3.out 

done