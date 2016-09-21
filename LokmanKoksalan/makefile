
all: lokmanKoksalan.o  algoritm1 

	
lokmanKoksalan.o : lokmanKoksalan.cpp
	g++ -g -c  lokmanKoksalan.cpp

		
algoritm1 : lokmanKoksalan.o  
	g++ -m64 -g  -lm -o algoritm1 lokmanKoksalan.o -Llib/ -lgurobi_c++ -lgurobi65 -lm

