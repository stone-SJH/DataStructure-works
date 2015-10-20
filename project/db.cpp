#include "db.h"

DataBase :: DataBase(){
	db = new BPTree();
	is_open = false;
}

DataBase :: DataBase(string data_name, string index_name){
	db_open(data_name, index_name);
}

void DataBase :: db_open(string data_name, string index_name){
	db = new BPTree();
	db->data_name = data_name;
	db->index_name = index_name;
	db->readFile(data_name);
	is_open = true;
	ds_name = data_name;
}	

bool DataBase :: db_isOpen(){
	return is_open;
}

bool DataBase :: db_insert(string insert_file_name){
	if (is_open){
		if (db->readFile(insert_file_name)) return true;
		else return false;
	}
	else return false;
}

bool DataBase :: db_insert(int ID, string name, string content){
	if (is_open){
		fstream fs("temp_data.dat", ios::binary|ios::out);
		fs.write((char *)&ID, sizeof(int));
		fs.write(name.c_str(), NAMESIZE);
		fs.write(content.c_str(), CONTSIZE);
		fs.close();
		if(db->readFile("temp_data.dat")) return true;
		else return true;
	}
	else return false;
}

bool DataBase :: db_delete(int key){
	if (is_open){
		if (db->deleteKey(key)) return true;
		else return false;
	}
	else return false;
}

bool DataBase :: db_change(int old_key, int new_key, string new_name, string new_content){
	if (is_open){
		if (db->deleteKey(old_key)){
			if(db_insert(new_key, new_name, new_content))
				return true;
			else return false;
		}
		else return false;
	}
	else return false;
}

bool DataBase :: db_find(int key){
	if (is_open)
		if(db->printKey(key)) return true;
	return false;
}

bool DataBase :: naive_find(int key){
	Record record;
	fstream fs(ds_name.c_str(), ios::in|ios::binary);
	while (fs.read((char *)&record, sizeof(Record))){
		if (record.ID == key){
			cout<<record.ID<<"\n  "<<record.name<<"\n      "<<record.content<<endl;
			return true;
		}
	}
	return false;
}

void DataBase :: db_close(){
	is_open = false;
	free(db);
	db = new BPTree();
}
		
