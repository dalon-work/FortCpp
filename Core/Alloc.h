#ifndef FortCpp_ALLOC_H
#define FortCpp_ALLOC_H

#include "ArrayBase.h"
#include <assert.h>
#include <iostream>

namespace FortCpp
{

namespace internal
{

template<typename _T, int _Rank, int _Options>
struct traits<Alloc<_T,_Rank,_Options> > {
	typedef _T Scalar;
	enum {
	    Rank = _Rank,
	    StorageType = Pointer,
	    Order = _Options & MajorOrderBit,
	    Align  = _Options & AlignedBit,
	    Stride = _Options & StrideBit,
	    Options = _Options
	};

}; // end struct traits


}; // end namespace internal

template<typename T,int Rank, int Options>
class Alloc : public ArrayBase<Alloc<T,Rank,Options> >
{

	typedef class Alloc<T,Rank,Options> Derived;
	typedef ArrayBase<Derived> Base;
	static const int Order = internal::traits<Derived>::Order;
	static const int Stride = internal::traits<Derived>::Stride;
	static const int StorageType = internal::traits<Derived>::StorageType;
	static const int Align = internal::traits<Derived>::Align;

private:
	internal::Storage<T,StorageType,Align> _storage;
	std::array<int,Rank> _dim = {{0}};
	std::array<int,Rank> _str = {{0}};

public:
	Alloc(){
      static_assert(Rank > 0,"RANK OF ARRAY MUST BE GREATER THAN 0");
   }

	Alloc(const Derived& B) : Alloc() {
		(*this).mold(B);
		*this = B;
	}

	Alloc(Derived&& B) : Alloc() {
		swap(B);
	}

	~Alloc() {
		deallocate();
	}

	void deallocate() {
		_dim = {{0}};
		_str = {{0}};
		_storage.deallocate();
	}

	template<typename... indices>
	Alloc(indices... idx) {
		allocate(idx...);
	}

	void swap(Derived&& B) {
		swap(B);
	}

	void swap(Derived& B) {
		std::array<int,Rank> d = _dim;
		std::array<int,Rank> s = _str;

		_dim = B._dim;
		_str = B._str;

		B._dim = d;
		B._str = s;

		_storage.swap(B._storage);
	}

	/***********************************************/

	const T& operator = (const T& B) {
		return Base::operator = (B);
	}

	template<typename OtherDerived>
	const Derived& operator = (const ArrayBase<OtherDerived>& B) {
		static_assert(Rank == internal::traits<OtherDerived>::Rank,
		              "RANKS DO NOT MATCH IN ASSIGNMENT OPERATION");
		return Base::operator = (B);
	}

	const Derived& operator = (const Derived& B) {
		return Base::operator = (B);
	}

	const Derived& operator = (Derived&& B) {
		return Base::operator = (B);
	}


	/************************************************/

	template<typename... indices>
	void allocate(indices... idx) {
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSED TO ALLOCATE DOES NOT MATCH RANK OF ARRAY");
		internal::set_array<Rank,0>(_dim,idx...);
#ifndef NDEBUG
		internal::debug::is_allocated(allocated(),associated());
#endif
		for (int i=0; i<Rank; i++) { _str[i] = 1; }
		internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.allocate(internal::product(idx...));
	}

	void allocate(const std::array<int,Rank>& new_dim) {
#ifndef NDEBUG
		internal::debug::is_allocated(allocated());
#endif
		_dim = new_dim;
		for (int i=0; i<Rank; i++) { _str[i] = 1; }
		internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.allocate(internal::product<Rank>(_dim));
	}

	template<typename... indices>
	void map(T* a, indices... idx) {
		static_assert(Align == UnAligned,
		              "CANNOT MAP ONTO AN ALIGNED ARRAY");
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSED TO MAP DOES NOT MATCH RANK OF ARRAY");
		static_assert(Stride == Contig,
		              "CANNOT MAP A STRIDED ARRAY");
		internal::set_array<Rank,0>(_dim,static_cast<int>(idx)...);
		for (int i=0; i<Rank; i++) { _str[i] = 1; }
		internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.map(a,internal::product(static_cast<int>(idx)...));
	}

	template<typename OtherDerived>
	void mold(const ArrayBase<OtherDerived>& B) {
		static_assert(Rank == internal::traits<OtherDerived>::Rank,
		              "RANKS DO NOT MATCH IN MOLD OPERATION");
		static_assert(Stride == Contig,
		              "CANNOT MOLD A STRIDED ARRAY");
		deallocate();
		allocate(B.derived().get_dim());
	}
	/***************************************/

	int offset(const std::array<int,Rank>& idx) const {
		int s = 0;
		for (int r=0; r<Rank; r++) {
			s += idx[r]*_str[r];
		}
		return s;
	}

	/***************************************/

	template<typename... indices>
	const T& operator () (indices... idx) const {
#ifndef NDEBUG
		internal::debug::is_negative<0>(idx...);
		internal::debug::in_bounds<0,Rank>(_dim,idx...);
#endif

		return _storage[
		           internal::offset<Order,Stride,Rank>::exec(_str,idx...)
		       ];
	}

	template<typename... indices>
	T& operator () (indices... idx) {
#ifndef NDEBUG
		internal::debug::is_negative<0>(idx...);
		internal::debug::in_bounds<0,Rank>(_dim,idx...);
#endif
		return _storage[
		           internal::offset<Order,Stride,Rank>::exec(_str,idx...)
		       ];
	}

	/*************************************************/

	const T& operator [] (int i) const {
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[
		           internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
		       ];
	}
	T& operator [] (int i) {
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[
		           internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
		       ];
	}

	/***********************************************/

	int size()                              const { return _storage.size();       }
	int size  (int r)                  const {
#ifndef NDEBUG
		internal::debug::in_rank<Rank>(r);
#endif
		return _dim[r-1];
	}
	int stride(int r)                  const {
#ifndef NDEBUG
		internal::debug::in_rank<Rank>(r);
#endif
		return _str[r-1];
	}
	const std::array<int,Rank>& get_dim()   const { return _dim;                  }
	const std::array<int,Rank>& get_str()   const { return _str;                  }
	bool allocated ()                            const { return _storage.allocated();  }
	bool associated()                            const { return _storage.associated(); }
	bool contiguous()                            const { return Stride == Contig;      }
	int rank()                              const { return Rank;                  }
	T*  data()                                         { return _storage.data();       }

	/***********************************************/

	template<typename... indices>
	Alloc<T, internal::count_slice<indices...>::count,
	      Order | UnAligned | internal::contig_view<Order,Stride,indices...>::stride>
	view(indices... idx) {
		static const int newRank = internal::count_slice<indices...>::count;
		static const int newStride = internal::contig_view<Order,Stride,indices...>::stride;
		static_assert(newRank > 0, "VIEW IS OF RANK ZERO");
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSES TO VIEW DOES NOT MATCH RANK OF ARRAY");

		Alloc<T,newRank, Order | UnAligned | newStride> S;

		std::array<int,Rank> beg;
		std::array<int,newRank> len;
		std::array<int,newRank> str;

		internal::set_len<Rank,newRank,0,0>(_dim,len,idx...);
		internal::set_beg<Rank          ,0>(beg     ,idx...);
		internal::set_str<Rank,newRank,0,0>(_str,str,idx...);

		S.mapView(&_storage[offset(beg)],len,str);

		return S;

	}

	 void mapView(T* p,
	              const std::array<int,Rank>& dim,
	              const std::array<int     ,Rank>& str) {
	     _storage.map(p,internal::product<Rank>(dim));
	     _dim = dim;
	     _str = str;
	 }


	// template<typename... indices>
	// Alloc<T,Rank-sizeof...(indices),Order | UnAligned | Contig>
	// reduce(indices... idx) {
	//  static const int newRank = Rank-sizeof...(indices);
	//
	//






	/*******************************************************/


};

}; // end namespace FortCpp

#endif
