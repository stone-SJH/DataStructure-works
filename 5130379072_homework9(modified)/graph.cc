//graph.cc
//f1303703 5130379072
//石嘉昊
//使用邻接链表，生成的图为传递给dijkstra（）函数的第一个参数
//12/10/2014
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
#define infinity 2147483647//set a max value helping find the minimum node
struct EdgeNode{
	int key;
	int value;
	EdgeNode *next;
};
struct GraphNode{
	int key;
	EdgeNode *first;
	GraphNode(){
		key = -1;
		first = NULL;
	}
};
void dijkstra(GraphNode *Graph, int source, int nodenum){
	bool flag = false;
	bool found[nodenum];
	int distance[nodenum];
	struct EdgeNode *node = Graph[source].first;
	for (int i = 0; i < nodenum; i++){
		distance[i] = infinity;
		found[i] = false;
	}
	while (node != NULL){
		distance[node->key]  = node->value;
		node = node->next;
	}
	found[source] = true;
	distance[source] = 0;
	for (int i = 0; i < nodenum; i++){
		int min = infinity;
		int to_change;
		for (int j = 0; j < nodenum; j++){//find the minimum node(from source)
			if (!found[j] && distance[j] < min){
				to_change = j;
				flag = true;
				min = distance[j];
			}
		}
		if (flag){
			found[to_change] = true;
			for(int i = 0; i < nodenum; i++){
				struct EdgeNode *new_node = Graph[to_change].first;
				int temp_dist = 0;
				while (new_node != NULL){
					if (new_node->key == i){
						temp_dist = new_node->value;
						break;
					}
					else new_node = new_node->next;
				}
				if (!found[i] && temp_dist > 0 && (min + temp_dist < distance[i]))
					distance[i] = min + temp_dist;
			}
		}
		flag = false;
	}
	for (int i = 0; i < nodenum; i++){//print the shortest distance from source node
		if (i != source && distance[i] == infinity) cout<< -1;
		else cout<<distance[i];
		if (i != nodenum - 1) cout<<"  ";
		else cout<<endl;
	}
}
						
int main(int argc, char *argv[]){
	string filename;
	if (argc != 2) cerr << "Usage: " << endl<< "    graph_hwk  input_file" << endl;
	else filename = argv[1];
	ifstream ifs;
	ifs.open(filename.c_str());
	int node_num;
	int edge_num;
	ifs>>node_num>>edge_num;
	node_num++;
	GraphNode *Graph = new GraphNode[node_num];
	for (int i = 0; i < node_num; i++) Graph[i].key = i;
	for (int i = 0; i < edge_num; i++){
		int first_node;
		int second_node;
		int value;
		ifs>>first_node>>second_node>>value;
		struct EdgeNode *new_node = new struct EdgeNode;
		new_node -> key = second_node;
		new_node -> value = value;
		new_node ->next = NULL;
		if (Graph[first_node].first == NULL)
			Graph[first_node].first = new_node;
		else{
			struct EdgeNode *last_node = Graph[first_node].first;
	 		while (last_node -> next != NULL){
				last_node = last_node -> next;
			}
			last_node -> next = new_node;
		}
	}
	dijkstra(Graph, 0, node_num);
	/*for(int i = 0; i < node_num; i++){
		cout<< i << ":";
		struct EdgeNode *node = Graph[i].first;
		while (node != NULL){
			cout<<node -> key<<","<<node ->value<<";";
			node = node ->next;
		}
		cout<<endl;
	}*/
} 
