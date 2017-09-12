
#include <iostream>
#include <map> 
#include <string>
#include <sys/times.h>
#include <unistd.h>
using namespace std;



int main(){
	string linha;
	int max_x=0, max_y=0, max_z=0,max_w=0;  
	while (cin>>linha){
		//cout<<linha<<endl;
			string x = "", y ="", z="", w="";
			if (linha=="Nao Suportada") return 0;
			if (linha[0]=='('){
				//cout<<linha<<endl;

				int obj = 1;
				for (int i=0; i<linha.length(); i++){
					if (linha[i] == ' ') continue;
					if (obj==1){
						if (linha[i]!='(' && linha[i]!=','){
							x = x+linha[i];
						} else if (linha[i]==','){
							obj=2;
							if (atoi(x.c_str())>max_x) max_x= atoi(x.c_str());
							cout<<x<<" ";
							x = "";
						}
					}else if (obj==2){
						if (linha[i]!=')'){
							y = y+linha[i];
						} else{
							obj=1;
							if (atoi(y.c_str())>max_y) max_y= atoi(y.c_str());
							
							cout<<y<<endl;
							y = "";
						} 
					} // else if (obj==3){
					// 	if (linha[i]!=','){
					// 		z = z+linha[i];
					// 	} else{
					// 		obj=4;
					// 		if (atoi(z.c_str())>max_z) max_z= atoi(z.c_str());
							
					// 		cout<<z<<" ";
					// 		z = "";
					// 	} 
					// } else {
					// 	if (linha[i]!=')'){
					// 		w = w+linha[i];
					// 	} else{
					// 		obj=1;
					// 		if (atoi(w.c_str())>max_w) max_w= atoi(w.c_str());
							
					// 		cout<<w<<endl;
					// 		w = "";
					// 	} 
					// }
				}
			}

	}

	//cout<<"======="<<max_x<<" "<<max_y<<" "<<max_z<<" "<<max_w<<"======="<<endl;
	//cout<<"======="<<max_x<<" "<<max_y<<" "<< "======="<<endl;

}