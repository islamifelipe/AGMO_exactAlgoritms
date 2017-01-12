#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./kruskal < InstanciasRelacaoLexicografica/completo/correlated/$i.corr2.in > TestesRelacaoLexicografica/kruskal/LAE/completo/correlated/NOVO23/$i.corr2.out
	./kruskal < InstanciasRelacaoLexicografica/completo/correlated/$i.corr3.in > TestesRelacaoLexicografica/kruskal/LAE/completo/correlated/NOVO23/$i.corr3.out

done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./kruskal < InstanciasRelacaoLexicografica/completo/anticorrelated/$i.anticorr2.in > TestesRelacaoLexicografica/kruskal/LAE/completo/anticorrelated/NOVO23/$i.anticorr2.out
	./kruskal < InstanciasRelacaoLexicografica/completo/anticorrelated/$i.anticorr3.in > TestesRelacaoLexicografica/kruskal/LAE/completo/anticorrelated/NOVO23/$i.anticorr3.out

done 
echo "DONE anticorrelated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./kruskal < InstanciasRelacaoLexicografica/completo/concave/$i.conc2.in > TestesRelacaoLexicografica/kruskal/LAE/completo/concave/NOVO23/$i.conc2.out
	./kruskal < InstanciasRelacaoLexicografica/completo/concave/$i.conc3.in > TestesRelacaoLexicografica/kruskal/LAE/completo/concave/NOVO23/$i.conc3.out

done 
echo "DONE concave"
