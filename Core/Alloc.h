#ifndef FortCpp_ALLOC_H
#define FortCpp_ALLOC_H

#include "ArrayBase.h"
#include <assert.h>
#include <iostream>

namespace FortCpp
{

namespace internal
{

template<typename _T, Index _Rank, Index _Options>
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

template<typename T,Index Rank, Index Options>
class Alloc : public ArrayBase<Alloc<T,Rank,Options> >
{

	typedef class Alloc<T,Rank,Options> Derived;
	typedef ArrayBase<Derived> Base;
	static const Index Order = internal::traits<Derived>::Order;
	static const Index Stride = internal::traits<Derived>::Stride;
	static const Index StorageType = internal::traits<Derived>::StorageType;
	static const Index Align = internal::traits<Derived>::Align;

private:
	internal::Storage<T,StorageType,Align> _storage;
	std::array<Index,Rank> _dim = {{0}};
	std::array<Index,Rank> _str = {{0}};

public:
	Alloc()
	{
		static_assert(Rank > 0,"RANK OF ARRAY MUST BE GREATER THAN 0");
	}

	Alloc(const Derived& B) : Alloc()
	{
		(*this).mold(B);
		*this = B;
		std::cout << "FORTCPP COPY!" << std::endl;
	}

	Alloc(Derived&& B) = default;

	// Alloc(Derived&& B) : Alloc() {
	//  swap(B);
	// }

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
	explicit
	Alloc(indices... idx)
	{
		allocate(idx...);
	}

	void swap(Derived&& B)
	{
		swap(B);
	}

	void swap(Derived& B)
	{
		std::array<Index,Rank> d = _dim;
		std::array<Index,Rank> s = _str;

		_dim = B._dim;
		_str = B._str;

		B._dim = d;
		B._str = s;

		_storage.swap(B._storage);
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

	const Derived& operator = (Derived&& B)
	{
		return Base::operator = (B);
	}

	/************************************************/

	template<typename... indices>
	void allocate(indices... idx)
	{
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSED TO ALLOCATE DOES NOT MATCH RANK OF ARRAY");
		_dim = {{ idx... }};
		//internal::set_array<Rank,0>(_dim,idx...);
#ifndef NDEBUG
		internal::debug::is_allocated(allocated(),associated());
#endif
		for (Index i=0; i<Rank; i++) { _str[i] = 1; }
		internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.allocate(internal::product(static_cast<Index>(idx)...));
	}

	void allocate(const std::array<Index,Rank>& new_dim)
	{
#ifndef NDEBUG
		internal::debug::is_allocated(allocated(),associated());
#endif
		_dim = new_dim;
		for (Index i=0; i<Rank; i++) { _str[i] = 1; }
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
		_dim = {{ static_cast<Index>(idx)... }};
		//internal::set_array<Rank,0>(_dim,static_cast<Index>(idx)...);
		for (Index i=0; i<Rank; i++) { _str[i] = 1; }
		internal::compute_strides<Order,Rank>::exec(_str,_dim);
		_storage.map(a,internal::product(static_cast<Index>(idx)...));
	}

	template<typename OtherDerived>
	void mold(const ArrayBase<OtherDerived>& B)
	{
		static_assert(Rank == internal::traits<OtherDerived>::Rank,
		              "RANKS DO NOT MATCH IN MOLD OPERATION");
		static_assert(Stride == Contig,
		              "CANNOT MOLD A STRIDED ARRAY");
		deallocate();
		allocate(B.derived().get_dim());
	}
	/***************************************/

	Index offset(const std::array<Index,Rank>& idx) const
	{
		Index s = 0;
		for (Index r=0; r<Rank; r++) {
			s += idx[r]*_str[r];
		}
		return s;
	}

	/***************************************/

	template<typename... indices>
	const T& operator () (indices... idx) const
	{
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSED TO ALLOCATE DOES NOT MATCH RANK OF ARRAY");
#ifndef NDEBUG
		internal::debug::is_negative<0>(static_cast<Index>(idx)...);
		internal::debug::in_bounds<0,Rank>(_dim, static_cast<Index>(idx)...);
#endif

		return _storage[
		           internal::offset<Order,Stride,Rank>::exec(_str,static_cast<Index>(idx)...)
		];
	}

	template<typename... indices>
	T& operator () (indices... idx)
	{
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSED TO ALLOCATE DOES NOT MATCH RANK OF ARRAY");
#ifndef NDEBUG
		internal::debug::is_negative<0>(static_cast<Index>(idx)...);
		internal::debug::in_bounds<0,Rank>(_dim,static_cast<Index>(idx)...);
#endif
		return _storage[
		           internal::offset<Order,Stride,Rank>::exec(_str,static_cast<Index>(idx)...)
		];
	}

	/*************************************************/

	const T& operator [] (Index i) const
	{
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[
		           internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
		];
	}
	T& operator [] (Index i)
	{
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[
		           internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
		];
	}

	/***********************************************/

	Index size()                         const { return _storage.size();       }
	Index size  (Index r)                  const
	{
#ifndef NDEBUG
		internal::debug::in_rank<Rank>(r);
#endif
		return _dim[r-1];
	}
	Index stride(Index r)                  const
	{
#ifndef NDEBUG
		internal::debug::in_rank<Rank>(r);
#endif
		return _str[r-1];
	}
	const std::array<Index,Rank>& get_dim()   const { return _dim;                  }
	const std::array<Index,Rank>& get_str()   const { return _str;                  }
	bool allocated ()                            const { return _storage.allocated();  }
	bool associated()                            const { return _storage.associated(); }
	bool contiguous()                            const { return Stride == Contig;      }
	Index rank()                              const { return Rank;                  }
	T*  data()                                         { return _storage.data();       }

	/***********************************************/

	void view(Derived& other)
	{
#ifndef NDEBUG
		internal::debug::is_allocated(allocated(),associated());
#endif
		mapView(other.data(),other.get_dim(),other.get_str());
	}

	template<typename... indices>
	Alloc<T, internal::count_slice<indices...>::count,
	      Order | UnAligned | internal::contig_view<Order,Stride,indices...>::stride>
	      view(indices... idx)
	{
		constexpr Index newRank = internal::count_slice<indices...>::count;
		constexpr Index newStride = internal::contig_view<Order,Stride,indices...>::stride;
		static_assert(newRank > 0, "VIEW IS OF RANK ZERO");
		static_assert(sizeof...(idx) == Rank,
		              "NUMBER OF INDICES PASSES TO VIEW DOES NOT MATCH RANK OF ARRAY");

		Alloc<T,newRank, Order | UnAligned | newStride> S;

		std::array<Index,Rank> beg;
		std::array<Index,newRank> len;
		std::array<Index,newRank> str;

		internal::set_data<Rank, newRank,0,0>(_dim, beg, len, str, idx...);
		S.mapView(&_storage[offset(beg)],len,str);

		return S;

	}

	void mapView(T* p,
	             const std::array<Index,Rank>& dim,
	             const std::array<Index,Rank>& str)
	{
		_storage.map(p,internal::product<Rank>(dim));
		_dim = dim;
		_str = str;
	}


	/*******************************************************/


};

}; // end namespace FortCpp

#endif
