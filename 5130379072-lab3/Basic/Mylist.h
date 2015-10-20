//f1303703 5130379072
//shi jiahao
#ifndef _mylist_
#define _mylist_
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/map.h"
#include "../StanfordCPPLib/foreach.h"
#include <string>
class Mylist{
public:
	Map <int,string> codes;//代码表
	Map <string,int> symbols;//变量表

	//Mylist();
	void clear();//清除
	void insert(int key,string value);//插入代码表
	void list();//输出代码表中的内容
	void symbol_insert(string key,int value);//插入变量表
};
#endif
