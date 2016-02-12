#ifndef FortCpp_ALLOC_H
#define FortCpp_ALLOC_H

#include "ArrayBase.h"
#include <assert.h>
#include <iostream>

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
	Alloc(const Derived& B)=default;
	Alloc(Derived&& B)     =default;

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

   unsigned offset(const std::array<unsigned,Rank>& idx) const {
      unsigned s = 0;
      for(int r=0;r<Rank;r++){
         s += idx[r]*_str[r];
      }
      return s;
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
   bool contiguous()                            const { return Stride == Contig;      }
   unsigned rank()                              const { return Rank;                  }
	T*  data()                                         { return _storage.data();       }

	/***********************************************/

   template<typename... indices>
   Alloc<T, internal::count_slice<indices...>::count,
      Order | UnAligned | internal::contig_view<Order,Stride,indices...>::stride>
   view(indices... idx){
      static const unsigned newRank = internal::count_slice<indices...>::count;
      static const unsigned newStride = internal::contig_view<Order,Stride,indices...>::stride;
      static_assert(newRank > 0, "VIEW IS OF RANK ZERO");
      static_assert(sizeof...(idx) == Rank,
            "NUMBER OF INDICES PASSES TO VIEW DOES NOT MATCH RANK OF ARRAY");

      Alloc<T,newRank, Order | UnAligned | newStride> S;

      std::array<unsigned,Rank> beg;
      std::array<unsigned,newRank> len;
      std::array<unsigned,newRank> str;

      internal::set_len<Rank,newRank,0,0>(_dim,len,idx...);
      internal::set_beg<Rank          ,0>(beg     ,idx...);
      internal::set_str<Rank,newRank,0,0>(_str,str,idx...);

      S.mapView(&_storage[offset(beg)],len,str);

      return S;

   }

   void mapView(T* p,
         const std::array<unsigned,Rank>& dim,
         const std::array<unsigned,Rank>& str)
   {
      _storage.map(p,internal::product<Rank>(dim));
      _dim = dim;
      _str = str;
   }











	/*******************************************************/


};

}; // end namespace FortCpp

#endif
