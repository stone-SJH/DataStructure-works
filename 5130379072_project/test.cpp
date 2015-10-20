#include "db.h"
#include <stdlib.h>
#include <time.h>
void test(int nrec, string data_filename, string index_filename){
	int max = nrec * 2;
	DataBase mydb;
	srand((int)time(NULL));
	int num = 0;
	ofstream ofs(data_filename.c_str(), ios::binary);
	for (int i = 0; i < nrec; i++){
		char  c_name[NAMESIZE];
		char c_content[CONTSIZE];
 		int n = i + 1; 
		sprintf(c_name, "name %d", n);
		sprintf(c_content, "content %d, 1.0", n);
		ofs.write((char *)&n, sizeof(int));
		ofs.write(c_name, NAMESIZE);
		ofs.write(c_content, CONTSIZE);
	}
	ofs.close();
	clock_t begin, end;
	begin = clock();
	mydb.db_open(data_filename, index_filename);
	int knum = 0;
	while (knum < nrec){
		int n = rand()%max+1;
		if (mydb.db_find(n)) knum++;
	}
	int count = 0;
	int change_count = 0;
	for (; count < nrec*5; count++){
		//cout<<endl<<count<<endl;		
		while (true){
			int n = rand() % max + 1;
			if (mydb.db_find(n)) break;
		}
		if (count % 37 == 0){
			bool flag = false;
			while (!flag){
				int n = rand()%max + 1;
				flag = mydb.db_delete(n);
			}
		}
		if (count % 11 == 0){
			bool flag = false;
			while (!flag){
				int n = rand()%max + 1;
				char c_name[NAMESIZE];
				char c_content[CONTSIZE];
				sprintf(c_name, "name %d", n);
				sprintf(c_content, "content %d, 2.0", n);
				flag = mydb.db_insert(n, c_name, c_content);
				if (flag) mydb.db_find(n);
			}
		}
		if (count % 17 == 0){
			bool flag = false;
			while (!flag){
				int n = rand() % max + 1;
				char c_name[NAMESIZE];
				char c_content[CONTSIZE];
				sprintf(c_name, "name %d", n);
				if (change_count % 2 == 0) sprintf(c_content, "content %d, 2.0", n);
				else sprintf(c_content, "content %d, 3.0 -- larger", n);
				flag = mydb.db_change(n, n, c_name, c_content);
				if (flag) change_count++;
			}
		}
	}
	for (int i = 0; i < max; i++){
		if(mydb.db_delete(i)){
			int count = 0;
			while (count < 10){
				int n = rand() % max + 1;
				mydb.db_find(n);
				count++;
			}
		}	
	}
	end = clock();
	cout<<"time:"<<end - begin<<endl;
}
void help(){
	cout<<"基于b+tree索引的数据库 :"<<endl;
	cout<<"---版本1.0，by stone"<<endl;
	cout<<"------------------使用指令说明----------------------"<<endl;
	cout<<"read filename1 filename2\n		------- 以filename1为数据文件，filename2为索引文件创建并读取数据库"<<endl;
	cout<<"insert1 filename1        \n               ------- 向已打开的数据库中插入filename1中的数据"<<endl;
	cout<<"insert2 key(int), name(string), content(string) \n		-------向已打开的数据库中插入一条数据"<<endl;
	cout<<"delete key(int)         \n               ------- 在数据库中删除键为key的数据"<<endl;
	cout<<"change key1(int), key2(int), name(string), content(string)\n		-------将数据库中key1值的记录修改为输入的数据"<<endl;
	cout<<"find key(int)           \n		------- 在数据库中查找键为key的数据"<<endl;
	cout<<"close                   \n               ------- 关闭已打开的数据库"<<endl;
	cout<<"quit                    \n               ------- 退出程序"<<endl;
	cout<<"----------------------------------------------------"<<endl;
}           
int main(){
	string command;
	cout<<"请输入指令，“test”启动测试，“show”启动展示 :"<<endl;
	cin>>command;
	if (command == "test"){
		while (true){
			cout<<"请输入测试数据数值，数据文件名称，索引文件名称 :"<<endl;
			int nrec;
			string d_name, i_name;
			cin>>nrec>>d_name>>i_name;
			test(nrec, d_name, i_name);
			cout<<"输入quit退出程序， 输入其他字符则再次执行 :"<<endl;
			string cmd;
			cin>>cmd;
			if (cmd ==  "quit") break;
		}
	}
	else if (command == "show"){
		help();
		DataBase mydb;
		while (true){
			string cmd;
			cout<<"请输入指令 :"<<endl;
			cin>>cmd;
			if (cmd == "read"){
				string d_name, i_name;
				cin>>d_name>>i_name;
				mydb.db_open(d_name, i_name);
				cout<<"数据库已打开"<<endl;
			}
			else if (cmd == "insert1"){
				string d_name;
				cin>>d_name;
				if (mydb.db_insert(d_name))
					cout<<"插入成功！"<<endl;
			}
			else if (cmd == "insert2"){
				int ID;
				string name, content;
				cin>>ID>>name>>content;
				if (mydb.db_insert(ID, name, content))
					cout<<"插入成功！"<<endl;
			}
			else if (cmd == "delete"){
				int key;
				cin>>key;
				if (mydb.db_delete(key))
					cout<<"删除成功！"<<endl;
			}
			else if (cmd == "change"){
				int key1, key2;
				string name, content;
				cin>>key1>>key2>>name>>content;
				if (mydb.db_change(key1, key2, name, content))
					cout<<"修改成功！"<<endl;
			}
			else if (cmd == "find"){
				int key;
				cin>>key;
				if (!mydb.db_find(key))
					cout<<"无对应数据！"<<endl;
			}
			else if (cmd == "close"){
				mydb.db_close();
				cout<<"数据库已关闭"<<endl;
			}
			else if (cmd == "quit")
				break;
			else 
				help();
		}
	}				
}

