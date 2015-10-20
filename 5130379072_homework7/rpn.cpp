#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;
#define max 100
template <class T> class Stack{
public:  
       Stack(){Top=0;}
       T top(){return a[Top];}
       void pop(){Top--;}
       bool empty(){
         if (Top == 0) return true;
          else return false;
       }
       void push(T b){
            Top++;
            a[Top]=b;
       }
private:
         T a[100];
         int Top;
};
 void process(char input[],char output[],int & count,int n){
    Stack <char> parse;
    for(int i = 0; i < n; i++)
     {
           if(input[i] != '+'&&input[i] != '-'&&input[i] != '*'&&input[i] != '/'&&input[i] != '('&&input[i] != ')')
           {
              cout << input[i] <<endl;
              output[count] = input[i];
              count++;
           }
           if(input[i] == '(')
              parse.push(input[i]);
           if(input[i] == '+')
              {
                  if(!parse.empty() && (parse.top() == '*' || parse.top() == '/' || parse.top() == '-' || parse.top() == '+'))
                      { 
                         output[count++] = parse.top();
                         parse.pop();
                         parse.push(input[i]);
                      }
                  else parse.push(input[i]);
              }
            if(input[i] == '-')
            {
                if(!parse.empty() && (parse.top() == '*' || parse.top() == '/' || parse.top() == '+' || parse.top() == '-')){
                    output[count++] = parse.top();
                    parse.pop();
                    parse.push(input[i]);
                }
                else parse.push(input[i]);
            }
            if(input[i]=='*'||input[i]=='/'){
                if(!parse.empty() && (parse.top() == '*' || parse.top() == '/')){
                    output[count++] = parse.top();
                    parse.pop();
                    parse.push(input[i]);
                }
                else parse.push(input[i]);
            }
            if(input[i] == ')')
            {
               while(parse.top()!='('){
                  output[count++] = parse.top();
                  parse.pop();
               }
                  parse.pop();
            }
       }
       while(!parse.empty()){
          output[count++] = parse.top();
          parse.pop();
       }
 }
int main()
 {
     while (true){
       char in[max];
       char a;
       int i = 0;
       while(cin.get(a))
       {
          if (a == '\n')break;
          if (a == ' ')continue;
          if (a == 'q'){
            cin.putback(a);
            string s;
            cin>>s;
            if (s == "quit") return 0;
          }
          if (a >= '0' && a <= '9'){
            cin.putback(a);
            int num;
            cin>>num;
            a = '0' + num;
          }
          in[i++]=a;
       }
       char out[max];
       int n = 0;
       process(in,out,n,i);
       for(int j = 0; j < n; j++){
         if (out[j] > '9' && out[j] < 'a') cout<<out[j]-'0'<<" ";
         else cout<<out[j]<<" ";
       }
       cout<<endl;
    }
  return 0;
}
