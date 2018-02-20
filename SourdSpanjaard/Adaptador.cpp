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

int main(int argc, char *argv[]){
	int n; 
	cin>>n;
	float o, d, p1, p2;
	while (cin>>o){
		cin>>d;
		cin>>p1;
		cin>>p2;
		cout<<o<<" "<<d<<" "<<p1<<" "<<p2<<endl;
	}
	return 0;	
}