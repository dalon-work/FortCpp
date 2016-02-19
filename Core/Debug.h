#ifndef FortCpp_DEBUG_H
#define FortCpp_DEBUG_H

namespace FortCpp
{

namespace internal
{

namespace debug
{

/******** IS_ALLOCATED ***********/

inline
void is_allocated(bool alloc)
{
	if ( alloc ) {
		throw ArrayException("ARRAY IS ALREADY ALLOCATED!");
	}
}

/********** IS_NEGATIVE ***********/

template<int D>
inline
void is_negative(int i)
{
	if ( i < 0 ) {
		throw NegIdxException(i,D);
	}
}

template<int D,typename... indices>
inline
void is_negative(int i,indices... idx)
{
	if ( i < 0 ) {
		throw NegIdxException(i,D);
	}
	else {
		is_negative<D+1>(idx...);
	}
};

/******* IN_SIZE *******/

inline
void in_size(int i,int s)
{
	if ( i < 0 || i >= s ) {
		throw SizeException(i,s);
	}
}

/******** IN_BOUNDS *******/

template<int D,int Rank>
inline
void in_bounds(const std::array<int,Rank>& dim,int i)
{
	if ( i >= dim[D] ) {
		throw BoundsException(i,dim[D],D);
	}
}

template<int D,int Rank,typename... indices>
inline
void in_bounds(const std::array<int,Rank>& dim,int i,indices... idx)
{
	if ( i >= dim[D] ) {
		throw BoundsException(i,dim[D],D);
	}
	in_bounds<D+1,Rank>(dim,idx...);
}

template<int Rank>
inline
void in_rank(int i)
{
	if ( i < 1 || i > Rank) {
		throw RankException(i,Rank);
	}
}

template<int r,int Rank>
inline
void in_rank()
{
	static_assert(r > 0,"Invalid dimension in size request");
	static_assert(r <= Rank,"Invalid dimension in size request");
}

/******* COMPARE_DIMS ***************/

template<std::size_t Rank>
inline
void compare_dims(const std::array<int,Rank>& lhs,
                  const std::array<int,Rank>& rhs)
{
	for (int r=0; r<Rank; r++) {
		if (lhs[r] != rhs[r]) {
			throw DimException(lhs[r],rhs[r],r);
		}
	}
}

template<typename Lhs,typename T,int... dims>
inline
void compare_dims(const Lhs& lhs, const Fixed<T,dims...>& rhs)
{
	compare_dims<internal::traits<Lhs>::Rank>(lhs.get_dim(),rhs.get_dim());
}

template<typename Lhs,typename T,int Rank,int Options>
inline
void compare_dims(const Lhs& lhs, const Alloc<T,Rank,Options>& rhs)
{
	compare_dims<Rank>(lhs.get_dim(),rhs.get_dim());
}

template<typename Lhs,typename Lhs2,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs, const BinaryOp<Lhs2,Rhs,Op>& rhs)
{
	compare_dims(lhs,rhs.get_lhs());
	compare_dims(lhs,rhs.get_rhs());
}

template<typename Lhs,int Side,typename Derived,typename Op>
inline
void compare_dims(const Lhs& lhs,const ConstantOp<Side,Derived,Op>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}

template<typename Lhs,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs,const UnaryOp<Rhs,Op>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}



}; // end namespace debug

}; // end namespace internal

}; // end namespace FortCpp

#endif
