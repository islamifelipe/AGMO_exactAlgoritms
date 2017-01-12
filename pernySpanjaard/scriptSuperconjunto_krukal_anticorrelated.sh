#USado para testar o kruksal para gerar superconjunto
#!/bin/bash

./kruskal < InstânciasRelacaoPareto/anticorrelated/10.anticorr3.in > KruskalLike_Pareto/Supercojunto/NOVA23/Anticorrelate/10.anticorr3.out

./kruskal < InstânciasRelacaoPareto/anticorrelated/12.anticorr3.in > KruskalLike_Pareto/Supercojunto/NOVA23/Anticorrelate/12.anticorr3.out
	
for i in 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./kruskal < InstânciasRelacaoPareto/anticorrelated/$i.anticorr1.in > KruskalLike_Pareto/Supercojunto/NOVA23/Anticorrelate/$i.anticorr1.out
	
	./kruskal < InstânciasRelacaoPareto/anticorrelated/$i.anticorr2.in > KruskalLike_Pareto/Supercojunto/NOVA23/Anticorrelate/$i.anticorr2.out
	
	./kruskal < InstânciasRelacaoPareto/anticorrelated/$i.anticorr3.in > KruskalLike_Pareto/Supercojunto/NOVA23/Anticorrelate/$i.anticorr3.out
	
done 
echo "DONE anticorrelated"
