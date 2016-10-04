/* Este programa gera um arquivo csv a partir de um txt. 
O objetivo é obter uma formataçao dos pesos dispostos horontalmente em duas linhas de modo a plotar a fronteira de pareito posteriormente
*/



#include <iostream>
#include <vector>


using namespace std;

int main(){
	std::vector<int> v1, v2;
	double peso1, peso2;
	char parentese1, parentese2, virgula;

	while (cin>>parentese1){
		cin>>peso1;
		cin>>virgula;
		cin>>peso2;
		cin>>parentese2;
		//cout<<parentese1<<peso1<<virgula<<peso2<<parentese2<<endl;
		v1.push_back(peso1);
		v2.push_back(peso2);
	}

	for (int i=0; i<v1.size(); i++){
		cout<<v1[i]<<";";
	}
	cout<<endl;
	for (int i=0; i<v1.size(); i++){
		cout<<v2[i]<<";";
	}
	return 0;
}