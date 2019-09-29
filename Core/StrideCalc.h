#ifndef FortCpp_STRIDECALC_H
#define FortCpp_STRIDECALC_H

namespace FortCpp
{

namespace internal
{

template<typename idx> struct is_sliceImpl        { static const bool value = 0; };
template<   > struct is_sliceImpl<FullSlice     > { static const bool value = 1; };
template<   > struct is_sliceImpl<ContigSlice   > { static const bool value = 1; };
template<   > struct is_sliceImpl<StridedSlice  > { static const bool value = 1; };

template<typename idx>
struct is_slice {
	static const bool value = is_sliceImpl<typename std::remove_reference<idx>::type>::value;
};

/*** Counts the number of slices in a view ***/
template<typename front,typename... indices> struct count_slice;

template<typename tail>
struct count_slice<tail> {
	static const int count = is_slice<tail>::value;
};

template<typename head,typename... indices>
struct count_slice {
	static const int
	count = is_slice<head>::value + count_slice<indices...>::count;
};

/*** Determines if a slice is contiguous or not ***/
template<int Order,int Stride,typename... indices> struct contig_view;
template<int Order,bool Full,typename... indices> struct contig_view_cont;

template<int Order,typename... indices>
struct contig_view<Order,Strided,indices...> {
	static const int
	stride = Strided;
};

/******* COLUMN MAJOR *********/

template<typename head>
struct contig_view<ColMajor,Contig,head> {
	static const int
	stride = std::is_same<head,FullSlice>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view<ColMajor,Contig,head,indices...> {
	static const int
	stride = std::is_same<head,FullSlice>::value ?
	         contig_view_cont<ColMajor,1,indices...>::stride : Strided;
};

template<typename head>
struct contig_view_cont<ColMajor,1,head> {
	static const int
	stride = std::is_same<head,FullSlice>::value ? Contig :
	         is_slice<head>::value ? Strided : Contig;
};

template<typename head,typename... indices>
struct contig_view_cont<ColMajor,1,head,indices...> {
	static const int
	stride = std::is_same<head,FullSlice>::value ?
	         contig_view_cont<ColMajor,1,indices...>::stride :
	         is_slice<head>::value ? Strided :
	         contig_view_cont<ColMajor,0,indices...>::stride;
};

template<typename head>
struct contig_view_cont<ColMajor,0,head> {
	static const int
	stride = is_slice<head>::value ? Strided : Contig;
};

template<typename head,typename... indices>
struct contig_view_cont<ColMajor,0,head,indices...> {
	static const int
	stride = is_slice<head>::value ? Strided :
	         contig_view_cont<ColMajor,0,indices...>::stride;
};

/******* ROW MAJOR *********/

template<typename head>
struct contig_view<RowMajor,Contig,head> {
	static const int
	stride = is_slice<head>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view<RowMajor,Contig,head,indices...> {
	static const int
	stride = !is_slice<head>::value ?
	         contig_view_cont<RowMajor,0,indices...>::stride :
	         std::is_same<head,FullSlice>::value ?
	         contig_view_cont<RowMajor,1,indices...>::stride :
	         Strided;
};

template<typename head>
struct contig_view_cont<RowMajor,1,head> {
	static const int
	stride = std::is_same<head,FullSlice>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view_cont<RowMajor,1,head,indices...> {
	static const int
	stride = std::is_same<head,FullSlice>::value ?
	         contig_view_cont<RowMajor,1,indices...>::stride :
	         Strided;
};

template<typename head>
struct contig_view_cont<RowMajor,0,head> {
	static const int
	stride = std::is_same<head,FullSlice>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view_cont<RowMajor,0,head,indices...> {
	static const int
	stride = std::is_same<head,FullSlice>::value ?
	         contig_view_cont<RowMajor,1,indices...>::stride :
	         is_slice<head>::value ?  Strided :
	         contig_view_cont<RowMajor,0,indices...>::stride;
};

/***** SET_LEN ********/

// template<int Rank,int nRank,int D,int nD,typename... indices>
// void set_len(const std::array<int,Rank>& dim,
//       std::array<int,nRank>& len,
//       int& i,
//       indices... idx);
template<int Rank,int nRank,int D,int nD,typename... indices>
void set_len(const std::array<int,Rank>& dim,
             std::array<int,nRank>& len,
             SliceBase& i,
             indices& ... idx);

template<int Rank,int nRank,int D,int nD>
void set_len(const std::array<int,Rank>& dim,
             const std::array<int,nRank>& len)
{
}

template<int Rank,int nRank,int D,int nD,typename... indices>
void set_len(const std::array<int,Rank>& dim,
             std::array<int,nRank>& len,
             int& i,
             indices& ... idx)
{
	set_len<Rank,nRank,D+1,nD>(dim,len,idx...);
}

template<int Rank,int nRank,int D,int nD,typename... indices>
void set_len(const std::array<int,Rank>& dim,
             std::array<int,nRank>& len,
             SliceBase& i,
             indices& ... idx)
{
	len[nD] = i.len(dim[D]);
	set_len<Rank,nRank,D+1,nD+1>(dim,len,idx...);
}

/***** SET_BEG *******/

static inline
int get_beg(const int& i)
{
	return static_cast<int>(i);
}

static inline
int get_beg(const SliceBase& i)
{
	return static_cast<int>(i.beg);
}

template<int Rank,int D,typename tail>
void set_beg(std::array<int,Rank>& beg,tail& i)
{
	beg[D] = get_beg(i);
}

template<int Rank,int D,typename head, typename... indices>
void set_beg(std::array<int,Rank>& beg,head& i,const indices& ... idx)
{
	beg[D] = get_beg(i);
	set_beg<Rank,D+1>(beg,idx...);
}

/******* SET_STR *************/

// template<int Rank,int newRank,int D,int nD,typename... indices>
// void set_str(const std::array<int,Rank>& str,
//       std::array<int,newRank>& nstr,
//       int& i,
//       indices... idx);
template<int Rank,int nRank,int D,int nD,typename... indices>
void set_str(const std::array<int,Rank>& str,
             std::array<int,nRank>& nstr,
             SliceBase& i,
             indices... idx);


template<int Rank,int nRank,int D,int nD>
void set_str(const std::array<int,Rank>& str,
             std::array<int,nRank>& nstr)
{
}

template<int Rank,int nRank,int D,int nD,typename... indices>
void set_str(const std::array<int,Rank>& str,
             std::array<int,nRank>& nstr,
             int& i,
             indices... idx)
{
	set_str<Rank,nRank,D+1,nD>(str,nstr,idx...);
}

template<int Rank,int nRank,int D,int nD,typename... indices>
void set_str(const std::array<int,Rank>& str,
             std::array<int,nRank>& nstr,
             SliceBase& i,
             indices... idx)
{
	nstr[nD] = i.str*str[D];
	set_str<Rank,nRank,D+1,nD+1>(str,nstr,idx...);
}





/** Struct to extract a given dimension and compute total stride**/
// template<typename ArrayDerived,int Dim> struct compute_stride;
// template<typename ArrayDerived>
// struct compute_stride<ArrayDerived,0>
// {
//   enum{
//     res = 1
//   };
// };
// template<typename ArrayDerived>
// struct compute_stride<ArrayDerived,1>
// {
//   enum{
//     D = traits<ArrayDerived>::D1,
//     res = (int(D) == int(Unknown)) ? int(Unknown) : int(D)
//   };
// };
// template<typename ArrayDerived>
// struct compute_stride<ArrayDerived,2>
// {
//   enum{
//     D = traits<ArrayDerived>::D2,
//     res = (int(D) == int(Unknown)) ? int(Unknown) : int(D)*int(compute_stride<ArrayDerived,1>::res)
//   };
// };
// template<typename ArrayDerived>
// struct compute_stride<ArrayDerived,3>
// {
//   enum{
//     D = traits<ArrayDerived>::D3,
//     res = (int(D) == int(Unknown)) ? int(Unknown) : int(D)*int(compute_stride<ArrayDerived,2>::res)
//   };
// };
// template<typename ArrayDerived>
// struct compute_stride<ArrayDerived,4>
// {
//   enum{
//     D = traits<ArrayDerived>::D4,
//     res = (int(D) == int(Unknown)) ? int(Unknown) : int(D)*int(compute_stride<ArrayDerived,3>::res)
//   };
// };
// template<typename ArrayDerived>
// struct compute_stride<ArrayDerived,5>
// {
//   enum{
//     D = traits<ArrayDerived>::D5,
//     res = (int(D) == int(Unknown)) ? int(Unknown) : int(D)*int(compute_stride<ArrayDerived,4>::res)
//   };
// };
//
// #<{(|* Structure to compute the stride of an array from a slice *|)}>#
// template<typename ArrayDerived,typename SliceDerived> struct compute_slice_stride
// {
//   enum{
//     Dim = traits<SliceDerived>::Dim,
//     S = (int(traits<SliceDerived>::S) == int(Unknown)) ? int(Unknown)
//       : (int(compute_stride<ArrayDerived,Dim-1>::res) < 0) ? int(Unknown)
//       : int(compute_stride<ArrayDerived,Dim-1>::res*traits<SliceDerived>::S)
//   };
// };
//
// #<{(|* Structure to compute the length in a single dimension of an array from a slice *|)}>#
// template<typename ArrayDerived,typename SliceDerived> struct compute_slice_length
// {
//   enum{
//     Dim = traits<SliceDerived>::Dim,
//     Type  = traits<SliceDerived>::Type,
//     sL  = traits<SliceDerived>::L,
//     D   = compute_stride<ArrayDerived,Dim>::D,
//     L = (int(Type) == int(Full)) ? (int(D) != int(Unknown)) ? D : (int(sL) != int(Unknown)) ? int(sL) : int(Unknown) : (int(sL) != int(Unknown)) ? int(sL) : int(Unknown)
//   };
// };
//
// #<{(|* Structure for runtime computation of length *|)}>#
// template<int SliceType,int Dim,typename AccessorDerived,typename SliceDerived> struct runtime_length;
//
// template<typename AccessorDerived,typename SliceDerived>
// struct runtime_length<Full,1,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S)
//   { return A.d1();}
// };
// template<typename AccessorDerived,typename SliceDerived>
// struct runtime_length<Full,2,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S)
//   { return A.d2();}
// };
// template<typename AccessorDerived,typename SliceDerived>
// struct runtime_length<Full,3,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S)
//   { return A.d3();}
// };
// template<typename AccessorDerived,typename SliceDerived>
// struct runtime_length<Full,4,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S)
//   { return A.d4();}
// };
// template<typename AccessorDerived,typename SliceDerived>
// struct runtime_length<Full,5,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S)
//   { return A.d5();}
// };
// template<int SliceType,int Dim,typename AccessorDerived,typename SliceDerived>
// struct runtime_length
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return S.l()/S.s()+S.l()%S.s();
//   }
// };
//
// #<{(|* Structure to compute total strides at runtime *|)}>#
// template<int Stride,int Dim, typename AccessorDerived, typename SliceDerived> struct runtime_stride;
//
// template<typename AccessorDerived, typename SliceDerived>
// struct runtime_stride<Unknown,1,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return S.s();
//   }
// };
// template<typename AccessorDerived, typename SliceDerived>
// struct runtime_stride<Unknown,2,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return S.s()*A.d1();
//   }
// };
// template<typename AccessorDerived, typename SliceDerived>
// struct runtime_stride<Unknown,3,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return S.s()*A.d1()*A.d2();
//   }
// };
// template<typename AccessorDerived, typename SliceDerived>
// struct runtime_stride<Unknown,4,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return S.s()*A.d1()*A.d2()*A.d3();
//   }
// };
// template<typename AccessorDerived, typename SliceDerived>
// struct runtime_stride<Unknown,5,AccessorDerived,SliceDerived>
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return S.s()*A.d1()*A.d2()*A.d3()*A.d4();
//   }
// };
// template<int Stride,int Dim,typename AccessorDerived, typename SliceDerived>
// struct runtime_stride
// {
//   static inline const int result(const AccessorDerived &A, const SliceDerived &S){
//     return Stride;
//   }
// };

}; // end namespace internal

}; // end namespace FortCpp

#endif

