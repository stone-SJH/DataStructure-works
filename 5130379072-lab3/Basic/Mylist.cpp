//f1303703 5130379072
//shi jiahao
#include "Mylist.h"

void Mylist::insert(int key,string value){
	codes.put(key,value);
}
void Mylist::list(){
	foreach(int n in codes){
		string value = codes.get(n);		
	        if (value !="") cout<<value<<endl;
	}
}
void Mylist::clear(){
	codes.clear();
	symbols.clear();
}
void Mylist::symbol_insert(string key,int value){
	symbols.put(key,value);
}
