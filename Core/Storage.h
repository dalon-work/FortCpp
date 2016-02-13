#ifndef STORAGE_H
#define STORAGE_H
#include "ArrayException.h"

namespace FortCpp
{

namespace internal
{

template<typename T,int StorageType,int Align> class Storage;

template<typename T>
class Storage<T,Pointer,UnAligned>
{
	typedef class Storage<T,Pointer,UnAligned> Derived;
private:

	T* _A=nullptr;
	bool _alloc=0;
	int _size;

public:
	Storage()=default;
	Storage(const Derived& B)=default;
	Storage(Derived&& B)=default;

	~Storage() {
		deallocate();
	}

	void allocate(int i) {
		_alloc = 1;
		_A = new T[i];
		_size = i;
	}

	void map(T* A, int i) {
		_alloc = 0;
		_A = A;
		_size = i;

	}

	T* data() { return _A; }
	bool allocated() const { return _alloc; }
	bool associated() const { return _A; }
	int size() const { return _size; }

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
		if ( _alloc ) { delete[] _A; }
		_A = nullptr;
		_alloc = 0;
		_size = 0;
	}

}; // end class Storage

}; // end namespace internal

}; // end namespace FortCpp

#endif
