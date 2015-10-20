i use recursion in this lab.all the method is written in the class named Mycity.here i explain some of the algorithm.
first some value:virtual_map is a map that is 2lines2rows larger than the map to help check whether the map state is valid
                 max_score is to record the max score now to help average pruning.
then some method:process(): core algorithm use recursion, in the second line i start check valid(), and the last line is checked by check(),and use pruing algorithm in test();
                 valid():check one point valid/invalid
                 transmap():i found that 5*3 is faster than 3*5 so i use it to trans all m*n(m<n) to n*m so it will be faster:)
---------------------------------
RUN: ./city(name) input.txt output.txt
intput.txt: (for example) 3
                          1 1
                          2 4
                          5 5
output.txt:(nothing is ok)
---------------------------------
by 5130379072 sjh stone
14/11/2
