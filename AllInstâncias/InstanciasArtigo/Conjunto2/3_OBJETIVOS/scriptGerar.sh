#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	./generator $i 0.2 > correlated/$i.corr1.in

	sleep 1 #como a semete do gerador aleatrorio usa o tempo, é preciso colocar o shell pra dormir pelo menos um segundo, de modo a nao gerar o mesmo time para a semente

	./generator $i -0.2 > anticorrelated/$i.anticorr1.in

	sleep 1

	./generator $i 0.5 > correlated/$i.corr2.in

	sleep 1

	./generator $i -0.5 > anticorrelated/$i.anticorr2.in

	sleep 1

	./generator $i 0.85 > correlated/$i.corr3.in

	sleep 1
	./generator $i -0.85 > anticorrelated/$i.anticorr3.in

	sleep 1

done
