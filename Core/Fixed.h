#ifndef FortCpp_FIXED_H
#define FortCpp_FIXED_H

namespace FortCpp
{

template<int _Options>
struct FixedOptions{
	enum {
	    StorageType = _Options & internal::StorageBit,
	    Order       = _Options & internal::MajorOrderBit,
	    Align       = _Options & internal::AlignedBit,
	    Stride      =  Contig,
	    Options     = _Options
	};

};

namespace internal
{


template<typename... dims> struct fixed_traits;

template<int Options>
struct fixed_traits<FixedOptions<Options> > : FixedOptions<Options> {};

template<>
struct fixed_traits<int D,int last> {
   static const int dim = last;
   enum {
	    StorageType = Static,
	    Order = ColMajor,
	    Align  = UnAligned,
	    Stride = Contig,
	    Options = 0
   };
};

template<typename... dims>
struct fixed_traits<int D,int i,dims...> : fixed_traits<D+1,dims...> 
{
   static const int dim = i;
};

template<typename _T,typename... _dims>
struct traits<Fixed<_T,_dims...> > : fixed_traits<1,_dims...> {
	typedef _T Scalar;
}; // end struct traits


}; // end namespace internal

template<typename T,typename... dims>
class Fixed : public ArrayBase<Fixed<T,dims...> >
{

	typedef class Fixed<T,dims...> Derived;
	typedef ArrayBase<Derived> Base;
   static const int Rank = internal::count_dims<dims...>::count;
	static const int Order = internal::traits<Derived>::Order;
	static const int Stride = internal::traits<Derived>::Stride;
   static const int StorageType = internal::traits<Derived>::StorageType;
   static const int Align = internal::traits<Derived>::Align;

private:
   internal::Storage<T,StorageType,Align> _storage;
public:
	Fixed()                =default;
	Fixed(const Derived& B)=default;
	Fixed(Derived&& B)     =default;

	~Fixed() 
   {
		deallocate();
	}

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

	// template<typename... indices>
	// void map(T* a, indices... idx)
   // {
	// 	static_assert(Align == UnAligned,
   //          "CANNOT MAP ONTO AN ALIGNED ARRAY");
	// 	static_assert(sizeof...(idx) == Rank, 
   //          "NUMBER OF INDICES PASSED TO MAP DOES NOT MATCH RANK OF ARRAY");
   //    static_assert(Stride == Contig,
   //          "CANNOT MAP A STRIDED ARRAY");
   //    internal::set_array<Rank,0>(_dim,static_cast<int>(idx)...);
   //    for(int i=0;i<Rank;i++) { _str[i] = 1; }
   //    internal::compute_strides<Order,Rank>::exec(_str,_dim);
	// 	_storage.map(a,internal::product(static_cast<int>(idx)...));
	// }

	/***************************************/

   // int offset(const std::array<int,Rank>& idx) const {
   //    int s = 0;
   //    for(int r=0;r<Rank;r++){
   //       s += idx[r]*_str[r];
   //    }
   //    return s;
   // }

	/***************************************/

// 	template<typename... indices>
// 	const T& operator () (indices... idx) const {
// #ifndef NDEBUG
//       internal::debug::is_negative<0>(idx...);
//       internal::debug::in_bounds<0,Rank>(_dim,idx...);
// #endif
//
// 		return _storage[
//          internal::offset<Rank,0>(_str,static_cast<int>(idx)...)
//          ];
// 	}

// 	template<typename... indices>
// 	T& operator () (indices... idx) {
// #ifndef NDEBUG
//       internal::debug::is_negative<0>(idx...);
//       internal::debug::in_bounds<0,Rank>(_dim,idx...);
// #endif
// 		return _storage[
//          internal::offset<Rank,0>(_str,static_cast<int>(idx)...)
//          ];
// 	}

	/*************************************************/

// 	const T& operator [] (int i) const {
// #ifndef NDEBUG
//       internal::debug::in_size(i,size());
// #endif
// 		return _storage[
//          internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
//          ];
// 	}
// 	T& operator [] (int i) {
// #ifndef NDEBUG
//       internal::debug::in_size(i,size());
// #endif
// 		return _storage[
//          internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
//          ];
// 	}

	/***********************************************/

// 	int size()                              const { return _storage.size();       }
// 	int size  (int r)                  const { 
// #ifndef NDEBUG
//       internal::debug::in_rank<Rank>(r);
// #endif
//       return _dim[r-1];             }
// 	int stride(int r)                  const {
// #ifndef NDEBUG
//       internal::debug::in_rank<Rank>(r);
// #endif
//       return _str[r-1];             }
// 	const std::array<int,Rank>& get_dim()   const { return _dim;                  }
// 	const std::array<int,Rank>& get_str()   const { return _str;                  }
// 	bool allocated ()                            const { return _storage.allocated();  }
// 	bool associated()                            const { return _storage.associated(); }
//    bool contiguous()                            const { return Stride == Contig;      }
//    int rank()                              const { return Rank;                  }
// 	T*  data()                                         { return _storage.data();       }

	/***********************************************/

   // template<typename... indices>
   // Fixed<T, internal::count_slice<indices...>::count,
   //    Order | UnAligned | internal::contig_view<Order,Stride,indices...>::stride>
   // view(indices... idx){
   //    static const int newRank = internal::count_slice<indices...>::count;
   //    static const int newStride = internal::contig_view<Order,Stride,indices...>::stride;
   //    static_assert(newRank > 0, "VIEW IS OF RANK ZERO");
   //    static_assert(sizeof...(idx) == Rank,
   //          "NUMBER OF INDICES PASSES TO VIEW DOES NOT MATCH RANK OF ARRAY");
   //
   //    Fixed<T,newRank, Order | UnAligned | newStride> S;
   //
   //    std::array<int,Rank> beg;
   //    std::array<int,newRank> len;
   //    std::array<int,newRank> str;
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
   //       const std::array<int,Rank>& dim,
   //       const std::array<int,Rank>& str)
   // {
   //    _storage.map(p,internal::product<Rank>(dim));
   //    _dim = dim;
   //    _str = str;
   // }











	/*******************************************************/


};

}; // end namespace FortCpp

#endif
