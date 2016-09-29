#ifndef FortCpp_ARRAY_EXCEPTION_H
#define FortCpp_ARRAY_EXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

namespace FortCpp
{

class ArrayException : public std::exception
{
public:
	std::string _msg;
	ArrayException() {};
	ArrayException(std::string msg) : _msg(msg) {} ;
	virtual const char* what() const throw() {
		return _msg.c_str();
	}

};

class NanException : public ArrayException
{
public:
	NanException() {};

	const char* what() const throw() {
		std::stringstream s;
		s << "NaN found!" << std::endl;
		return s.str().c_str();
	}
};

class DimException : public ArrayException
{
public:
	int a,b,d;
	DimException(int A,int B,int D) : a(A),b(B),d(D) {}

	const char* what() const throw() {
		std::stringstream s;
		s << "DIMENSIONS OF LHS AND RHS DO NOT MATCH" << std::endl
		  << "DIM: " << d << " LHS: " << a << " RHS: " << b << std::endl;
		return s.str().c_str();
	}
};

class NegIdxException : public ArrayException
{
public:
	int i,d;
	NegIdxException(int I,int D) : i(I),d(D) {}

	const char* what() const throw() {
		std::stringstream s;
		s << "NEGATIVE INDEX " << i <<
		  " NOT ALLOWED IN DIMENSION " << d << std::endl;
		return s.str().c_str();
	}

};

class BoundsException : public ArrayException
{
public:
	int i,d,dim;
	BoundsException(int I, int D, int DIM) : i(I),d(D),dim(DIM) {}

	const char* what() const throw() {
		std::stringstream s;
		s << "INDEX " << i
		  << " OUT OF BOUNDS IN DIMENSION " << dim
		  << ". (SIZE: " << d << ")" << std::endl;
		return s.str().c_str();
	}
};

class SizeException : public ArrayException
{
public:
	int i,size;
	SizeException(int I, int _size) : i(I),size(_size) {}

	const char* what() const throw() {
		std::stringstream s;
		s << "INDEX " << i
		  << " OUT OF BOUNDS OF ARRAY SIZE (" << size << ")" << std::endl;
		return s.str().c_str();
	}
};

class RankException : public ArrayException
{
public:
	int i,rank;
	RankException(int I, int _rank) : i(I),rank(_rank) {}

	const char* what() const throw() {
		std::stringstream s;
		s << "DIMENSION " << i
		  << " OUT OF BOUNDS OF ARRAY RANK (" << rank << ")" << std::endl;
		return s.str().c_str();
	}
};

class BoundSliceException : public ArrayException
{
public:
	int i,d;
	BoundSliceException(int I,int D) : i(I), d(D) {}

	const char* what() const throw() {
		std::stringstream s;
		s << "INDEX " << i
		  << " OUT OF BOUNDS."
		  << " (SIZE: " << d << ")" << std::endl;
		return s.str().c_str();
	}
};

class ZeroSliceException : public ArrayException
{
public:
	int b,e,s;
	ZeroSliceException(int B,int E,int S) : b(B), e(E), s(S) {}

	const char* what() const throw() {
		std::stringstream st;
		st << "SLICE IS OF SIZE 0 WITH:" << std::endl
		  << "BEG: " << b << " END: " << e << " STRIDE: " << s << std::endl;
		return st.str().c_str();
	}
};


}; // end namespace FortCpp





#endif

