#include "btree.h"

class DataBase{
private:
	BPTree* db;//实例化的b+tree
	string ds_name;//数据文件名称
	bool is_open;//是否处于数据库已打开的状态
public:
	DataBase();
	DataBase(string data_name, string index_name);//构造函数，以数据文件名称和索引文件名词创建一个数据库，也可用于读入已有的索引
	void db_open(string data_name, string index_name);//以数据文件名称和索引文件名词创建（打开）一个数据库
	bool db_isOpen();//返回该数据库是否已打开
	bool db_insert(string insert_file_name);//将一个数据文件中的数据插入该数据库
	bool db_insert(int ID, string name, string content);//将一条数据文件插入该数据库
	bool db_delete(int key);//删除key值代表的一条索引
	bool db_change(int old_key, int new_key, string new_name, string new_content);//将某条数据（old_key)修改为另一条(new_key,new_name,new_content)
	bool db_find(int key);//查找某key值代表的数据
	bool naive_find(int key);//不使用数据库，直接在数据文件中查找某条数据，用于性能对比
	void db_close();//关闭数据库
};
