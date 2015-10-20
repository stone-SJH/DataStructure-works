//Huffman Encoding
//f1303703 5130379072 石嘉昊
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "huffman.h"
#include "IOBitStream.h"
using namespace std;

/*void naiveCopy(string inputFilename, string outputFilename) {
  ofstream ofs(outputFilename.c_str(), ios::out | ios::binary);
  ifstream ifs(inputFilename.c_str(), ios::in | ios::binary);
  char c;
  while (ifs.get(c)) ofs.put(c);
  ofs.close();
  ifs.close();
}

void compress(string inputFilename, string outputFilename) {
  naiveCopy(inputFilename, outputFilename);
};

void decompress(string inputFilename, string outputFilename) {
  naiveCopy(inputFilename, outputFilename);
}
*/
void useage(string prog) {
  cerr << "Useage: " << endl
      << "    " << prog << "[-d] input_file output_file" << endl;
  exit(2);
}

int main(int argc, char* argv[]) {
  int i;
  string inputFilename, outputFilename;
  bool isDecompress = false;
  for (i = 1; i < argc; i++) {
    if (argv[i] == string("-d")) isDecompress = true;//+tag -d 解码
    else if (inputFilename == "") inputFilename = argv[i];
    else if (outputFilename == "") outputFilename = argv[i];
    else useage(argv[0]);
  }
  //huffman类实例
  Huffman huffman_encoding;
  Ibitstream ibs;
  Obitstream obs;
  ibs.open(inputFilename.c_str());
  obs.open(outputFilename.c_str());
  if (outputFilename == "") useage(argv[0]);
  if (isDecompress){
	if (!huffman_encoding.decompression(ibs, obs)) cout<<"error!";
  }
  else huffman_encoding.compression(ibs, obs);
  return 0;
}
