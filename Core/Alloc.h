#ifndef FortCpp_ARRAY2D_H
#define FortCpp_ARRAY2D_H

#include "ArrayBase.h"
#include <assert.h>
// #include "Slice.h"
// #include "StrideCalc.h"

namespace FortCpp
{

namespace internal
{

template<typename _T, unsigned _Rank, unsigned _Options>
struct traits<Alloc<_T,_Rank,_Options> > {
	typedef _T Scalar;
	enum {
	    Size = Unknown,
	    Rank = _Rank,
	    StorageType = Pointer,
	    Order = _Options & MajorOrderBit,
	    Align  = _Options & AlignedBit,
	    Stride = _Options & StrideBit,
	    Options = _Options
	};

}; // end struct traits


}; // end namespace internal

template<typename T,unsigned Rank, unsigned Options>
class Alloc : public ArrayBase<Alloc<T,Rank,Options> >
{

	typedef class Alloc<T,Rank,Options> Derived;
	typedef ArrayBase<Derived> Base;
	static const unsigned Order = internal::traits<Derived>::Order;
	static const unsigned Stride = internal::traits<Derived>::Stride;
   static const unsigned StorageType = internal::traits<Derived>::StorageType;
   static const unsigned Align = internal::traits<Derived>::Align;

private:
   internal::Storage<T,StorageType,Align> _storage;
	std::array<unsigned,Rank> _dim;
	std::array<unsigned,Rank> _str;

public:
	Alloc()                =default;
	Alloc(const Derived& B)=delete;
	Alloc(Derived&& B)     =delete;

	~Alloc() 
   {
		deallocate();
	}

	void deallocate() 
   {
		_dim = {{0}};
		_str = {{0}};
		_storage.deallocate();
	}

	template<typename... indices>
	Alloc(indices... idx) 
   {
		allocate(idx...);
	}

	/***********************************************/

	const T& operator = (const T& B) 
   {
		return Base::operator = (B);
	}

	template<typename OtherDerived>
	const Derived& operator = (const ArrayBase<OtherDerived>& B) 
   {
		static_assert(Rank == internal::traits<OtherDerived>::Rank,
            "RANKS DO NOT MATCH IN ASSIGNMENT OPERATION");
		return Base::operator = (B);
	}

	const Derived& operator = (const Derived& B) 
   {
		return Base::operator = (B);
	}

	/************************************************/

	template<typename... indices>
	void allocate(indices... idx) 
   {
		static_assert(sizeof...(idx) == Rank, 
            "NUMBER OF INDICES PASSED TO ALLOCATE DOES NOT MATCH RANK OF ARRAY");
      internal::set_array<Rank,0>(_dim,static_cast<unsigned>(idx)...);
      for(int i=0;i<Rank;i++) { _str[i] = 1; }
      internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.allocate(internal::product(static_cast<unsigned>(idx)...));
	}

	void allocate(const std::array<unsigned,Rank>& new_dim)
   {
      _dim = new_dim;
      for(int i=0;i<Rank;i++) { _str[i] = 1; }
      internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.allocate(internal::product<Rank>(_dim));
	}

	template<typename... indices>
	void map(T* a, indices... idx)
   {
		static_assert(Align == UnAligned,
            "CANNOT MAP ONTO AN ALIGNED ARRAY");
		static_assert(sizeof...(idx) == Rank, 
            "NUMBER OF INDICES PASSED TO MAP DOES NOT MATCH RANK OF ARRAY");
      static_assert(Stride == Contig,
            "CANNOT MAP A STRIDED ARRAY");
      internal::set_array<Rank,0>(_dim,static_cast<unsigned>(idx)...);
      for(int i=0;i<Rank;i++) { _str[i] = 1; }
      internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.map(a,internal::product(static_cast<unsigned>(idx)...));
	}

	template<typename OtherDerived>
	void mold(const ArrayBase<OtherDerived>& B) 
   {
		static_assert(Rank == internal::traits<OtherDerived>::Rank,
            "RANKS DO NOT MATCH IN MOLD OPERATION");
      static_assert(Stride == Contig,
            "CANNOT MOLD A STRIDED ARRAY");
		deallocate();
      allocate(B.get_dim());
	}

	/***************************************/

	template<typename... indices>
	const T& operator () (indices... idx) const {
#ifndef NDEBUG
      internal::is_negative<0>(idx...);
      internal::in_bounds<0,Rank>(_dim,idx...);
#endif

		return _storage[
         internal::offset<Rank,0>(_str,static_cast<unsigned>(idx)...)
         ];
	}

	template<typename... indices>
	T& operator () (indices... idx) {
#ifndef NDEBUG
      internal::is_negative<0>(idx...);
      internal::in_bounds<0,Rank>(_dim,idx...);
#endif
		return _storage[
         internal::offset<Rank,0>(_str,static_cast<unsigned>(idx)...)
         ];
	}

	/*************************************************/

	const T& operator [] (int i) const {
#ifndef NDEBUG
      internal::in_size(i,size());
#endif
		return _storage[
         internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
         ];
	}
	T& operator [] (int i) {
#ifndef NDEBUG
      internal::in_size(i,size());
#endif
		return _storage[
         internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
         ];
	}

	/***********************************************/

	unsigned size()                              const { return _storage.size();       }
	unsigned size  (unsigned r)                  const { 
#ifndef NDEBUG
      internal::in_rank<Rank>(r);
#endif
      return _dim[r-1];             }
	unsigned stride(unsigned r)                  const {
#ifndef NDEBUG
      internal::in_rank<Rank>(r);
#endif
      return _str[r-1];             }
	const std::array<unsigned,Rank>& get_dim()   const { return _dim;                  }
	const std::array<unsigned,Rank>& get_str()   const { return _str;                  }
	bool allocated ()                            const { return _storage.allocated();  }
	bool associated()                            const { return _storage.associated(); }
	T*  data()                                         { return _storage.data();       }

	/***********************************************/



	// Alloc<T,internal::count_slice<indices...>::count,Order|UnAligned|Strided>
//  template<typename... indices>
//     void view(indices... idx){
//        Alloc<T,internal::count_slice<indices...>::count,Order|UnAligned|Strided> S;
//        // cout << internal::count_slice<indices...>::count << endl;
//
//     }
//
//  template<typename OtherDerived,typename... indices>
//     void map_view(ArrayBase<OtherDerived>& A,indices... idx){
//
//        std::array<unsigned,internal::traits<OtherDerived>::Rank> beg;
//     }








	// template< FortCpp_SLICE_TEMPLATE(1) >
	//  StridedArray1d<Derived,FortCpp_SLICE(1) > operator () (const FortCpp_SLICE_0(1) &S1,
	//                                                            const int j){
	//   typedef StridedArray1d<Derived,FortCpp_SLICE(1)> ResultType;
	//   enum{
	//     Stride1 = internal::traits<ResultType>::S1
	//   };
	//   const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
	//   const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
	//   FortCpp_SLICE_CHECK(1)
	//   FortCpp_BOUNDS_CHECK(j,2)
	//   ResultType S(&storage[dim(S1.b(),j)],l1,s1);
	//   return S;
	// }

	// template<FortCpp_SLICE_TEMPLATE(2)>
	//  StridedArray1d<Derived,FortCpp_SLICE(2)> operator () (const int i,
	//                                                          const FortCpp_SLICE_0(2) &S2){
	//   typedef StridedArray1d<Derived,FortCpp_SLICE(2)> ResultType;
	//   enum{
	//     Stride1 = internal::traits<ResultType>::S1
	//   };
	//   const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
	//   const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
	//   FortCpp_BOUNDS_CHECK(i,1)
	//   FortCpp_SLICE_CHECK(2)
	//   ResultType S(&storage[dim(i,S2.b())],l2,s2);
	//   return S;
	// }
	//
	// template<FortCpp_SLICE_TEMPLATE(1), FortCpp_SLICE_TEMPLATE(2)>
	//  StridedAlloc<Derived,FortCpp_SLICE(1),
	//                               FortCpp_SLICE(2) >operator () (const FortCpp_SLICE_0(1) &S1,
	//                                                          const FortCpp_SLICE_0(2) &S2){
	//   typedef StridedAlloc<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2)> ResultType;
	//   enum{
	//     Stride1 = internal::traits<ResultType>::S1,
	//     Stride2 = internal::traits<ResultType>::S2
	//   };
	//   const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
	//   const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
	//   const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
	//   const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
	//   FortCpp_SLICE_CHECK(1)
	//   FortCpp_SLICE_CHECK(2)
	//   ResultType S(&storage[dim(S1.b(),S2.b())],l1,l2,s1,s2);
	//   return S;
	// }

	/*******************************************************/


};

}; // end namespace FortCpp

#endif
