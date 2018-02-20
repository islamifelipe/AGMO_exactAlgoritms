#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>
#include <sys/times.h>
#include <unistd.h>
#include <queue>
#include <algorithm>
#include <fstream>      // std::ifstream 
using namespace std;

inline bool compara (pair<float, float> a, pair<float, float> b){
	return (a.first < b.first || (a.first == b.first && a.second <= b.second));
}

// SS e SR
int main(int argc, char *argv[]){
	char * SSar = argv[1];
	char * SRar = argv[2];
	std::ifstream SS;
    SS.open(SSar, std::ifstream::in);
    std::vector<pair<float, float > > vSS;
    std::ifstream SR;
    SR.open(SRar, std::ifstream::in);
    std::vector<pair<float, float > > vSR;
    string s;
    string partr = "(";
    while (SS>>s){
    	 // cout<<s<<endl;
    	if (s=="optimum"){
    		char ss, virg;
    		while (SS>>ss){
	    		if (ss=='('){
		    		float peso1, peso2;
		    		SS>>peso1;
		    		SS>>virg; // ,
		    		SS>>peso2;
		    		// cout<<peso1<<" "<<peso2<<endl;
		    		vSS.push_back(make_pair(peso1, peso2));
		    	}
		    }
    	}
    }
    cout<<endl;
    char ss;
    while (SR>>ss){
    	if (ss=='('){
			float peso1, peso2;
			SR>>peso1;
			SR>>peso2;
			// cout<<peso1<<" "<<peso2<<endl;
			vSR.push_back(make_pair(peso1, peso2));	
    	}
    }
    if (vSS.size()==vSR.size()){
	    stable_sort (vSS.begin(), vSS.end(),compara);
		stable_sort (vSR.begin(), vSR.end(),compara);
		for (int i=0; i<vSS.size(); i++){
			// for (int j=0; j<vSR.size(); j++){
				if (vSS[i].first == vSR[i].first && vSS[i].second == vSR[i].second);
				else {
					cout<<"("<<vSS[i].first<<" "<<vSS[i].second<<") != ("<< vSR[i].first<<" "<< vSR[i].second<<")"<<endl;
				}
			// }
		}
	} else {
		cout<<"Tamanhos diferentes!!!"<<endl;
	}
	return 0;
}