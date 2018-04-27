
i=40
k=3
./ramos < ../../../1\ -\ ARTIGO\ COAP/InstanciasArtigo/Conjunto2/2_OBJETIVOS/concave/"$i".conc"$k".in > ../../../1\ -\ ARTIGO\ COAP/Ramos/Conjunto2/concave/"$i".conc"$k".out 
	
for i in  42 45 48 50
do
	for k in 1 2 3
	do
		./ramos < ../../../1\ -\ ARTIGO\ COAP/InstanciasArtigo/Conjunto2/2_OBJETIVOS/concave/"$i".conc"$k".in > ../../../1\ -\ ARTIGO\ COAP/Ramos/Conjunto2/concave/"$i".conc"$k".out 
	done
done