//Huffman Encoding
//f1303703 5130379072 石嘉昊
#include "IOBitStream.h"
#include <iostream>

#define bits 8

int getBit(int n, int src){
	return ((src & (1 << n)) != 0);
}
void setBit(int n, int &dst){
	dst = dst | (1 << n);
}//from 0 to 1
Ibitstream :: Ibitstream() : pos(bits), cur_byte(0), last_pos(0) {};
void Ibitstream :: open(const char *filename){
	ifstream::open(filename, ios::binary);
}
int Ibitstream :: bitRead(){
	if ( last_pos != tellg()/*the bit data has been read from last bitRead() and now we should change the pos*/ || pos == bits){//a byte has beed read completely, now change a byte
		if ((cur_byte = get()) == EOF) return EOF;//get current byte addr
		pos = 0;//set position to the start of a byte
		last_pos = tellg();//set last_pos to the stream pointer's position
	}
	int now_pos = getBit(pos, cur_byte);//get the bit(0 or 1) using byte addr and position in a byte
	pos += 1;
	return now_pos;
}
void Ibitstream :: rewind(){
	clear();//clear the stream
	seekg(0, ios::beg);//set the stream pointer to the start
}
Obitstream :: Obitstream() : pos(bits), cur_bit(0), last_pos(0) {};
void Obitstream :: open(const char *filename){
	ofstream::open(filename, ios::binary);
}
void Obitstream :: bitWrite(int bit){
	if (last_pos != tellp() || pos == bits){//change a byte
		pos = 0;
		cur_bit = 0;
	}
	if (bit) setBit(pos, cur_bit);//if bit=1 then change it (the default bit data is set to be 0)
	if (pos == 0 || bit){
		if (pos != 0) seekp(-1, ios::cur);//if the bit has changed from 0 to 1 then overwrite
		put(cur_bit);//else if the bit is the first bit of a byte then print it
	}
	pos += 1;
	last_pos = tellp();
}
	
	 
