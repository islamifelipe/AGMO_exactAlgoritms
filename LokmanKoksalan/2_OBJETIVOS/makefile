
all: spanningTreePM.o  novoAlgo 

	
spanningTreePM.o : spanningTreePM.cpp
	g++ -lpthread -g -c  spanningTreePM.cpp

		
novoAlgo : spanningTreePM.o  
	g++ -lpthread -m64 -g  -lm -o novoAlgo_2 spanningTreePM.o -Llib/ -lgurobi_c++ -lgurobi65 -lm

