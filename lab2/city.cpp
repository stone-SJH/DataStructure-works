#include <iostream>
#include <fstream>
using namespace std;
#define ave 2.3
class Mycity{
public:
  int **map;
  int **virtual_map;
  int *ans;
  int max_score;
  int row;
  int col;
  
  Mycity(int r,int c);
  void process(int p);
  void check();
  void transmap(int & row_t,int & col_t);
  bool test(int p);
  bool valid(int p);
};

Mycity::Mycity(int r,int c){
  row = r;
  col = c;
  max_score = 1;
  map = new int*[r];
  for (int i = 0; i < r; i++)
    map[i] = new int [c];
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      map[i][j] = 1;
  virtual_map = new int * [row+2];
  for (int i = 0; i <= row+1; i++)
    virtual_map[i] = new int [col+2];
  for (int i = 0; i <= row+1; i++)
    for (int j = 0; j <= col+1; j++)
      virtual_map[i][j] = 0;
  ans = new int [r*c];
  for (int i = 0; i < r*c; i++)
      ans [i] = 1;
}
bool Mycity::valid(int p){
  int row_t = p / col + 1;
  int col_t = p % col + 1;
  if (virtual_map[row_t][col_t] == 1) return true;
  if (virtual_map[row_t][col_t] == 2)
    if (virtual_map[row_t-1][col_t] == 1 || virtual_map[row_t+1][col_t] == 1 || virtual_map[row_t][col_t-1] == 1 || virtual_map[row_t][col_t+1] == 1) return true;
  if (virtual_map[row_t][col_t] == 3){
    if (virtual_map[row_t-1][col_t] == 2 && (virtual_map[row_t+1][col_t] == 1 || virtual_map[row_t][col_t-1] == 1 || virtual_map[row_t][col_t+1] == 1)) return true;
    if (virtual_map[row_t+1][col_t] == 2 && (virtual_map[row_t-1][col_t] == 1 || virtual_map[row_t][col_t-1] == 1 || virtual_map[row_t][col_t+1] == 1)) return true;
    if (virtual_map[row_t][col_t-1] == 2 && (virtual_map[row_t+1][col_t] == 1 || virtual_map[row_t-1][col_t] == 1 || virtual_map[row_t][col_t+1] == 1)) return true;
    if (virtual_map[row_t][col_t+1] == 2 && (virtual_map[row_t+1][col_t] == 1 || virtual_map[row_t][col_t-1] == 1 || virtual_map[row_t-1][col_t] == 1)) return true;
  }
  return false;
}
void Mycity::check(){
  bool flag = true;
  if (!valid((row-2)*col+col-1)) flag = false;
  for (int i = 0 ; i < col; i++){
    if (!valid((row-1)*col+i))flag = false;
  }
  if (flag){
    int score = 0;
    for (int i = 0; i < row; i++)
      for (int j = 0; j < col; j++)
        score += map[i][j];
    if (score > max_score){
      int pos = 0;
      max_score = score;
      for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++){
           ans[pos] = map[i][j];
           pos++;
        }
    }
  }
}
bool Mycity::test(int p){
  int row_t = p / col;
  int col_t = p % col;
  int test_score = 0;
  for (int i = 0; i < row_t; i++)
    for (int j = 0; j < col; j++)
      test_score += map[i][j];
  for (int i = 0; i <= col_t; i++)
      test_score += map[row_t][i];
  test_score += (col * row - p - 1) * ave;
  if (test_score < max_score) return false;
  return true;
}
void Mycity::transmap(int & row_t,int & col_t){
     int **trans_map;
     int r,c;
     int count = 0;
     for(int i = 0; i < row_t; i++)
       for (int j = 0; j < col_t; j++){
         map[i][j] = ans[count];
         count++;
     }
     trans_map = new int*[col_t];
     for (int i = 0; i < col_t; i++)
       trans_map[i] = new int [row_t];
     for (int i = 0; i < row_t; i++)
       for(int j = 0; j < col_t; j++)
         trans_map[j][i] = map[i][j];
     int temp = row_t;
     row_t = col_t;
     col_t = temp;
     int pos = 0;
     for (int i = 0; i < row_t; i++)
       for (int j = 0; j < col_t; j++){
        ans[pos] = trans_map[i][j];
        pos++;
       }
}
void Mycity::process(int p){
  for (int i = 3; i >= 1; i--){
     int row_t = p / col;
     int col_t = p % col;
     map[row_t][col_t] = i;
     virtual_map[row_t+1][col_t+1] = i;
     if (max_score != 0)
       if (!test(p)) return;
     if (p >= col && p != row * col - 1){
       if (valid(p-col)){process(p+1);}
     }
     else if (p != row * col - 1) {process(p+1);}
     if (p == row * col - 1) {check();}
  }
} 
int main(int argc, char *argv[]){
   ifstream ifs(argv[1]);
   ofstream ofs(argv[2]);
   int total;
   ifs>>total;
   for (int i = 0; i < total; i++){  
     int row_t;
     int col_t;
     ifs>>row_t>>col_t;
     ofs<<row_t<<"*"<<col_t<<endl;
     int trans = false;
     if (col_t > row_t){
       int temp = row_t;
       row_t = col_t;
       col_t = temp;
       trans = true;
     }
     Mycity c(row_t,col_t);
     c.process(0);
     if (trans){
       c.transmap(row_t,col_t);
     } 
     ofs/*<<"max score:"*/<<c.max_score<<endl;
     for (int i = 0; i < row_t * col_t; i++){
       ofs<< c.ans[i]<<" ";
       if ((i+1) % col_t == 0) ofs<<endl;
     }
     //ofs<<"--------------------------------------------------------------------"<<endl;
   }
   return 0;
}
