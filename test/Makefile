
all: testMain_debug.o testMain_opt.o
	clang++ -O0 -g --std=c++11 testMain_debug.o tests.C -o tests_debug.exec
	g++ -O3 -g --std=c++11 testMain_opt.o   tests.C -o tests_opt.exec

testMain_debug.o: testMain.C
	g++ -O0 -g --std=c++11 testMain.C -c -o testMain_debug.o

testMain_opt.o: testMain.C
	g++ -O3 -g --std=c++11 testMain.C -c -o testMain_opt.o
