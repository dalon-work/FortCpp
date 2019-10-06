#ifndef STORAGE_H
#define STORAGE_H
#include "ArrayException.h"

namespace FortCpp
{

namespace internal
{


template<typename T,int Alignment>
class Storage<T,Pointer,Alignment>
{
	typedef class Storage<T,Pointer,Alignment> Derived;
private:

	T* _A=nullptr;
	bool _alloc=0;
	unsigned _size=0;

public:
	Storage()=default;
	Storage(const Derived& B)=default;
	Storage(Derived&& B)=default;

	~Storage() {
		deallocate();
	}

	void allocate(unsigned i) {
		_alloc = 1;
      if ( i == 0 ) return;

      if constexpr ( Alignment == Aligned ) {
         i = sizeof(T)*i;
         i += 64 - (i % 64);
         _A = std::aligned_alloc(64,sizeof(T)*i);
      }
      else {
         _A = std::malloc(sizeof(T)*i);
      }

		_size = i;
	}

	void map(T* A, unsigned i) {
		_alloc = 0;
		_A = A;
		_size = i;
	}

	void swap(Derived& B) {
		T*   p =   _A;
		unsigned  s = _size;
		bool a = _alloc;

		_A     = B._A;
		_size  = B._size;
		_alloc = B._alloc;

		B._A     = p;
		B._size  = s;
		B._alloc = a;
	}

	T* data() { return _A; }
	bool allocated() const { return _alloc; }
	bool associated() const { return _A; }
	unsigned size() const { return _size; }

	const T& operator [] (int i) const {
// #ifdef FortCpp_READ_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
		return _A[i];
	}

	T& operator [] (int i) {
// #ifdef FortCpp_WRITE_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
		return _A[i];
	}

	/**
	 * Deallocates memory, or disassociates the array from the
	 * memory it points to if the array was not allocated
	 */
	void deallocate() {
		if ( _alloc ) { std::free(_A); }
		_A = nullptr;
		_alloc = 0;
		_size = 0;
	}

}; // end class Storage

/****  FIXED_STORAGE *****/

template<typename T,int... dims>
class Storage<T,Static,UnAligned,dims...>
{
	typedef class Storage<T,Static,UnAligned,dims...> Derived;
private:
	static constexpr unsigned _size = internal::fixed_product<dims...>::value;
	static constexpr bool _alloc=1;

	T _A[_size];

public:
	Storage()                 = default;
	Storage(const Derived& B) = default;
	Storage(Derived&& B)      = default;
	~Storage()                = default;


	T* data() { return _A.data(); }
	bool allocated() const { return 1; }
	bool associated() const { return 1; }
	unsigned size() const { return _size; }

	const T& operator [] (int i) const {
// #ifdef FortCpp_READ_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
		return _A[i];
	}

	T& operator [] (int i) {
// #ifdef FortCpp_WRITE_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
		return _A[i];
	}

}; // end class Storage

template<typename T,int... dims>
class Storage<T,Static,Aligned,dims...>
{
	typedef class Storage<T,Static,Aligned,dims...> Derived;
private:
	static constexpr unsigned _size = internal::fixed_product<dims...>::value;
	static constexpr bool _alloc=1;

	alignas(64) T _A[_size];

public:
	Storage()                 = default;
	Storage(const Derived& B) = default;
	Storage(Derived&& B)      = default;
	~Storage()                = default;


	T* data() { return _A.data(); }
	bool allocated() const { return 1; }
	bool associated() const { return 1; }
	unsigned size() const { return _size; }

	const T& operator [] (int i) const {
// #ifdef FortCpp_READ_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
		return _A[i];
	}

	T& operator [] (int i) {
// #ifdef FortCpp_WRITE_NAN
//     FortCpp_NAN_CHECK(A[i])
// #endif
		return _A[i];
	}

}; // end class Storage

}; // end namespace internal

}; // end namespace FortCpp

#endif
