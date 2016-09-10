#USado para testar o kruksal e o prim para gerar apenas uma unica solucao
#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./main_prim_pareto < InstânciasRelacaoPareto/correlated/$i.corr1.in > PrimLike_Pareto/SolucaoUnisca/correlated/$i.corr_par.out
	echo "Done  $i";
done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./main_prim_pareto < InstânciasRelacaoPareto/anticorrelated/$i.anticorr1.in > PrimLike_Pareto/SolucaoUnisca/anticorrelated/$i.anticorr_par.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	
	./main_prim_pareto < InstânciasRelacaoPareto/concave/$i.conc1.in > PrimLike_Pareto/SolucaoUnisca/concave/$i.conc_par.out
	
	echo "Done  $i";
done 
echo "DONE concave"


for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	./main_prim_pareto < InstânciasRelacaoPareto/Grid/Correlated/$i.gridcorr1.in > PrimLike_Pareto/SolucaoUnisca/Grid/Correlated/$i.gridcorr1_par.out
	echo "Done  $i";
done 
echo "DONE correlated"


for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	./main_prim_pareto < InstânciasRelacaoPareto/Grid/Anticorrelated/$i.gridanticorr1.in > PrimLike_Pareto/SolucaoUnisca/Grid/Anticorrelated/$i.gridanticorr1_par.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"


#USado para testar o kruksal e o Kruskal para gerar apenas uma unica solucao
#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./main_kruskal_pareto < InstânciasRelacaoPareto/correlated/$i.corr1.in > KruskalLike_Pareto/SolucaoUnisca/correlated/$i.corr_par.out
	echo "Done  $i";
done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./main_kruskal_pareto < InstânciasRelacaoPareto/anticorrelated/$i.anticorr1.in > KruskalLike_Pareto/SolucaoUnisca/anticorrelated/$i.anticorr_par.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	
	./main_kruskal_pareto < InstânciasRelacaoPareto/concave/$i.conc1.in > KruskalLike_Pareto/SolucaoUnisca/concave/$i.conc_par.out
	
	echo "Done  $i";
done 
echo "DONE concave"


for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	./main_kruskal_pareto < InstânciasRelacaoPareto/Grid/Correlated/$i.gridcorr1.in > KruskalLike_Pareto/SolucaoUnisca/Grid/Correlated/$i.gridcorr1_par.out
	echo "Done  $i";
done 
echo "DONE correlated"


for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	./main_kruskal_pareto < InstânciasRelacaoPareto/Grid/Anticorrelated/$i.gridanticorr1.in > KruskalLike_Pareto/SolucaoUnisca/Grid/Anticorrelated/$i.gridanticorr1_par.out
	echo "Done  $i";
done 
echo "DONE anticorrelated"



