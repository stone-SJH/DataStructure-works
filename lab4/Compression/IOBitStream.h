//Huffman Encoding
//f1303703 5130379072 石嘉昊
#ifndef _IOBITSTREAM_
#define _IOBITSTREAM_

#include <fstream>
using namespace std;
class Ibitstream : public ifstream{
private:
	int pos;//bit position
	int cur_byte;//current byte addr
	streampos last_pos;//last tellg() return position
public:
	Ibitstream();
	void open(const char *filename);//open a file using binary state
	int bitRead();//bit read function
	void rewind();//rewind the stream to read the file from the start
};
class Obitstream : public ofstream{//same as Ibitstream
private:
	int pos;
	int cur_bit;
	streampos last_pos;
public:
	Obitstream();
	void open(const char *filename);
	void bitWrite(int bit);//bit= 0 or 1
};

#endif
