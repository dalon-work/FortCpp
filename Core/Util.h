#ifndef FortCpp_UTIL_H
#define FortCpp_UTIL_H

namespace FortCpp
{

namespace internal
{


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
		unsigned o=str[0]*(i%dim[0]);
      unsigned d=dim[0];
      for(int r=1;r<Rank-1;r++){
         o += str[r]*((i/d)%dim[r]);
         d *= dim[r];
      }
      o += str[Rank-1]*(i/d);
		return o;
	}
};

template<unsigned Rank>
struct linear_index<RowMajor,Strided,Rank> {
	static unsigned exec(const std::array<unsigned,Rank>& dim,
	          const std::array<unsigned,Rank>& str,
	          unsigned i) {
		unsigned o = str[Rank-1]*(i%dim[Rank-1]);
		unsigned d=dim[Rank-1];
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
