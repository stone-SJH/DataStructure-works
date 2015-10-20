#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define M 128
#define HEADSIZE 16
#define NAMESIZE 32
#define CONTSIZE 56	

struct IndexNode{
	int key;//存储id--由name生成或者自带
	int value;//二进制文件对应key值的B+树索引位置
};

struct BPTreeNode{	
	long parent;
	long left;
	long right;
	bool is_active;//是否被删除
	bool is_leaf;//是否是叶节点
	int total;//节点中子节点数量
	int key[M];
	long value[M];
	long children[M];//子节点位置（类似与指针，但是此处为子节点索引在文件中的位置）
};

struct Record{//一条数据代表的结构，可以直接在此处修改（除ID外的条目），对本程序其他不产生影响
	int ID;
	char name[NAMESIZE];
	char content[CONTSIZE];
};

class BPTree{
private:
//重要位置指针
	long root;//根节点位置
	long pre;//上一次被访问的位置
	long cur;//当前被访问的位置 
	long least;//键值最小的节点位置
	long new_pos;//要插入的新节点的位置
private:
	void merge(BPTreeNode node, long position);//合并两个节点并递归，在deleteKey函数中使用
public:
	string data_name;//数据文件名称
	string index_name;//索引文件名称
public:
	bool readFile(string filename);//读入数据文件并构建B+树
	bool findKey(int key, long &position);//查找某个key对应的数据
	bool insertKey(IndexNode index, long position);//插入某个key对应的数据
	bool deleteKey(int key);//删除某个key对应的数据
	bool printKey(int key);//输出
};
