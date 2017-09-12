#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50 100 200 300 400 500 600 700 800 900 1000
do
	.././generatorGeral $i 0.2 10 > correlate/$i.corr1.in

	sleep 1 #como a semete do gerador aleatrorio usa o tempo, é preciso colocar o shell pra dormir pelo menos um segundo, de modo a nao gerar o mesmo time para a semente

	.././generatorGeral $i -0.2 10 > anticorrelate/$i.anticorr1.in

	sleep 1

	.././generatorGeral $i 0.5 10 > correlate/$i.corr2.in

	sleep 1

	.././generatorGeral $i -0.5 10 > anticorrelate/$i.anticorr2.in

	sleep 1

	.././generatorGeral $i 0.85 10 > correlate/$i.corr3.in

	sleep 1
	.././generatorGeral $i -0.85 10 > anticorrelate/$i.anticorr3.in

	sleep 1

done
