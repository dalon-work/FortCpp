#ifndef FortCpp_UTIL_H
#define FortCpp_UTIL_H

namespace FortCpp
{

namespace internal
{

/********** IS_NEGATIVE ***********/

template<unsigned D>
void is_negative(int i)
{
	if ( i < 0 ) {
      throw NegIdxException(i,D);
	}
}

template<unsigned D,typename... indices>
void is_negative(int i,indices... idx)
{
	if ( i < 0 ) {
      throw NegIdxException(i,D);
	}
	else {
		is_negative<D+1>(idx...);
	}
};

/*********** PRODUCT ***********/

constexpr inline
unsigned product(unsigned i)
{
	return i;
}

template<typename... indices>
constexpr inline
unsigned product(unsigned i,indices... idx)
{
	return i*product(idx...);
}

template<unsigned Rank>
inline
unsigned product(const std::array<unsigned,Rank>& idx)
{
	unsigned s=1;
	for (int r=0; r<Rank; r++) {
		s *= idx[r];
	}
	return s;
}

/********** SET_ARRAY **********/

template<unsigned Rank,unsigned D>
void set_array(std::array<unsigned,Rank>& A,unsigned i)
{
	A[D] = i;
}

template<unsigned Rank,unsigned D,typename... indices>
void set_array(std::array<unsigned,Rank>& A,unsigned i,indices... idx)
{
	A[D] = i;
	set_array<Rank,D+1>(A,idx...);
}

/******* OFFSET *******/

template<unsigned Rank,unsigned D>
unsigned offset(const std::array<unsigned,Rank>& str,unsigned i)
{
	return  i*str[D];
}

template<unsigned Rank,unsigned D,typename... indices>
unsigned offset(const std::array<unsigned,Rank>& str,unsigned i,indices... idx)
{
	return i*str[D]+offset<Rank,D+1>(str,idx...);
}

/******* IN_SIZE *******/

void in_size(int i,unsigned s)
{
   if( i < 0 || i >= s ){
      throw SizeException(i,s);
   }
}

/******** IN_BOUNDS *******/

template<unsigned D,unsigned Rank>
void in_bounds(std::array<unsigned,Rank> &dim,unsigned i)
{
   if( i >= dim[D] ){
      throw BoundsException(i,dim[D],D);
   }
}

template<unsigned D,unsigned Rank,typename... indices>
void in_bounds(std::array<unsigned,Rank> &dim,unsigned i,indices... idx)
{
   if( i >= dim[D] ){
      throw BoundsException(i,dim[D],D);
   }
   in_bounds<D+1,Rank>(dim,idx...);
}

template<unsigned Rank>
void in_rank(int i)
{
   if( i < 1 || i > Rank){
      throw RankException(i,Rank);
   }
}

/******* COMPARE_DIMS ***************/

template<std::size_t Rank>
void compare_dims(const std::array<unsigned,Rank>& lhs,
      const std::array<unsigned,Rank>& rhs){
   for(int r=0;r<Rank;r++){
      if(lhs[r] != rhs[r]){
         throw DimException(lhs[r],rhs[r],r);
      }
   }
}

template<typename Lhs,typename T,unsigned Rank,unsigned Options>
void compare_dims(const Lhs& lhs, const Alloc<T,Rank,Options>& rhs)
{
	compare_dims<Rank>(lhs.get_dim(),rhs.get_dim());
}

template<typename Lhs,typename Lhs2,typename Rhs,typename Op>
void compare_dims(const Lhs& lhs, const BinaryOp<Lhs2,Rhs,Op>& rhs)
{
	compare_dims(lhs,rhs.get_lhs());
   compare_dims(lhs,rhs.get_rhs());
}

template<typename Lhs,unsigned Side,typename Derived,typename Op>
void compare_dims(const Lhs& lhs,const ConstantOp<Side,Derived,Op>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}

template<typename Lhs,typename Rhs,typename Op>
void compare_dims(const Lhs& lhs,const UnaryOp<Rhs,Op>& rhs)
{
	compare_dims(lhs,rhs.getExpr());
}

/*********** COMPUTE_STRIDES **********/

template<unsigned Order,unsigned Rank> struct compute_strides;

template<unsigned Rank>
struct compute_strides<ColMajor,Rank> {
	static void exec(std::array<unsigned,Rank>& str,
	          const std::array<unsigned,Rank>& dim) {
		for (int r=0; r<Rank-1; r++) {
			for (int i=r+1; i<Rank; i++) {
				str[i]*=dim[r];
			}
		}
	}
};

template<unsigned Rank>
struct compute_strides<RowMajor,Rank> {
	static void exec(std::array<unsigned,Rank>& str,
	          const std::array<unsigned,Rank>& dim) {
		for (int r=1; r<Rank; r++) {
			for (int i=0; i<r; i++) {
				str[i] *= dim[r];
			}
		}
	}
};

/******** COMPUTE_OFFSET ***********/

template<unsigned Order,unsigned Stride,unsigned Rank> struct linear_index;

template<unsigned Order,unsigned Rank>
struct linear_index<Order,Contig,Rank> {
	static unsigned exec(const std::array<unsigned,Rank>& dim,
	          const std::array<unsigned,Rank>& str,
	          unsigned i) {
		return i;
	}
};

template<unsigned Rank>
struct linear_index<ColMajor,Strided,Rank> {
	static unsigned exec(const std::array<unsigned,Rank>& dim,
	          const std::array<unsigned,Rank>& str,
	          unsigned i) {
		unsigned o= str[0]*(i%dim[0]);
		unsigned d=1;
		for (int r=1; r<Rank; r++) {
			d *= dim[r-1];
			o += str[r]*((i/d)*dim[r]);
		}
		return o;
	}
};

template<unsigned Rank>
struct linear_index<RowMajor,Strided,Rank> {
	static unsigned exec(const std::array<unsigned,Rank>& str,
	          const std::array<unsigned,Rank>& dim,
	          unsigned i) {
		unsigned o = str[Rank-1]*(i%dim[Rank-1]);
		unsigned d=1;
		for (int r=Rank-1; r>=1; r--) {
			d *= dim[r];
			o += str[r-1]*((i/d)*dim[r-1]);
		}
		return o;
	}
};




}; // end namespace internal

}; // end namespace FortCpp

#endif
