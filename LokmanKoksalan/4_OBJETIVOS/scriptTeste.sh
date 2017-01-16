#!/bin/bash

#CORRELATED 1___  4 OBJETVI
for i in  5 #7 10 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
        ./algoritm1 < ../../AllInstâncias/4_OBJETIVOS/correlate/$i.corr1.in > Testes/LAE/completo/correlated/$i.corr1.out
        ./algoritm1 < ../../AllInstâncias/4_OBJETIVOS/correlate/$i.corr2.in > Testes/LAE/completo/correlated/$i.corr2.out
        ./algoritm1 < ../../AllInstâncias/4_OBJETIVOS/correlate/$i.corr3.in > Testes/LAE/completo/correlated/$i.corr3.out

done
echo "DONE correlated"

#ANTICORRELATED 1___  4 OBJETVI
for i in 5 #7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
        ./algoritm1 < ../../AllInstâncias/4_OBJETIVOS/anticorrelate/$i.anticorr1.in > Testes/LAE/completo/anticorrelated/$i.anticorr1.out
        ./algoritm1 < ../../AllInstâncias/4_OBJETIVOS/anticorrelate/$i.anticorr2.in > Testes/LAE/completo/anticorrelated/$i.anticorr2.out
        ./algoritm1 < ../../AllInstâncias/4_OBJETIVOS/anticorrelate/$i.anticorr3.in > Testes/LAE/completo/anticorrelated/$i.anticorr3.out

done
echo "DONE anticorrelated"