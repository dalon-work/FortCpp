#ifndef FortCpp_UTIL_H
#define FortCpp_UTIL_H

namespace FortCpp
{

namespace internal
{


/*********** PRODUCT ***********/

constexpr inline
int product(int i)
{
	return i;
}

template<typename... indices>
constexpr inline
int product(int i,indices... idx)
{
	return i*product(idx...);
}

template<int Rank>
inline
int product(const std::array<int,Rank>& idx)
{
	int s=1;
	for (int r=0; r<Rank; r++) {
		s *= idx[r];
	}
	return s;
}

/********** SET_ARRAY **********/

template<int Rank,int D>
void set_array(std::array<int,Rank>& A,int i)
{
	A[D] = i;
}

template<int Rank,int D,typename... indices>
void set_array(std::array<int,Rank>& A,int i,indices... idx)
{
	A[D] = i;
	set_array<Rank,D+1>(A,idx...);
}

/******* OFFSET *******/

template<int Rank,int D>
int offset(const std::array<int,Rank>& str,int i)
{
	return  i*str[D];
}

template<int Rank,int D,typename... indices>
int offset(const std::array<int,Rank>& str,int i,indices... idx)
{
	return i*str[D]+offset<Rank,D+1>(str,idx...);
}


/*********** COMPUTE_STRIDES **********/

template<int Order,int Rank> struct compute_strides;

template<int Rank>
struct compute_strides<ColMajor,Rank> {
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
	static int exec(const std::array<int,Rank>& dim,
	          const std::array<int,Rank>& str,
	          int i) {
		return i;
	}
};

template<int Rank>
struct linear_index<ColMajor,Strided,Rank> {
	static int exec(const std::array<int,Rank>& dim,
	          const std::array<int,Rank>& str,
	          int i) {
		int o=str[0]*(i%dim[0]);
      int d=dim[0];
      for(int r=1;r<Rank-1;r++){
         o += str[r]*((i/d)%dim[r]);
         d *= dim[r];
      }
      o += str[Rank-1]*(i/d);
		return o;
	}
};

template<int Rank>
struct linear_index<RowMajor,Strided,Rank> {
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
