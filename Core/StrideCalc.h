#ifndef FortCpp_STRIDECALC_H
#define FortCpp_STRIDECALC_H

namespace FortCpp
{

namespace internal
{

template<typename idx>
struct is_slice {
	static const bool value = std::is_base_of<internal::SliceBase,idx>::value;
};

template<typename idx>
struct is_full_slice_helper
{
	static const bool is_full = false;
};

template<typename B, typename E, typename S>
struct is_full_slice_helper<internal::Slice<B,E,S>>
{
	static const bool is_full = traits<internal::Slice<B,E,S>>::is_full;
};

template<typename idx>
struct is_full_slice {
	static const bool value = is_full_slice_helper<idx>::is_full ? true : false;
};

/*** Counts the number of slices in a view ***/
template<typename... indices>
struct count_slice {
	static const Index count = (is_slice<indices>::value + ...);
};

/*** Determines if a slice is contiguous or not ***/
template<Index Order,Index Stride,typename... indices> struct contig_view;
template<Index Order,bool Full, typename... indices> struct contig_view_cont;

template<Index Order,typename... indices>
struct contig_view<Order,Strided,indices...> {
	static const Index stride = Strided;
};

/******* COLUMN MAJOR *********/

template<typename head>
struct contig_view<ColMajor,Contig,head> {
	static const Index stride = is_full_slice<head>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view<ColMajor,Contig,head,indices...> {
	static const Index stride = is_full_slice<head>::value ?
	                            contig_view_cont<ColMajor,1,indices...>::stride : Strided;
};

template<typename head>
struct contig_view_cont<ColMajor,1,head> {
	static const Index
	stride = is_full_slice<head>::value ? Contig :
	         is_slice<head>::value ? Strided : Contig;
};

template<typename head,typename... indices>
struct contig_view_cont<ColMajor,1,head,indices...> {
	static const Index
	stride = is_full_slice<head>::value ?
	         contig_view_cont<ColMajor,1,indices...>::stride :
	         is_slice<head>::value ? Strided :
	         contig_view_cont<ColMajor,0,indices...>::stride;
};

template<typename head>
struct contig_view_cont<ColMajor,0,head> {
	static const Index
	stride = is_slice<head>::value ? Strided : Contig;
};

template<typename head,typename... indices>
struct contig_view_cont<ColMajor,0,head,indices...> {
	static const Index
	stride = is_slice<head>::value ? Strided :
	         contig_view_cont<ColMajor,0,indices...>::stride;
};

/******* ROW MAJOR *********/

template<typename head>
struct contig_view<RowMajor,Contig,head> {
	static const Index
	stride = is_slice<head>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view<RowMajor,Contig,head,indices...> {
	static const Index
	stride = !is_slice<head>::value ?
	         contig_view_cont<RowMajor,0,indices...>::stride :
	         is_full_slice<head>::value ?
	         contig_view_cont<RowMajor,1,indices...>::stride :
	         Strided;
};

template<typename head>
struct contig_view_cont<RowMajor,1,head> {
	static const Index
	stride = is_full_slice<head>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view_cont<RowMajor,1,head,indices...> {
	static const Index
	stride = is_full_slice<head>::value ?
	         contig_view_cont<RowMajor,1,indices...>::stride :
	         Strided;
};

template<typename head>
struct contig_view_cont<RowMajor,0,head> {
	static const Index
	stride = is_full_slice<head>::value ? Contig : Strided;
};

template<typename head,typename... indices>
struct contig_view_cont<RowMajor,0,head,indices...> {
	static const Index
	stride = is_full_slice<head>::value ?
	         contig_view_cont<RowMajor,1,indices...>::stride :
	         is_slice<head>::value ?  Strided :
	         contig_view_cont<RowMajor,0,indices...>::stride;
};

/***** SET_LEN ********/

template<Index Rank,Index nRank,Index D,Index nD,typename Head, typename... indices>
void set_data(const std::array<Index,Rank>& dim,
		      const std::array<Index,Rank>& ostr,
              std::array<Index,Rank>& beg,
              std::array<Index,nRank>& len,
			  std::array<Index,nRank>& str,
              Head& i,
              indices& ... idx)
{
	if constexpr ( is_slice<Head>::value ) {
		i.set_data(dim[D], ostr[D], beg[D], len[nD], str[nD]);
		if constexpr ( sizeof...(idx) > 0 ) {
			set_data<Rank,nRank,D+1,nD+1>(dim, ostr, beg, len, str, idx...);
		}
	}
	else {
		beg[D] = static_cast<Index>(i);
		if constexpr ( sizeof...(idx) > 0 ) {
			set_data<Rank,nRank,D+1,nD>(dim, ostr, beg, len, str, idx...);
		}
	}
}

/***** SET_BEG *******/

inline
Index get_beg(const Index& i)
{
	return static_cast<Index>(i);
}

template<typename B, typename E, typename S>
Index get_beg(const internal::Slice<B,E,S>& i)
{
	return i.get_beg();
}


template<Index Rank,Index D,typename head, typename... indices>
void set_beg(std::array<Index,Rank>& beg, head& i, const indices& ... idx)
{
	beg[D] = get_beg(i);
	if constexpr ( sizeof...(idx) > 0 ) {
		set_beg<Rank,D+1>(beg,idx...);
	}
}

/******* SET_STR *************/

template<Index Rank,Index nRank,Index D,Index nD,typename Head, typename... indices>
void set_str(const std::array<Index,Rank>& str,
             std::array<Index,nRank>& nstr,
             Head& i,
             indices... idx)
{
	if constexpr ( is_slice<Head>::value ) {
		nstr[nD] = i.str*str[D];
		if constexpr ( sizeof...(idx) > 0 ) {
			set_str<Rank,nRank,D+1,nD+1>(str,nstr,idx...);
		}
	}
	else {
		if constexpr ( sizeof...(idx) > 0 ) {
			set_str<Rank,nRank,D+1,nD>(str,nstr,idx...);
		}
	}

}

}; // end namespace internal

}; // end namespace FortCpp

#endif

