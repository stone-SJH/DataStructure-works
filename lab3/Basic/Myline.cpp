//f1303703 5130379072
//shi jiahao
#include "Myline.h"

Myline::Myline(Mylist & dcode, string line, bool run, int run_line){
	to_record = 0;
	to_quit = 0;
	to_list = 0;
	to_clear = 0;
	to_let = 0;
	to_goto = 0;
	expression_on = 0;
	to_print = 0;
	to_input = 0;
	error_flag = false;
	run_mode = run;
	run_line_ptr = run_line;
	next_line = "";
	expression = "";
	left_expression = "";
	right_expression = "";
	//初始化变量
	scanner = new TokenScanner(line);
	scanner->ignoreWhitespace();
	parseline(line, dcode);
	if (!to_record) checktail();
	//调用分析
	expression += '=';
	//下面开始执行命令
	if (expression_on) result = eval(expression, dcode);
	if (to_record) dcode.insert(this_key,this_value);
	if (to_list) dcode.list();
	if (to_clear) dcode.clear();
	if (to_print && !error_flag) cout<<result<<endl;
	if (to_let && !error_flag){
		bool key_flag = true;
		for(int i = 0; i < max_key; i++){
			if (parseletter(target) == keyname[i]){
				key_flag = false;
				break;
			}
		}
		if (key_flag) dcode.symbol_insert(target,result);
		else {
			error_syntax_error();
			error_flag = true;
		}
	} 
	if (to_input){
		bool key_flag = true;
		for(int i = 0; i < max_key; i++){
			if (parseletter(target) == keyname[i]){
				key_flag = false;
				break;
			}
		}
		if (key_flag){
		 	while(to_input){
		 		input();
		 		if (!error_flag){ 
 					dcode.symbol_insert(target,target_result);
		 			to_input = 0;
		 		}
			 }
		}
		else{
			error_syntax_error();
			error_flag = true;
		}
	}
	while (run_mode){
		next_line = "";
		if (to_goto){
			if (dcode.codes.containsKey(run_line_ptr))
				next_line = dcode.codes.get(run_line_ptr);
			else{
				error_line_number();
				break;
			}
		}
		else foreach( int n in dcode.codes)
			if ( n > run_line_ptr){
				run_line_ptr = n;
				next_line = dcode.codes.get(run_line_ptr);
				break;
			}
		if (next_line != ""){
        		to_record = 0;
			to_quit = 0;
			to_list = 0;
			to_clear = 0;
			to_let = 0;
			to_goto = 0;
			expression_on = 0;
			to_print = 0;
			to_input = 0;
			error_flag = false;
			expression = "";
			left_expression = "";
			right_expression = "";
			parseline(next_line, dcode);
			expression += '=';
			if (expression_on) result = eval(expression, dcode);
			if (to_record) dcode.insert(this_key,this_value);
			if (to_list) dcode.list();
			if (to_clear) dcode.clear();
			if (to_print && !error_flag) cout<<result<<endl;
			if (to_let && !error_flag) dcode.symbol_insert(target,result);
			if (to_input){
				 while(to_input){
				 	input();
				 	if (!error_flag){ 
 						dcode.symbol_insert(target,target_result);
				 		to_input = 0;
				 	}
				 }
			}
		}			
		else run_mode = false;
	}
}

string Myline::parseletter(string s){
	string temp = s;
	for (int i = 0; i < s.length(); i++){
		if (s[i] <= 'Z' && s[i] >= 'A')
			temp[i] = temp[i] + 'a' - 'A';
	}
	return temp;
}	
void Myline::checktail(){
	string token = scanner->nextToken();
	if (token != ""){
		error_syntax_error();
		error_flag = true;
		to_quit = 0;
		to_list = 0;
		to_clear = 0;
		to_let = 0;
		to_goto = 0;
		to_print = 0;
		to_input = 0;	
	}
}
void Myline::parseline(string line, Mylist dcode){
	if (run_mode){
		scanner = new TokenScanner(line);
		scanner->ignoreWhitespace();
	}
	string token = scanner->nextToken();
	while (token != ""){
		if ((!run_mode) && (!expression_on) && (token[0] >= '0' && token[0] <= '9')){
			this_key = atoi(token.c_str());
			if (scanner->nextToken() == "") this_value = "";		
			else this_value = line;
			to_record = 1;
			return;
		}
		if (run_mode && (!expression_on) && (token[0] >= '0' && token[0] <= '9')){
			token = scanner->nextToken();
			continue;
		}
		if (parseletter(token) == "quit"){
			to_quit = 1;
			return;
		}
		if (parseletter(token) == "list"){
			to_list = 1;
			return;
		}
		if (parseletter(token) == "clear"){
			to_clear = 1;
			return;
		}		
		if (expression_on){
			expression += parseletter(token);
		}
		if (parseletter(token) == "print"){
			expression_on = 1;
			to_print = 1;
		}
		if (parseletter(token) == "let"){
			to_let = 1;
			target = parseletter(scanner->nextToken());
			scanner->nextToken();
			expression_on = 1;
		}
		if (parseletter(token) == "input"){
			to_input = 1;
			target = parseletter(scanner->nextToken());
			return;
		}
		if (parseletter(token) == "run"){
			run_mode = true;
			return;
		}
		if (run_mode && parseletter(token) == "rem"){
			return;
		}
		if (run_mode && parseletter(token) == "end"){
			run_mode = false;
			return;
		}
		if (run_mode && parseletter(token) == "goto"){
			token = scanner->nextToken();
			to_goto = 1;
			run_line_ptr = atoi(token.c_str());
			return;
		}
		if (run_mode && parseletter(token) == "if"){
			token = scanner->nextToken();
			while (token != ">" && token != "<" && token != "="){
				left_expression += token;
				token = scanner->nextToken();
			}
			func = token[0];
			token = scanner->nextToken();
			while (parseletter(token) != "then"){
				right_expression += token;
				token = scanner->nextToken();
			}
			left_expression += "=";
			right_expression += "=";
			int left_result = eval(left_expression, dcode);
			int right_result = eval (right_expression, dcode);
			bool jump = false;			
			switch (func){
			case '>':
				if (left_result > right_result) jump = true;
				break;
			case '<':
				if (left_result < right_result) jump = true;
				break;
			case '=':
				if (left_result == right_result) jump = true;
				break;
			default:
				break;
			}
			token = scanner->nextToken();
			if(jump){
				to_goto = 1;
				run_line_ptr = atoi(token.c_str());
			}
			return;
		}
		if (!expression_on){
			error_syntax_error();
			error_flag = true;
			return;
		}	
		token = scanner->nextToken();
	}
}
int Myline::fun(char x)
{
    switch(x)
    {
        case '+' :
        case '-' :return 1;
        case '*' :
        case '/' :return 2;
        case '(' :return 0;
        default  :return -1;
    }
}

int Myline::js(int x,int y,char z)
{
    switch(z)
    {
        case '+':return y+x;
        case '-':return y-x;
        case '*':return y*x;
        default :if (x!=0) return y/x;
    }
}

int Myline::eval(string a, Mylist dcode){
        char b[256];
	char c[256];
        stack <char> s1;
        stack <int> s2;
        float d;
        s1.push('#');
        int j=0;
        int m=a.length()-1;
        for(int i = 0; i < m; i++)
        {
            if(a[i] >= 'a' && a[i] <= 'z'){
		memset(c,0,sizeof(c));
		int p = 0;
		while (a[i] >= 'a' && a[i] <= 'z')
		{
		    b[j++] = a[i];
		    c[p++] = a[i++];
		}
		string str(c);
		if (dcode.symbols.containsKey(str)){
		    int num = dcode.symbols.get(str);
		    s2.push(num);
		    i--;
		}
		else{
		     error_variable_not_defined();
		     error_flag = 1;
		     return 0;
		}
	    }
	    else if(a[i] >= '0' && a[i] <= '9' || a[i] == '.')
            {
                memset(c,0,sizeof(c));
                int p = 0;
                while(a[i] >= '0' && a[i] <= '9' || a[i] == '.')
                {
                    b[j++] = a[i];
                    c[p++] = a[i++];
                }
                int num = atof(c);
                s2.push(num);
                i--;
            }

            else if(a[i] == '(')  s1.push(a[i]);

            else if(a[i] == ')')
            {
                while(s1.top() != '(')
                {
                    b[j++] = s1.top();
                    int x = s2.top();
		    s2.pop();
                    int y = s2.top();
		    s2.pop();
		    if (x == 0 && b[j-1] == '/'){
			error_divide_by_zero();
			error_flag = true;
			return 0;
	 	    }
                    x = js(x,y,b[j-1]);
                    s2.push(x);
                    s1.pop();
                }
                s1.pop();
            }

            else 
            {
                while(fun(s1.top()) >= fun(a[i]))
                {
                    b[j++] = s1.top();
                    int x = s2.top();
		    s2.pop();
                    int y = s2.top();
		    s2.pop();
		    if (x == 0 && b[j-1] == '/'){
			error_divide_by_zero();
			error_flag = true;
			return 0;
	 	    }
                    x=js(x,y,b[j-1]);
                    s2.push(x);
                    s1.pop();
                }
                s1.push(a[i]);
            }
        }

        while(s1.top() != '#')
        {
            b[j++] = s1.top();
            int x = s2.top();
	    s2.pop();
            int y = s2.top();
            s2.pop();
            if (x == 0 && b[j-1] == '/'){
		error_divide_by_zero();
		error_flag = true;
		return 0;
	    }
            x = js(x,y,b[j-1]);
            s2.push(x);
            s1.pop();
        }
	int result = s2.top();
	s2.pop();
	return result;
}
void Myline::error_divide_by_zero(){
	cout<<"DIVIDE BY ZERO"<<endl;
}
void Myline::error_variable_not_defined(){
	cout<<"VARIABLE NOT DEFINED"<<endl;
}
void Myline::error_syntax_error(){
	if (!error_flag) cout<<"SYNTAX ERROR"<<endl;
}
void Myline::error_invalid_number(){
	cout<<"INVALID NUMBER"<<endl;
}
void Myline::error_line_number(){
        cout<<"LINE NUMBER ERROR"<<endl;
}
void Myline::input(){
	cout<<" ? ";
	string str;
	getline(cin,str);
	error_flag = 0;	
	for(int i = 0; i < str.length(); i++){
		if ((str[i] < '0' || str[i] > '9') && str[i] != '-'){
			error_flag = 1;
			error_invalid_number();
			return;
		}
	}
	target_result = atoi(str.c_str());
	return;
}
			
	
