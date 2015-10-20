#include "hash.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

int main(){
  ifstream ifs("hash.in");
  ofstream ofs("hash.out");
  int total;
  ifs >> total;
  int sum = 0;
  while (sum != total-1){
    HashTable ht;
    string str;
    ifs >> str;
    int bucket[200];
    for (int i = 0; i < 200; i++)
       bucket[i] = 0;
    for (int i = 0; i < str.length(); i++){
       int res = str[i];
       int c = i;
       ht.Insert(res,c);
       bucket[res]++;
    }
    int count = 0;
    int result[200];
    for (int i = 0; i < 200; i++){
       if (bucket[i] == 1) {
            result[count] = i;
            count++;
       }
    }
    if (count == 0) {
        ofs<<"None"<<endl;
        continue;
    }
    for(int i = 0; i < str.length(); i++){
        if (bucket[(int)str[i]] == 1){
           ofs<<str[i]<<endl;
           break;
        }
    }
    sum++;
  }
  ofs.close();
}

