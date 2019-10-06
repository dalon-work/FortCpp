#ifndef FortCpp_UTIL_H
#define FortCpp_UTIL_H

namespace FortCpp
{

namespace internal
{

/*********** IS_ARRAY **********/

template<typename Derived>
struct is_arrayImpl { static const bool value = 0; };

template<typename T,int Rank,int Options>                struct is_arrayImpl<Alloc<T,Rank,Options> >       { static const bool value = 1; };
template<typename Derived>                               struct is_arrayImpl<ArrayBase<Derived> >          { static const bool value = 1; };
template<typename T,int... dims>                         struct is_arrayImpl<Fixed<T,dims...> >            { static const bool value = 1; };
template<typename Lhs,typename Rhs,typename Op>          struct is_arrayImpl<BinaryOp<Lhs,Rhs,Op> >        { static const bool value = 1; };
template<int Side,typename Lhs,typename Rhs,typename Op> struct is_arrayImpl<ConstantOp<Side,Lhs,Rhs,Op> > { static const bool value = 1; };
template<typename Rhs,typename Op>                       struct is_arrayImpl<UnaryOp<Rhs,Op> >             { static const bool value = 1; };

template<typename Derived>
struct is_array {
	static const bool value = is_arrayImpl<typename std::remove_reference<Derived>::type>::value;
};

/*********** PRODUCT ***********/

template<typename... indices>
constexpr inline
int product(indices... idx)
{
	return ( idx * ... );
}

template<size_t Rank, size_t... I>
int product_impl(const std::array<int,Rank>& idx, std::index_sequence<I...>) {
   return ( idx[I] * ... );
}

template<size_t Rank, typename Indices = std::make_index_sequence<Rank>>
inline
int product(const std::array<int,Rank>& idx)
{
   return product_impl(idx, Indices{});
}


/******* OFFSET *******/

template<int Rank,int D>
constexpr inline
unsigned strided_offset(const std::array<int,Rank>& str, int i)
{
	return i*str[D];
}

template<size_t Rank,int D,typename... indices>
constexpr inline
unsigned strided_offset(const std::array<int,Rank>& str,int i,indices... idx)
{
	return i*str[D]+strided_offset<Rank,D+1>(str,idx...);
}

template<int Rank,int D>
constexpr inline
unsigned row_contig_offset(const std::array<int,Rank>& str, int i)
{
	return i;
}

template<int Rank,int D,typename... indices>
constexpr inline
unsigned row_contig_offset(const std::array<int,Rank>& str,int i,indices... idx)
{
	return i*str[D]+strided_offset<Rank,D+1>(str,idx...);
}


template<int Order,int Stride,int Rank> struct offset;

template<>
struct offset<ColMajor,Contig,1> {
	constexpr inline
	static unsigned
	exec(const std::array<int,1>& str,int i) {
		return i;
	}
};

template<>
struct offset<RowMajor,Contig,1> {
	constexpr inline
	static unsigned
	exec(const std::array<int,1>& str,int i) {
		return i;
	}
};

template<>
struct offset<ColMajor,Strided,1> {
	constexpr inline
	static unsigned
	exec(const std::array<int,1>& str,int i) {
		return i*str[0];
	}
};

template<>
struct offset<RowMajor,Strided,1> {
	constexpr inline
	static unsigned
	exec(const std::array<int,1>& str,int i) {
		return i*str[0];
	}
};

template<int Order,int Rank>
struct offset<Order,Strided,Rank> {
	template<typename... indices>
	constexpr inline
	static unsigned
	exec(const std::array<int,Rank>& str,indices... idx) {
		return strided_offset<Rank,0>(str,idx...);
	}
};

template<int Rank>
struct offset<ColMajor,Contig,Rank> {
	template<typename... indices>
	constexpr inline
	static unsigned
	exec(const std::array<int,Rank>& str,int i,indices... idx) {
		return i+strided_offset<Rank,1>(str,idx...);
	}
};

template<int Rank>
struct offset<RowMajor,Contig,Rank> {
	template<typename... indices>
	constexpr inline
	static unsigned
	exec(const std::array<int,Rank>& str,indices... idx) {
		return row_contig_offset<Rank,0>(str,idx...);
	}
};


/*********** COMPUTE_STRIDES **********/

template<int Order,int Rank> struct compute_strides;

template<int Rank>
struct compute_strides<ColMajor,Rank> {
	inline
	static void exec(std::array<int,Rank>& str,
	                 const std::array<int,Rank>& dim) {
		for (int r=0; r<Rank-1; r++) {
			for (int i=r+1; i<Rank; i++) {
				str[i]*=dim[r];
			}
		}
	}
};

template<int Rank>
struct compute_strides<RowMajor,Rank> {
	inline
	static void exec(std::array<int,Rank>& str,
	                 const std::array<int,Rank>& dim) {
		for (int r=1; r<Rank; r++) {
			for (int i=0; i<r; i++) {
				str[i] *= dim[r];
			}
		}
	}
};

/******** COMPUTE_OFFSET ***********/

template<int Order,int Stride,int Rank> struct linear_index;

template<int Order,int Rank>
struct linear_index<Order,Contig,Rank> {
	inline
	static int exec(const std::array<int,Rank>& dim,
	                const std::array<int,Rank>& str,
	                int i) {
		return i;
	}
};

template<int Rank>
struct linear_index<ColMajor,Strided,Rank> {
	inline
	static int exec(const std::array<int,Rank>& dim,
	                const std::array<int,Rank>& str,
	                int i) {
		int o=str[0]*(i%dim[0]);
		int d=dim[0];
		for (int r=1; r<Rank-1; r++) {
			o += str[r]*((i/d)%dim[r]);
			d *= dim[r];
		}
		o += str[Rank-1]*(i/d);
		return o;
	}
};

template<int Rank>
struct linear_index<RowMajor,Strided,Rank> {
	inline
	static int exec(const std::array<int,Rank>& dim,
	                const std::array<int,Rank>& str,
	                int i) {
		int o = str[Rank-1]*(i%dim[Rank-1]);
		int d=dim[Rank-1];
		for (int r=Rank-2; r>=1; r--) {
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
