#ifndef STORAGE_H
#define STORAGE_H
#include "ArrayException.h"

namespace FortCpp
{

namespace internal
{

template<typename T,unsigned StorageType,unsigned Align> class Storage;

template<typename T>
class Storage<T,Pointer,UnAligned>
{
  typedef class Storage<T,Pointer,UnAligned> Derived;
  private:

  T* A=nullptr;
  bool alloc=0;

  public:
  Storage()=default;
  Storage(const Derived &B)=delete;
  Storage(const Derived &&B)=delete;

  ~Storage(){
     deallocate(); 
  }

   void allocate(unsigned i){
    alloc = 1;
    A = new T[i];
  }

  void map(T* a, unsigned i){
    alloc = 0;
    A = a;
  }

   T* data() { return A; }
   bool allocated() const { return alloc; }
   bool associated() const { return A; }

   const T & operator [] (unsigned i) const { 
// #ifdef FortCpp_READ_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
    return A[i]; 
  }

   T & operator [] (unsigned i){
// #ifdef FortCpp_WRITE_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
    return A[i]; }

  /**
   * Deallocates memory, or disassociates the array from the 
   * memory it points to if the array was not allocated
   */
  void deallocate(){
    if ( alloc ){ delete[] A; }
    A = nullptr;
    alloc = 0;
  }

}; // end class Storage

}; // end namespace internal

}; // end namespace FortCpp

#endif
