
all: testMain_opt.o
	g++ -O3 -g --std=c++17 testMain_opt.o tests.C -o tests_opt.exec

testMain_opt.o: testMain.C
	g++ -O3 -g --std=c++17 testMain.C -c -o testMain_opt.o
