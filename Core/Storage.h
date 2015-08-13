#ifndef STORAGE_H
#define STORAGE_H
#include "ArrayException.h"

namespace FortCpp
{

namespace internal
{

template<typename T,int Type,int Size, int Align> class Storage;

template<typename T>
class Storage<T,Pointer,Unknown,UnAligned>
{
  typedef class Storage<T,Pointer,Unknown,UnAligned> Derived;
  private:
  T*__restrict__ _A;
  int _size;
  bool _alloc;


  public:
  Storage(const Derived &B)=default;
  inline explicit Storage() : _size(0), _alloc(0), _A(0) {}

  ~Storage(){ this->deallocate(); }

  inline void allocate(const int i){
    this->_size = i;
    this->_alloc = 1;
    this->_A = new T[this->_size];
  }

  /** 
   * Maps a pointer into the array. The size of the 
   * pointer array is given by i
   */
  void map(T* a, const int i){
    this->_alloc = 0;
    this->_size = i;
    this->_A = a;
  }

  inline T* data() { return _A; }
  inline const int size() const { return _size; }
  inline const bool allocated() const { return _alloc; }
  inline const bool associated() const { return _A; }
  inline const T & operator [] (const int i) const { 
#ifdef FortCpp_READ_NAN
    FortCpp_NAN_CHECK(_A[i])
#endif
    return _A[i]; 
  }
  inline T & operator [] (const int i){
#ifdef FortCpp_WRITE_NAN
    FortCpp_NAN_CHECK(_A[i])
#endif
    return _A[i]; }

  /**
   * Deallocates memory, or disassociates the array from the 
   * memory it points to if the array was not allocated
   */
  void deallocate(){
    if ( this->_alloc ){ delete[] this->_A; }
    this->_A = 0;
    this->_alloc = 0;
    this->_size = 0;
  }

}; // end class Storage

template<typename T,int Size>
class Storage<T,Pointer,Size,UnAligned>
{
  typedef class Storage<T,Pointer,Size,UnAligned> Derived;
  private:
  T*__restrict__ _A;


  public:
  Storage(const Derived &B)=default;
  inline explicit Storage() : _A(0) {}

  ~Storage(){ this->deallocate(); }

  /** 
   * Maps a pointer into the array. The size of the 
   * pointer array is given by i
   */
  void map(T* a, const int i){
    this->_A = a;
  }

  inline T* data() { return _A; }
  inline const int size() const { return Size; }
  inline const bool allocated() const { return 0; }
  inline const bool associated() const { return 1; }
  inline const T & operator [] (const int i) const {
#ifdef FortCpp_READ_NAN
    FortCpp_NAN_CHECK(_A[i])
#endif
    return _A[i]; }
  inline T & operator [] (const int i){
#ifdef FortCpp_WRITE_NAN
    FortCpp_NAN_CHECK(_A[i])
#endif
    return _A[i]; }

  /**
   * Deallocates memory, or disassociates the array from the 
   * memory it points to if the array was not allocated
   */
  void deallocate(){
    this->_A = 0;
  }

}; // end class Storage


template<typename T,int Size>
class Storage<T,Static,Size,UnAligned>
{
  typedef class Storage<T,Static,Size,UnAligned> Derived;
  private:
  T _A[Size];

  Storage(const Derived &B){ }

  public:
  inline explicit Storage() {}

  ~Storage(){}

  inline static const int size() { return Size; }
  inline T* data() { return _A; }
  inline const T & operator [] (const int i) const { 
#ifdef FortCpp_READ_NAN
    FortCpp_NAN_CHECK(_A[i])
#endif
    return _A[i]; }
  inline T & operator [] (const int i){
#ifdef FortCpp_WRITE_NAN
    FortCpp_NAN_CHECK(_A[i])
#endif
    return _A[i]; }
  inline const bool allocated() const { return 1; }
  inline const bool associated() const { return 1; }
  inline void deallocate() const {}

}; // end class Storage

}; // end namespace internal

}; // end namespace FortCpp

#endif
