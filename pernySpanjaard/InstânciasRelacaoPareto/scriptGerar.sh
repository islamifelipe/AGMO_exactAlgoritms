#!/bin/bash

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do
	.././dominanceRelation < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr2.in > correlated/$i.corr2.in
	echo "Done  $i.2";

	.././dominanceRelation < ../../AllInstâncias/2_OBJETIVOS/correlated/$i.corr3.in > correlated/$i.corr3.in
	echo "Done  $i.3";
done 
echo "DONE correlated"

for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do

	.././dominanceRelation < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr2.in > anticorrelated/$i.anticorr2.in
	echo "Done  $i.2";

	.././dominanceRelation < ../../AllInstâncias/2_OBJETIVOS/anticorrelated/$i.anticorr3.in > anticorrelated/$i.anticorr3.in
	echo "Done  $i.3";

done 
echo "DONE anticorrelated"


for i in 5 7 10 12 15 17 20 22 25 27 30 33 35 38 40 42 45 48 50
do

	.././dominanceRelation < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc2.in > concave/$i.conc2.in
	echo "Done  $i.2";

	.././dominanceRelation < ../../AllInstâncias/2_OBJETIVOS/concave/$i.conc3.in > concave/$i.conc3.in
	echo "Done  $i.3";

done 
echo "DONE concave"
