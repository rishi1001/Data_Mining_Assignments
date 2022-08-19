
.PHONY:apriori fptree

apriori:
	@g++ -O3 main.cpp -o apriori 

apriori_test: apriori
	@echo "Running on test2" 
	@time ./apriori ./testcases/2/test2.dat 30 ./testcases/2/myout.dat
	@echo "Checking on test2"
	@diff ./testcases/2/out_30.dat_sorted ./testcases/2/myout.dat
	@echo "Running on test3" 
	@time ./apriori ./testcases/3/test3.dat 1 ./testcases/3/myout.dat
	@echo "Checking on test3"
	@diff ./testcases/3/out_1.dat_sorted ./testcases/3/myout.dat
	@echo "Running on test4" 
	@time ./apriori ./testcases/4/test4.dat 90 ./testcases/4/myout.dat
	@echo "Checking on test4"
	@diff ./testcases/4/out_90.dat_sorted ./testcases/4/myout.dat
	@echo "Running on test5" 
	@time ./apriori ./testcases/5/test5.dat 98 ./testcases/5/myout.dat
	@echo "Checking on test5"
	@diff ./testcases/5/out_98.dat_sorted ./testcases/5/myout.dat


fptree:
	g++ -O3 fpt.cpp -o fpt 
	./fpt ./testcases/2/test2.dat 30 ./testcases/2/myout.dat

