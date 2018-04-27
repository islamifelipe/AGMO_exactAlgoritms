
for i in 6 12 18 20 24 27 30 33 36 38 40 42 44 48 50 52 55 58 60 63 66 68 70 75 77 80 82 86 90 93 96 100
do
	mono bicritrs.exe InstanciasGrid/Conjunto1/correlated/"$i".gridcorr1.in > Paretos/Conjunto1/correlated/"$i".gridcorr1.out
	mono bicritrs.exe InstanciasGrid/Conjunto1/correlated/"$i".gridcorr2.in > Paretos/Conjunto1/correlated/"$i".gridcorr2.out
	mono bicritrs.exe InstanciasGrid/Conjunto1/correlated/"$i".gridcorr3.in > Paretos/Conjunto1/correlated/"$i".gridcorr3.out
	
	mono bicritrs.exe InstanciasGrid/Conjunto2/correlated/"$i".gridcorr1.in > Paretos/Conjunto2/correlated/"$i".gridcorr1.out
	mono bicritrs.exe InstanciasGrid/Conjunto2/correlated/"$i".gridcorr2.in > Paretos/Conjunto2/correlated/"$i".gridcorr2.out
	mono bicritrs.exe InstanciasGrid/Conjunto2/correlated/"$i".gridcorr3.in > Paretos/Conjunto2/correlated/"$i".gridcorr3.out
	
	mono bicritrs.exe InstanciasGrid/Conjunto3/correlated/"$i".gridcorr1.in > Paretos/Conjunto3/correlated/"$i".gridcorr1.out
	mono bicritrs.exe InstanciasGrid/Conjunto3/correlated/"$i".gridcorr2.in > Paretos/Conjunto3/correlated/"$i".gridcorr2.out
	mono bicritrs.exe InstanciasGrid/Conjunto3/correlated/"$i".gridcorr3.in > Paretos/Conjunto3/correlated/"$i".gridcorr3.out
	


	mono bicritrs.exe InstanciasGrid/Conjunto1/anticorrelated/"$i".gridanticorr1.in > Paretos/Conjunto1/anticorrelated/"$i".gridanticorr1.out
	mono bicritrs.exe InstanciasGrid/Conjunto1/anticorrelated/"$i".gridanticorr2.in > Paretos/Conjunto1/anticorrelated/"$i".gridanticorr2.out
	mono bicritrs.exe InstanciasGrid/Conjunto1/anticorrelated/"$i".gridanticorr3.in > Paretos/Conjunto1/anticorrelated/"$i".gridanticorr3.out
	
	mono bicritrs.exe InstanciasGrid/Conjunto2/anticorrelated/"$i".gridanticorr1.in > Paretos/Conjunto2/anticorrelated/"$i".gridanticorr1.out
	mono bicritrs.exe InstanciasGrid/Conjunto2/anticorrelated/"$i".gridanticorr2.in > Paretos/Conjunto2/anticorrelated/"$i".gridanticorr2.out
	mono bicritrs.exe InstanciasGrid/Conjunto2/anticorrelated/"$i".gridanticorr3.in > Paretos/Conjunto2/anticorrelated/"$i".gridanticorr3.out
	
	mono bicritrs.exe InstanciasGrid/Conjunto3/anticorrelated/"$i".gridanticorr1.in > Paretos/Conjunto3/anticorrelated/"$i".gridanticorr1.out
	mono bicritrs.exe InstanciasGrid/Conjunto3/anticorrelated/"$i".gridanticorr2.in > Paretos/Conjunto3/anticorrelated/"$i".gridanticorr2.out
	mono bicritrs.exe InstanciasGrid/Conjunto3/anticorrelated/"$i".gridanticorr3.in > Paretos/Conjunto3/anticorrelated/"$i".gridanticorr3.out
	
done