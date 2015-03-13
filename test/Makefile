
all:
	gfortran slicing.f90 -c -g -fcheck=all -O0
	g++ main.C slicing.o -lgfortran -o test.exec -std=c++11 -g -O0
