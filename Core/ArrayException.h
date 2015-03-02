#ifndef FortCpp_ARRAY_EXCEPTION_H
#define FortCpp_ARRAY_EXCEPTION_H

#include <exception>
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "stdio.h"
#include "execinfo.h"
#include "unistd.h"
using namespace std;

namespace FortCpp
{

namespace internal
{
  enum BOUND
  {
    Lower,
    Upper
  };
}; // end namespace internal 

/** 
 * This class is the base class for all the Array Exceptions
 * The only thing it does is print out a stacktrace.
 * This only works using the gcc, due the use of the
 * backtrace() function, which is a non-standard part of gcc.
 * It also only works for linux systems with the addr2line
 * program installed. This stacktrace may or may not be
 * accurate, your mileage may vary. If your system doesn't
 * meet these requirements, comment out the code inside
 * the constructor and set this constructor as a breakpoint
 * in your favorite debugger. 
 */
class ArrayException : public exception
{
  public:
  ArrayException(){
    cout << "ARRAY ERROR: PRESS ANY KEY TO EXIT THE PROGRAM" << endl;
#ifdef FortCpp_ARRAY_BACKTRACE
    const int s = 1024;
    // char **symbols;
    void *buf[s];
    char exe[s];
    char link[s];

    // pulls the symbol information
    int bt_size = backtrace(buf,s);
    // symbols = backtrace_symbols(buf,bt_size);

    // pulls the executable name to use with addr2line
    snprintf(link,sizeof link,"/proc/%d/exe",getpid());
    if(readlink(link,exe,sizeof link)==-1) {
        fprintf(stderr,"ERROR\n");
        exit(1);
    }

    cout << "==== ARRAY ERROR ====" << endl;
    for (int i=2; i<bt_size-2; ++i){
      char syscom[s];
      sprintf(syscom,"addr2line %p -e %s", buf[i] , exe ); //last parameter is the name of this app
      system(syscom);
    }
#endif
  }
  virtual const char* what() const throw()=0;

  static void handle_exception(exception &e){
    cout << e.what() << endl;
#ifdef FortCpp_EXCEPTION_PAUSE
    cin.ignore();
    exit(-1);
#endif
  }


};

class SizeException : public ArrayException
{
  public:
    const int a,b;
    SizeException(const int A, const int B) : a(A),b(B) {}

    const char * what() const throw(){
      std::stringstream s;
      s << "SIZE OF LHS (" << a 
        << ") AND RHS ("   << b 
        << ") IN ASSIGNMENT DO NOT MATCH" 
        << std::endl;
      return s.str().c_str();
    }
};

class BoundsException : public ArrayException
{
  public:
    const int i,d,dim;
    BoundsException(const int I, const int DIM, const int D) : i(I),d(D),dim(DIM) {}

    const char * what() const throw(){
      std::stringstream s;
      s << "INDEX " << i 
        << " OUT OF BOUNDS IN DIMENSION " << dim 
        << ". (SIZE: " << d << ")" << endl;
      return s.str().c_str();
    }
};

class LinearSizeException : public ArrayException
{
  public:
    const int i,size;
    LinearSizeException(const int I, const int _size) : i(I),size(_size) {}

    const char * what() const throw(){
      std::stringstream s;
      s << "INDEX " << i 
        << " OUT OF BOUNDS OF ARRAY SIZE (" << size << ")" << endl;
      return s.str().c_str();
    }
};



}; // end namespace FortCpp





#endif

