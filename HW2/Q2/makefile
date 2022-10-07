run:
	./gSpan -f formatted.txt  -s 0.3 -o -i
	g++ -std=c++11 index.cpp -o index
	./index 0.3 1
	g++ -std=c++11 query.cpp -I vf3lib/include -o query
	./query