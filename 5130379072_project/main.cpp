#include "db.h"
#include <time.h>

int main(){
	int nrec = 500000;
	clock_t begin, end;
	string data_filename = "1.dat";
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
	mydb.db_open(data_filename, "2.dat");
	begin = clock();
	int n = 0;
	while (n < 2000){
		int x = rand()%nrec + 1;
		if (mydb.db_find(x)) n++;
	}
 	end = clock();
	cout<<end - begin<<endl;
	int c; cin>>c;
	begin = clock();
	n = 0;
	while (n < 2000){
		int x = rand()%nrec + 1;
		if (mydb.naive_find(x)) n++;
	}
	end = clock();
	cout<<end - begin <<endl;
}
