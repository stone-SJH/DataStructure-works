const int MAXSIZE = 200;
const int x = 32;
const int r = 10;
const int DELKEY = -1;
const int success = 0;
const int overflow = 1;
const int underflow = 2;

typedef int tableEntry;
using namespace std;

class HashTable{
  tableEntry elem[MAXSIZE];
  int flag[MAXSIZE];
  int count;
  int Hash(tableEntry k);
public:
  HashTable();
  int Search(tableEntry k,int &p);
  bool Insert(tableEntry k,int i);
};
