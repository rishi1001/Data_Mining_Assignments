cd Q2
g++ -std=c++11 convert.cpp -o convert
g++ -std=c++11 index.cpp -o index
cd ..
./Q2/convert $1
./Q2/gSpan -f formatted.txt -s 0.3 -o -i
./Q2/index 0.3 1