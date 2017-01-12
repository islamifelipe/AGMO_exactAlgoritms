#USado para testar o kruksal para gerar superconjunto
#!/bin/bash

./kruskal < InstânciasRelacaoPareto/correlated/20.corr1.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/20.corr1.out

./kruskal < InstânciasRelacaoPareto/correlated/22.corr1.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/22.corr1.out

./kruskal < InstânciasRelacaoPareto/correlated/25.corr1.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/25.corr1.out

./kruskal < InstânciasRelacaoPareto/correlated/25.corr2.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/25.corr2.out

./kruskal < InstânciasRelacaoPareto/correlated/27.corr1.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/27.corr1.out

./kruskal < InstânciasRelacaoPareto/correlated/27.corr2.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/27.corr2.out

	
for i in 30 33 35 38 40 42 45 48 50
do
	./kruskal < InstânciasRelacaoPareto/correlated/$i.corr1.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/$i.corr1.out
	
	./kruskal < InstânciasRelacaoPareto/correlated/$i.corr2.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/$i.corr2.out
	
	./kruskal < InstânciasRelacaoPareto/correlated/$i.corr3.in > KruskalLike_Pareto/Supercojunto/NOVA23/Correlate/$i.corr3.out
	
done 
echo "DONE correlated"
