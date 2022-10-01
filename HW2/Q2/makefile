run:
	./gSpan -f formatted.txt  -s 0.9 -o -i
	g++ -std=c++11 index.cpp -o index
	./index 0.9 1
	g++ -std=c++11 query.cpp -I vf3lib/include -o query
	./query query.txt output_2019CS10368.txt