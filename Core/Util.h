#ifndef FortCpp_UTIL_H
#define FortCpp_UTIL_H

namespace FortCpp
{

namespace internal
{

/*********** IS_ARRAY **********/

template<typename Derived>
struct is_arrayImpl { static const bool value = 0; };

template<typename T,Index Rank,Index Options>                struct is_arrayImpl<Alloc<T,Rank,Options> >       { static const bool value = 1; };
template<typename Derived>                               struct is_arrayImpl<ArrayBase<Derived> >          { static const bool value = 1; };
template<typename T,Index... dims>                         struct is_arrayImpl<Fixed<T,dims...> >            { static const bool value = 1; };
template<typename Lhs,typename Rhs,typename Op>          struct is_arrayImpl<BinaryOp<Lhs,Rhs,Op> >        { static const bool value = 1; };
template<Index Side,typename Lhs,typename Rhs,typename Op> struct is_arrayImpl<ConstantOp<Side,Lhs,Rhs,Op> > { static const bool value = 1; };
template<typename Rhs,typename Op>                       struct is_arrayImpl<UnaryOp<Rhs,Op> >             { static const bool value = 1; };

template<typename Derived>
struct is_array {
	static const bool value = is_arrayImpl<typename std::remove_reference<Derived>::type>::value;
};

/*********** PRODUCT ***********/

template<typename... indices>
constexpr
Index product(Index i,indices... idx)
{
	return i*product(idx...);
}

template<Index Rank>
Index product(const std::array<Index,Rank>& idx)
{
	Index s=1;
	for (Index r=0; r<Rank; r++) {
		s *= idx[r];
	}
	return s;
}


/********** SET_ARRAY **********/

template<Index Rank,Index D>
void set_array(std::array<Index,Rank>& A,Index i)
{
	A[D] = i;
}

template<Index Rank,Index D,typename... indices>
void set_array(std::array<Index,Rank>& A,Index i,indices... idx)
{
	A[D] = i;
	set_array<Rank,D+1>(A,idx...);
}

/******* OFFSET *******/

template<Index Rank,Index D>
constexpr
Index strided_offset(const std::array<Index,Rank>& str, Index i)
{
	return i*str[D];
}

template<Index Rank,Index D,typename... indices>
constexpr
Index strided_offset(const std::array<Index,Rank>& str,Index i,indices... idx)
{
	return i*str[D]+strided_offset<Rank,D+1>(str,idx...);
}

template<Index Rank,Index D>
constexpr
Index row_contig_offset(const std::array<Index,Rank>& str, Index i)
{
	return i;
}

template<Index Rank,Index D,typename... indices>
constexpr inline
Index row_contig_offset(const std::array<Index,Rank>& str,Index i,indices... idx)
{
	return i*str[D]+strided_offset<Rank,D+1>(str,idx...);
}


template<Index Order,Index Stride,Index Rank> struct offset;

template<>
struct offset<ColMajor,Contig,1> {
	constexpr
	static Index
	exec(const std::array<Index,1>& str,Index i)
	{
		return i;
	}
};

template<>
struct offset<RowMajor,Contig,1> {
	constexpr
	static Index
	exec(const std::array<Index,1>& str,Index i)
	{
		return i;
	}
};

template<>
struct offset<ColMajor,Strided,1> {
	constexpr inline
	static Index
	exec(const std::array<Index,1>& str,Index i)
	{
		return i*str[0];
	}
};

template<>
struct offset<RowMajor,Strided,1> {
	constexpr inline
	static Index
	exec(const std::array<Index,1>& str,Index i)
	{
		return i*str[0];
	}
};

template<Index Order,Index Rank>
struct offset<Order,Strided,Rank> {
	template<typename... indices>
	constexpr inline
	static Index
	exec(const std::array<Index,Rank>& str,indices... idx)
	{
		return strided_offset<Rank,0>(str,idx...);
	}
};

template<Index Rank>
struct offset<ColMajor,Contig,Rank> {
	template<typename... indices>
	constexpr inline
	static Index
	exec(const std::array<Index,Rank>& str,Index i,indices... idx)
	{
		return i+strided_offset<Rank,1>(str,idx...);
	}
};

template<Index Rank>
struct offset<RowMajor,Contig,Rank> {
	template<typename... indices>
	constexpr inline
	static Index
	exec(const std::array<Index,Rank>& str,indices... idx)
	{
		return row_contig_offset<Rank,0>(str,idx...);
	}
};


/*********** COMPUTE_STRIDES **********/

template<Index Order,Index Rank> struct compute_strides;

template<Index Rank>
struct compute_strides<ColMajor,Rank> {
	inline
	static void exec(std::array<Index,Rank>& str,
	                 const std::array<Index,Rank>& dim)
	{
		for (Index r=0; r<Rank-1; r++) {
			for (Index i=r+1; i<Rank; i++) {
				str[i]*=dim[r];
			}
		}
	}
};

template<Index Rank>
struct compute_strides<RowMajor,Rank> {
	inline
	static void exec(std::array<Index,Rank>& str,
	                 const std::array<Index,Rank>& dim)
	{
		for (Index r=1; r<Rank; r++) {
			for (Index i=0; i<r; i++) {
				str[i] *= dim[r];
			}
		}
	}
};

/******** COMPUTE_OFFSET ***********/

template<Index Order,Index Stride,Index Rank> struct linear_index;

template<Index Order,Index Rank>
struct linear_index<Order,Contig,Rank> {
	inline
	static Index exec(const std::array<Index,Rank>& dim,
	                  const std::array<Index,Rank>& str,
	                  Index i)
	{
		return i;
	}
};

template<Index Rank>
struct linear_index<ColMajor,Strided,Rank> {
	inline
	static Index exec(const std::array<Index,Rank>& dim,
	                  const std::array<Index,Rank>& str,
	                  Index i)
	{
		Index o=str[0]*(i%dim[0]);
		Index d=dim[0];
		for (Index r=1; r<Rank-1; r++) {
			o += str[r]*((i/d)%dim[r]);
			d *= dim[r];
		}
		o += str[Rank-1]*(i/d);
		return o;
	}
};

template<Index Rank>
struct linear_index<RowMajor,Strided,Rank> {
	inline
	static Index exec(const std::array<Index,Rank>& dim,
	                  const std::array<Index,Rank>& str,
	                  Index i)
	{
		Index o = str[Rank-1]*(i%dim[Rank-1]);
		Index d=dim[Rank-1];
		for (Index r=Rank-2; r>=1; r--) {
			o += str[r]*((i/d)%dim[r]);
			d *= dim[r];
		}
		o += str[0]*(i/d);
		return o;
	}
};




}; // end namespace internal

}; // end namespace FortCpp

#endif
