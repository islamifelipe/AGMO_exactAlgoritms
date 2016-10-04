/* Este programa gera um arquivo csv a partir de um .out do Perny Sapanjaard 
O objetivo é obter uma formataçao dos pesos dispostos horontalmente em duas linhas de modo a plotar a fronteira de pareito posteriormente
*/



#include <iostream>
#include <vector>


using namespace std;

int main(){
	std::vector<int> v1, v2;
	double peso1, peso2;
	char parentese1, parentese2, virgula, espaco;
	double maxx = -1, maxy = -1, minx = 1000000, miny = 10000000;
	while (cin>>parentese1){
		if (parentese1=='('){
			cin>>peso1;
			cin>>virgula;
			//cin>>espaco;
			cin>>peso2;
			cin>>parentese2;
			//cout<<parentese1<<peso1<<virgula<<peso2<<parentese2<<endl;
			v1.push_back(peso1);
			v2.push_back(peso2);
			if (peso1 > maxx) maxx = peso1;
			if (peso1 < minx) minx = peso1;
			if (peso2 > maxy) maxy = peso2;
			if (peso2 < miny) miny = peso2;
		}
	}

	int aux = 0;
	int cont=1;
	while (aux<v1.size()){
		for (int i=aux; i<v1.size() && i<255*cont; i++){
			cout<<v1[i]<<";";
		}
		cout<<endl;
		for (int i=aux; i<v1.size() && i<255*cont; i++){
			cout<<v2[i]<<";";
		}
		cout<<endl;
		aux+=255;
		cont++;
	}

	cout<<"max x = "<<maxx<<endl;
	cout<<"max y = "<<maxy<<endl;
	cout<<"min x = "<<minx<<endl;
	cout<<"min y = "<<miny<<endl;
	return 0;
}