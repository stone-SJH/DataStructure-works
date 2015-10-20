CXX = g++
CXXFLAGS = -IStanfordCPPLib -fvisibility-inlines-hidden -g
LDOPTIONS = -L.
LIB = -lStanfordCPPLib
Compression: Compression.cc
	g++ -o $@ $^ -Wall -O2  huffman.h huffman.cpp IOBitStream.h IOBitStream.cpp $(CXXFLAGS) $(LDOPTIONS) $(LIB)
clean:
	rm *.o *.tmp Compression -f
