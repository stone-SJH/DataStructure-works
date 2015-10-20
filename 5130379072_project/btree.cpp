#include "btree.h"

bool BPTree :: readFile(string filename){
	bool flag = false;
	bool success = true;
	if (filename != data_name.c_str()) flag = true;
	fstream fs;
	fs.open(index_name.c_str(), ios::binary|ios::in);
	if (!fs){
		BPTreeNode node;//设置根节点
		node.total =0;
        	for (int i = 0; i < M; ++i){
        	        node.key[i] = 0;
        	        node.value[i] = 0;
        	        node.children[i] = 0;
        	}
        	node.parent = 0;
        	node.left = 0;
        	node.right = 0;
        	node.is_active = true;
        	node.is_leaf = true;
        	root = 16; //设置头文件，包括根节点的位置和数最小节点的位置
        	least = 16;
        	fstream outfile (index_name.c_str(),  ios::binary|ios::out|ios::trunc);//写入header
        	outfile.seekp(0, ios::beg);
        	outfile.write((char *)&root, sizeof(long));
        	outfile.write((char *)&least, sizeof(long));
        	outfile.write((char *)&node, sizeof(BPTreeNode));
		outfile.close();
	}
	fstream ifs(filename.c_str(), ios::in|ios::binary);//读入数据并写入树中
	fstream ofs(data_name.c_str(), ios::out|ios::app);
	ofs.seekp(0, ios::end);
	Record record;
	long p = 0;
	if (flag) p = ofs.tellp();
	while (ifs.read((char *)&record, sizeof(record))){
		IndexNode index;
		long key;
		long position;
		key = index.key = record.ID;//设定key值，可以在此处修改key的类型，如可以将一条记录的name设定为key。这里将ID设定为key是为了方便测试
		index.value = p;//设定value值，即该条记录在数据文件中的位置
		if (flag){
			ofs.write((char *)&record, sizeof(record));
			p = ofs.tellp();
		}
		else p = ifs.tellp();
		if (findKey(key, position)) success = false;
		else if (insertKey(index, position));
		else success = false;
	}
	ifs.close();
	if (!success) return false;
	return true;
}

bool BPTree :: findKey(int key, long &position){
	BPTreeNode node;
	long point;
        fstream fs(index_name.c_str(),  ios::binary|ios::in|ios::out);
        fs.seekp(0, ios::beg);
        fs.read((char *)&root, sizeof(root));//读入root
        fs.seekp(root, ios::beg);
	while(true){
		cur = fs.tellp();
		fs.read((char *)&node, sizeof(BPTreeNode));//读入root节点数据
		if (!node.is_active) continue; //跳过已删除节点的访问	
		if (!node.is_leaf){//b+树的数据存放在叶子节点上，要找到叶子节点
			for (int i = 1; i <= node.total; ++i){//查找到该行节点的最接近查找值的位置
                                point = -1;
                                if (node.key[i] > key){
                                        point = node.children[i-1];
                                        break;
                                }
                        }
                        if (point == -1) {
                                point = node.children[node.total];
                        }
                        fs.seekp(point, ios::beg);//定位下一次查找索引时的起始位置
                        pre = cur;//迭代当前位置
		}
		else{
			for (int i = 1; i <= node.total; ++i){
				if (key == node.key[i]){
					position = cur;
					fs.close();
					return true;
				}
			}
			position = cur;
			return false;
		}
	}
}

bool BPTree :: insertKey(IndexNode index, long position){
	fstream fs(index_name.c_str(), ios::in|ios::out|ios::binary);
	fs.seekp(0, ios::end);
	long ed = fs.tellp();
	BPTreeNode node, new_node;
	long next_key;
	long next_value;//分裂后的节点，存储以插入父节点
	long key = index.key;
	long value = index.value;
	if (position == 0){//分裂根节点
		BPTreeNode temp_node;
		//创建新的根节点
		new_node.total = 1;
		new_node.key[1] = key;
		new_node.value[1] = value;
		new_node.children[0] = root;
		new_node.children[1] = new_pos;
		new_node.parent = 0;
		new_node.left = 0;
		new_node.right = 0;
		new_node.is_active = true;
		new_node.is_leaf = false;
		//查找合适的位置存放新的根节点
		fs.seekp(16, ios::beg);
		cur = fs.tellp();
		fs.read((char *)&temp_node, sizeof(BPTreeNode));
		while(temp_node.is_active && fs.tellp() < ed){//找到文件尾部或者找到一个被删除的节点
			cur = fs.tellp();
			fs.read((char *)&temp_node, sizeof(BPTreeNode));
		}
		if (temp_node.is_active) new_pos = fs.tellp();
		else{
			fs.seekp(cur, ios::beg);
			new_pos = fs.tellp();
		}
		fs.seekp(new_pos, ios::beg);
		fs.write((char *)&new_node, sizeof(BPTreeNode));
		root = new_pos;
		fs.seekp(0, ios::beg);
		fs.write((char *)&root, sizeof(long));
		for (int i = 0; i <= new_node.total; ++i){
			fs.seekp(new_node.children[i], ios::beg);
			fs.read((char *)&temp_node, sizeof(BPTreeNode));
			temp_node.parent = new_pos;
			fs.seekp(new_node.children[i], ios::beg);
			fs.write((char *)&temp_node, sizeof(BPTreeNode));
		}
		fs.close();
		return true;
	}
	else{//不需要分裂根节点
		fs.seekp(position, ios::beg);
		fs.read((char *)&node, sizeof(BPTreeNode));
		long insert_pos_in_node = 0;//插入新节点在原节点的子节点数组中的位置
		if (node.total < M - 1){//要插入的节点仍有空余位置则按序插入新节点
                     	bool flag = false;//判定新节点是否放入原节点的子节点数组中
			for (int i = 1; i <= node.total; ++i){
				if (key < node.key[i]){
					for (int j = node.total - i, k = -1; j >= 0; --j, ++k){
						node.key[node.total - k] = node.key[node.total - k - 1];
						node.children[node.total - k] = node.children[node.total - k -1];
						node.value[node.total - k] = node.value[node.total - k - 1];
					}
					node.key[i] = index.key;
					node.value[i] = index.value;
					node.children[i] = new_pos;
					insert_pos_in_node = i;
					flag = true;
					break;
				}
			}
			if (!flag){
				node.key[node.total + 1] = index.key;
				node.value[node.total + 1] = index.value;
				node.children[node.total + 1] = new_pos;
				insert_pos_in_node = node.total + 1;
			}
			node.total += 1;
			fs.seekp(position, ios::beg);
			fs.write((char *)&node, sizeof(BPTreeNode));
			fs.close();
			return true;
		}
		else{//要插入的原节点中已无空余位置，则需要分裂节点
			long t_key[M];
			long t_value[M];
			long t_children[M+1];
			BPTreeNode temp_node;
			bool flag = false;
			for (int i = 1, j = 0, k = 0; i < M; i++, k++){//是否能够插入新键值。flag是判断变量，在j被更改为1后说明已经插入新键值，并使以后的更替后移一位
				if (node.key[i] < key || flag){
					t_key[i-1+j] = node.key[i];
					t_value[i-1+j] = node.value[i];
					t_children[k+j] = node.children[k];
				}
				else{
					t_key[i-1] = key;
					t_value[i-1] = value;
					t_children[k] = node.children[k];
					t_children[k+1] = new_pos;
					j = 1;
					i--;
					flag = true;
				}
			}
			if (!flag){
				t_key[M-1] = key;
				t_value[M-1] = value;
				t_children[M-1] = node.children[M-1];
				t_children[M] = new_pos;
			}
			//以上构建虚拟节点包含t_key,t_value,t_children;
			//下面构建新节点
			node.total = M / 2;

			if (!node.is_leaf){//该节点不是叶子节点，则是内部节点
				if (M % 2 == 0) new_node.total = M / 2 - 1;
				else new_node.total = M / 2;
				for (int i = 0, j = 1; i <= M; i++){//建立新节点，修改旧节点
					if (i < M / 2){
						node.key[i+1] = t_key[i];
						node.value[i+1] = t_value[i];
						node.children[i] = t_children[i];
					}
					else if (i == M / 2){
						next_key = t_key[i];
						next_value = t_value[i];
						node.children[i] = t_children[i];
					}
					else if (i < M){
						new_node.key[j] = t_key[i];
						new_node.value[j] = t_value[i];
						new_node.children[j-1] = t_children[i];
						j++;
						node.key[i] = 0;
						node.value[i] = 0;
					}
					else if (i == M) new_node.children[j-1] = t_children[i];
				}
				new_node.is_leaf = false;
			}
			else{//该节点是叶子节点
				if (M % 2 != 0) new_node.total = M / 2 + 1;
				else new_node.total = M / 2;
				long max;
				if (M % 2 == 0) max = M - 1;
				else max = M;
				for (int i = 0, j = 1; i <= max; i++){
					if (i < M / 2){
						node.key[i+1] = t_key[i];
						node.value[i+1] = t_value[i];
					}
					else{
						new_node.key[j] = t_key[i];
						new_node.value[j] = t_value[i];
						j++;
						if (i < M-1){
							node.key[i+1] = 0;
							node.value[i+1] = 0;
						}
					}
				}
				new_node.is_leaf = true;
				next_key = new_node.key[1];
				next_value = new_node.value[1];
				for (int i = 0; i < M; ++i) new_node.children[i] = 0;//叶节点设置空的子节点集合
			}
			new_node.key[0] = 0;
			new_node.parent = node.parent;
			new_node.left = position;
			new_node.right = node.right;
			new_node.is_active = true;
			//以上新节点new_node完全建立完毕，下面查找其存放位置。规则是若存在被删除的节点则覆盖之，若不存在则放到index.dat索引文件的尾部。
			/*fs.seekp(16, ios::beg);//跳过header
			cur = fs.tellp();
			fs.read((char *)&temp_node, sizeof(BPTreeNode));
			while(temp_node.is_active && fs.tellp() < ed){//找到文件尾部或者找到一个被删除的节点
				cur = fs.tellp();
				fs.read((char *)&temp_node, sizeof(BPTreeNode));
			}
			if (temp_node.is_active) new_pos = fs.tellp();
			else{-------------------------------------------------
				fs.seekp(cur, ios::beg);
				new_pos = cur;
			}*/
			new_pos = ed;
			if (!new_node.is_leaf){//重写分裂出的新节点的原有成员的父节点位置，将之设定为新节点并写入索引文件
				for (int i = 0; i <= new_node.total; i++){
					BPTreeNode child;
					fs.seekp(new_node.children[i], ios::beg);
					fs.read((char *)&child, sizeof(BPTreeNode));
					child.parent = new_pos;
					fs.seekp(new_node.children[i], ios::beg);
					fs.write((char *)&child, sizeof(BPTreeNode));
				}
			}
			//插入分裂后的节点并修改原节点右值
			fs.seekp(new_pos, ios::beg);
			fs.write((char *)&new_node, sizeof(BPTreeNode));
			node.right = new_pos;
			fs.seekp(position, ios::beg);
			fs.write((char *)&node, sizeof(BPTreeNode));
			fs.close();
			//递归插入需要插入父节点的键值
			IndexNode new_index;
			new_index.key = next_key;
			new_index.value = next_value;
			if (insertKey(new_index, new_node.parent)) return true;
			else return false;
		}
	}
}
bool BPTree :: deleteKey(int key){
	long position;
	BPTreeNode node;
	int pos_in_node;
	if (!findKey(key, position)) return false;
	fstream fs(index_name.c_str(), ios::in|ios::out|ios::binary);
	fs.seekp(position, ios::beg);
	fs.read((char *)&node, sizeof(BPTreeNode));
	for (pos_in_node = 1; pos_in_node <= node.total; pos_in_node++){//查找被删除key的位置
		if (node.key[pos_in_node] == key) break;
	}
	for (int i = pos_in_node; i <= node.total; i++){
		node.key[i] = node.key[i+1];
		node.value[i] = node.value[i+1];
	}
	node.total--;
	if (node.total < M/2) merge(node, position);//如果节点内数量少于一半则考虑合并节点（向左右节点借也放在合并函数中）
	fs.seekp(position, ios::beg);
	fs.write((char *)&node, sizeof(BPTreeNode)); 
	return true;
}

bool BPTree :: printKey(int key){
	fstream ifs(data_name.c_str(), ios::in|ios::binary);
	fstream fs(index_name.c_str(), ios::in|ios::out|ios::binary);
	BPTreeNode node;
	Record record;
	long index_pos = -1;
	long data_pos = -1;
	if(findKey(key, index_pos)){
		fs.seekp(index_pos, ios::beg);
		fs.read((char *)&node, sizeof(BPTreeNode));
		for (int i = 1; i <= node.total; i++){
			if (node.key[i] == key){
				data_pos = node.value[i];
				ifs.seekp(data_pos, ios::beg);
				ifs.read((char *)&record, sizeof(Record));
				cout<<record.ID<<"\n  "<<record.name<<"\n      "<<record.content<<endl;
				return true;
			}
		}
	}
	//else cout<<"No such data"<<endl;
	return false;
	fs.close();
	ifs.close();
}
void BPTree :: merge(BPTreeNode node, long position){
	fstream fs(index_name.c_str(), ios::in|ios::out|ios::binary);	
	long pos = node.parent;
	fs.seekp(pos);
	BPTreeNode lnode;
	BPTreeNode rnode;
	BPTreeNode temp_node1;
	BPTreeNode temp_node2;
	fs.read((char *)&temp_node1, sizeof(BPTreeNode));//读入父节点，开始检查
	for (int i = 1; i < temp_node1.total; i++){
		fs.seekp(temp_node1.children[i]);
		fs.read((char *)&temp_node2, sizeof(BPTreeNode));
		if (temp_node1.children[i+1] == position){//定位左节点
			fs.seekp(temp_node1.children[i]);
			fs.read((char *)&lnode, sizeof(BPTreeNode));
			if (temp_node2.total > M/2){//如果可以则借节点
				fs.seekp(temp_node2.children[temp_node2.total]);
				node.total++;
				for (int j = node.total-1; j > 1; j--){
					node.key[j+1] = node.key[j];
					node.value[j+1] = node.value[j];
				}
				node.key[1] = temp_node2.key[temp_node2.total];
				node.value[1] = temp_node2.key[temp_node2.total];
				temp_node2.total--;
			}
			return;
		}
		else if (temp_node1.children[i-1] == position){//定位右节点
			fs.seekp(temp_node1.children[i]);
			fs.read((char *)&rnode, sizeof(BPTreeNode));
			if (temp_node2.total > M/2){//如果可以则借节点
				fs.seekp(temp_node2.children[temp_node2.total]);
				node.total++;
				for (int j = 1; j < node.total - 1; j++){
					node.key[j] = node.key[j+1];
					node.value[j] = node.value[j+1];
				}
				node.key[node.total] = temp_node2.key[1];
				node.value[node.total] = temp_node2.key[1];
				temp_node2.total--;
			}
			return;
		}
	}
	//都没有可以借节点的则开始合并
	if (lnode.total != 0){//假如有左边兄弟则与左边合并
		for (int i = node.total, j = 1; i < node.total + lnode.total; i++, j++){
			 node.key[i] = lnode.key[j];
			 node.value[i] = lnode.children[j];
		}
		lnode.total = 0;
		temp_node1.total--;
		if (temp_node1.total == M) merge(temp_node1, temp_node1.parent);//递归检查
	}
	else  if (rnode.total != 0){//假如有右边兄弟则与右边合并
		for (int i = node.total, j = 1; i < node.total + rnode.total; i++, j++){
			 node.key[i] = rnode.key[j];
			 node.value[i] = rnode.children[j];
		}
		lnode.total = 0;
		temp_node1.total--;
		if (temp_node1.total == M) merge(temp_node1, temp_node1.parent);//递归检查
	}
	return;
};
