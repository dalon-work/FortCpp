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
void is_allocated(bool alloc,bool assoc)
{
	if ( alloc ) {
		throw ArrayException("ARRAY IS ALREADY ALLOCATED!");
	}
	if ( assoc ) {
		throw ArrayException("ARRAY IS ALREADY ASSOCIATED!");
	}
}

/********** IS_NEGATIVE ***********/

template<Index D>
inline
void is_negative(Index i)
{
	if ( i < 0 ) {
		throw NegIdxException(i,D);
	}
}

template<Index D,typename... indices>
inline
void is_negative(Index i,indices... idx)
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
void in_size(Index i,Index s)
{
	if ( i < 0 || i >= s ) {
		throw SizeException(i,s);
	}
}

/******** IN_BOUNDS *******/

template<Index D,Index Rank>
inline
void in_bounds(const std::array<Index,Rank>& dim,Index i)
{
	if ( i >= dim[D] ) {
		throw BoundsException(i,dim[D],D);
	}
}

template<Index D,Index Rank,typename... indices>
inline
void in_bounds(const std::array<Index,Rank>& dim,Index i,indices... idx)
{
	if ( i >= dim[D] ) {
		throw BoundsException(i,dim[D],D);
	}
	in_bounds<D+1,Rank>(dim,idx...);
}

template<Index Rank>
inline
void in_rank(Index i)
{
	if ( i < 1 || i > Rank) {
		throw RankException(i,Rank);
	}
}

template<Index r,Index Rank>
inline
void in_rank()
{
	static_assert(r > 0,"Invalid dimension in size request");
	static_assert(r <= Rank,"Invalid dimension in size request");
}

/******* COMPARE_DIMS ***************/

template<Index Rank>
inline
void compare_dims(const std::array<Index,Rank>& lhs,
                  const std::array<Index,Rank>& rhs)
{
	for (Index r=0; r<Rank; r++) {
		if (lhs[r] != rhs[r]) {
			throw DimException(lhs[r],rhs[r],r);
		}
	}
}

template<typename Lhs,typename T,Index... dims>
inline
void compare_dims(const Lhs& lhs, const Fixed<T,dims...>& rhs);

template<typename Lhs,typename T,Index Rank,Index Options>
inline
void compare_dims(const Lhs& lhs, const Alloc<T,Rank,Options>& rhs);

template<typename Lhs,typename Lhs2,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs, const BinaryOp<Lhs2,Rhs,Op>& rhs);

template<typename Lhs,Index Side,typename Lhs2,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs,const ConstantOp<Side,Lhs2,Rhs,Op>& rhs);

template<typename Lhs,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs,const UnaryOp<Rhs,Op>& rhs);

/**************************/

template<typename Lhs,typename T,Index... dims>
inline
void compare_dims(const Lhs& lhs, const Fixed<T,dims...>& rhs)
{
	compare_dims<internal::traits<Lhs>::Rank>(lhs.get_dim(),rhs.get_dim());
}

template<typename Lhs,typename T,Index Rank,Index Options>
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

template<typename Lhs,Index Side,typename Lhs2,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs,const ConstantOp<Side,Lhs2,Rhs,Op>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}

template<typename Lhs,typename Rhs,typename Op>
inline
void compare_dims(const Lhs& lhs,const UnaryOp<Rhs,Op>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}

template<typename Lhs, typename Rhs,typename ReturnType,typename FuncType>
void compare_dims(const Lhs& lhs, const FuncTypeOp<Rhs, ReturnType, FuncType>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}

template<typename Lhs,typename Rhs, typename ReturnType,typename FuncType>
void compare_dims(const FuncTypeOp<Lhs, ReturnType, FuncType>& lhs, const Rhs& rhs)
{
	compare_dims(lhs.getExpr(),rhs);
}



}; // end namespace debug

}; // end namespace internal

}; // end namespace FortCpp

#endif
