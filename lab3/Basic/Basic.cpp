//f1303703 5130379072
//shi jiahao
//basic interpreter

#include "Myline.h"
#include "Mylist.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
 Mylist mycode;
 while (true){
 	 string line;
	 getline(cin, line);
	 Myline this_line(mycode, line, false, 0);
	 if (this_line.to_quit == 1) break;
 }
 return 0;
}

