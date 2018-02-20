#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <vector>
#include <stack>   
#include <utility>      // std::pair
#include <sys/times.h>
#include <unistd.h>
using namespace std;

int main(){

	int n;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	vector<int*> matriz;	
	//cin>>n; // quantidade de vértices do grafo;
	//Grafo my_grafo(n);
	// contruir o grafo
	//for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
	//	my_grafo.addVertice(i);
	//}
	while (cin>>origem){
		int * linha = new int[4];
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		linha[0] = origem; 
		linha[1] = destino;
		linha[2] =peso1;
		linha[3] = peso2;
		matriz.push_back(linha);
		//my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}

	int nA = id; // quantidade de arestas do grafo	
	cout<<destino+1<<endl;
	for (int i=0;i<matriz.size();i++){
		cout<<matriz[i][0]<<" "<<matriz[i][1]<<" "<<matriz[i][2]<<" "<<matriz[i][3]<<endl;
	}
	//my_grafo.gerarArestasPtr();

}
