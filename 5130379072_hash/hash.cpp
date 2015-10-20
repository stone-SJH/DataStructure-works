#include "hash.h"

HashTable::HashTable(){
  count = 0;
  for (int i = 0; i < MAXSIZE; i++){
     flag[i] = 0;
  }
}

int HashTable::Hash(tableEntry k){
  int kk = k;
  return kk;
}

bool HashTable::Insert(tableEntry k,int i){
  int pos;
  if (!Search(k,pos)){
     elem[pos] = k;
     flag[pos] = i;
     count++;
     return 1;
  }
}

int HashTable::Search(tableEntry k,int &p){
  p = Hash(k);
  if (flag[p] == 0) return 0;
  else return flag[p];
}

