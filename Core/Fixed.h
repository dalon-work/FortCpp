#ifndef FortCpp_FIXED_H
#define FortCpp_FIXED_H

#include "FixedUtil.h"

namespace FortCpp
{

namespace internal
{


/*** TRAITS ***/
template<typename _T,Index... _dims>
struct traits<Fixed<_T,_dims...> > {
	typedef _T Scalar;
	enum {
		Rank = sizeof...(_dims),
		Order = ColMajor,
		Stride = Contig,
		StorageType = Static,
		Align = UnAligned
	};
}; // end struct traits


}; // end namespace internal

template<typename T,Index... dims>
class Fixed : public ArrayBase<Fixed<T,dims...> >
{

	typedef class Fixed<T,dims...> Derived;
	typedef ArrayBase<Derived> Base;
	static const Index Rank = sizeof...(dims);
	static const Index Order = internal::traits<Derived>::Order;
	static const Index Stride = internal::traits<Derived>::Stride;
	static const Index StorageType = internal::traits<Derived>::StorageType;
	static const Index Align = internal::traits<Derived>::Align;

// private:
	internal::Storage<T,StorageType,Align,dims...> _storage;
#ifndef NDEBUG
	const std::array<Index,Rank> _dim = {{dims...}};
#endif

public:
	Fixed()                =default;
	Fixed(const Derived& B)=default;
	Fixed(Derived&& B)     =default;
	~Fixed()               =default;

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

	/***************************************/

	template<typename... indices>
	const T& operator () (indices... idx) const
	{
#ifndef NDEBUG
		internal::debug::is_negative<0>(idx...);
		internal::debug::in_bounds<0,Rank>(_dim,idx...);
#endif

		return _storage[
		           internal::fixed_compute_offset<1,dims...>(idx...)
		];
	}

	template<typename... indices>
	T& operator () (indices... idx)
	{
#ifndef NDEBUG
		internal::debug::is_negative<0>(idx...);
		internal::debug::in_bounds<0,Rank>(_dim,idx...);
#endif
		return _storage[
		           internal::fixed_compute_offset<1,dims...>(idx...)
		];
	}

	/*************************************************/

	const T& operator [] (Index i) const
	{
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[i];
	}
	T& operator [] (Index i)
	{
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[i];
	}

	/***********************************************/

	Index size() const { return _storage.size(); }
	template<Index r>
	Index size  ()                  const
	{
#ifndef NDEBUG
		internal::debug::in_rank<r,Rank>();
#endif
		return internal::fixed_get_dim<1,r,dims...>::value;
	}
	std::array<Index,Rank> get_dim()   const
	{
		std::array<Index,Rank> d = {{dims...}};
		return d;
	}

//  const std::array<Index,Rank>& get_str()   const { return _str;                  }
//  bool allocated ()                            const { return _storage.allocated();  }
//  bool associated()                            const { return _storage.associated(); }
//    bool contiguous()                            const { return Stride == Contig;      }
//    Index rank()                              const { return Rank;                  }
	T*  data()                                         { return _storage.data();       }

	/***********************************************/

	// template<typename... indices>
	// Fixed<T, internal::count_slice<indices...>::count,
	//    Order | UnAligned | internal::contig_view<Order,Stride,indices...>::stride>
	// view(indices... idx){
	//    static const Index newRank = internal::count_slice<indices...>::count;
	//    static const Index newStride = internal::contig_view<Order,Stride,indices...>::stride;
	//    static_assert(newRank > 0, "VIEW IS OF RANK ZERO");
	//    static_assert(sizeof...(idx) == Rank,
	//          "NUMBER OF INDICES PASSES TO VIEW DOES NOT MATCH RANK OF ARRAY");
	//
	//    Fixed<T,newRank, Order | UnAligned | newStride> S;
	//
	//    std::array<Index,Rank> beg;
	//    std::array<Index,newRank> len;
	//    std::array<Index,newRank> str;
	//
	//    internal::set_len<Rank,newRank,0,0>(_dim,len,idx...);
	//    internal::set_beg<Rank          ,0>(beg     ,idx...);
	//    internal::set_str<Rank,newRank,0,0>(_str,str,idx...);
	//
	//    S.mapView(&_storage[offset(beg)],len,str);
	//
	//    return S;
	//
	// }
	//
	// void mapView(T* p,
	//       const std::array<Index,Rank>& dim,
	//       const std::array<Index,Rank>& str)
	// {
	//    _storage.map(p,internal::product<Rank>(dim));
	//    _dim = dim;
	//    _str = str;
	// }











	/*******************************************************/


};

}; // end namespace FortCpp

#endif
