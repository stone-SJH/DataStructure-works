//f1303703 5130379072
//shi jiahao
#ifndef _myline_
#define _myline_
#include "Mylist.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stack>
#include <vector>
using namespace std;
#define max_key 13//保留关键字的数量
class Myline{
private:
	int fun(char x);//计算辅助函数
	int js(int x, int y, char z);//计算辅助函数 
	string keyname[max_key] = {"if", "then", "goto", "list", "let", "rem", "print", "input", "end", "run", "quit", "clear", "help"};
	//存储保留关键字
public:
	int to_record;
	int to_quit;
	int to_list;
	int to_clear;
	int to_goto;
	int to_let;
	int to_input;
	int to_print;//状态变量 在检测到关键字的时候修改并启动执行
	bool run_mode;//在接受指令run的时候打开，开始执行Mylist类中codes（map）中的代码（区别与非run状态是执行命令行中的代码）
	int this_key;
	int expression_on;
	int run_line_ptr;//run mode下由于stanford库中map无迭代器，使用这个变量作为指针指向当前执行代码位置
	string next_line;//run mode下 下一行命令
	bool error_flag;//该行出现错误是将其设置为true
	string target;//变量表symbol中的变量名
	int target_result;//symbol中的变量值
	string expression;//读取的表达式，例如Let、print、if then语句等需要的
	string this_value;
	int result;
	string left_expression;
	string right_expression;//以上三个为if then语句所需的变量
	char func;
	TokenScanner *scanner;//使用stanford库中的tokenscanner来进行语句分析
	
	Myline(Mylist &dcode, string line, bool run, int run_line);//构造函数，在其中进行处理
	void parseline(string line, Mylist dcode);//核心函数，对读入的命令进行分析处理
	string parseletter(string str);//将大小写混合/大写统一转化为小写
	int eval(string expression, Mylist dcode);//计算表达式的值
	void input();//输入函数，检测输入是否合法
	void checktail();//检测在命令输入完毕后是否有其他非法输入
	
	void error_divide_by_zero();
	void error_variable_not_defined();
	void error_invalid_number();
	void error_line_number();
	void error_syntax_error();//以上五个是错误处理函数
};
#endif

