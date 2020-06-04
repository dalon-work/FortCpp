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
	virtual const char* what() const throw()
	{
		return _msg.c_str();
	}

};

class NanException : public ArrayException
{
public:
	NanException() {};

	const char* what() const throw()
	{
		std::stringstream s;
		s << "NaN found!" << std::endl;
		return s.str().c_str();
	}
};

class DimException : public ArrayException
{
public:
	Index a,b,d;
	DimException(Index A,Index B,Index D) : a(A),b(B),d(D) {}

	const char* what() const throw()
	{
		std::stringstream s;
		s << "DIMENSIONS OF LHS AND RHS DO NOT MATCH" << std::endl
		  << "DIM: " << d << " LHS: " << a << " RHS: " << b << std::endl;
		return s.str().c_str();
	}
};

class NegIdxException : public ArrayException
{
public:
	Index i,d;
	NegIdxException(Index I,Index D) : i(I),d(D) {}

	const char* what() const throw()
	{
		std::stringstream s;
		s << "NEGATIVE INDEX " << i <<
		  " NOT ALLOWED IN DIMENSION " << d << std::endl;
		return s.str().c_str();
	}

};

class BoundsException : public ArrayException
{
public:
	Index i,d,dim;
	BoundsException(Index I, Index D, Index DIM) : i(I),d(D),dim(DIM) {}

	const char* what() const throw()
	{
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
	Index i,size;
	SizeException(Index I, Index _size) : i(I),size(_size) {}

	const char* what() const throw()
	{
		std::stringstream s;
		s << "INDEX " << i
		  << " OUT OF BOUNDS OF ARRAY SIZE (" << size << ")" << std::endl;
		return s.str().c_str();
	}
};

class RankException : public ArrayException
{
public:
	Index i,rank;
	RankException(Index I, Index _rank) : i(I),rank(_rank) {}

	const char* what() const throw()
	{
		std::stringstream s;
		s << "DIMENSION " << i
		  << " OUT OF BOUNDS OF ARRAY RANK (" << rank << ")" << std::endl;
		return s.str().c_str();
	}
};

class BoundSliceException : public ArrayException
{
public:
	Index i,d;
	BoundSliceException(Index I,Index D) : i(I), d(D) {}

	const char* what() const throw()
	{
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
	Index b,e,s;
	ZeroSliceException(Index B,Index E,Index S) : b(B), e(E), s(S) {}

	const char* what() const throw()
	{
		std::stringstream st;
		st << "SLICE IS OF SIZE 0 WITH:" << std::endl
		   << "BEG: " << b << " END: " << e << " STRIDE: " << s << std::endl;
		return st.str().c_str();
	}
};


}; // end namespace FortCpp





#endif

