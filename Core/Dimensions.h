#ifndef FortCpp_DIMENSIONS_H
#define FortCpp_DIMENSIONS_H
#include "ForwardDeclarations.h"
#include <array>

namespace FortCpp
{

namespace internal
{


/************ DIMENSIONS *******************/

template<int Rank,int Order,int Stride>
class Dimensions
{
private:

	// offset computes the index location of
	// an element in a MD Array

public:

	Dimensions()=default;
	Dimensions(const Dimensions& other)=delete;
	Dimensions(const Dimensions&& other)=delete;
	~Dimensions()=default;

	// set_dim sets the dimensions of the array
	template<typename... indices>
	void set_dim(indices... idx) {
		copy_dim<0>(idx...);
		str.compute_strides(dim);
		s = product(idx...);
	}

	void unset() {
		for (int r=0; r<Rank; r++) {
			dim[r]=0;
			str.str[r]=0;
		}
		s=0;
	}

	void copy_dim(const std::array<int,Rank>& otherdim,
	              const std::array<int,Rank>& otherstr) {
		dim = otherdim;
		str.str = otherstr;
		s = product<Rank>(dim);
	}


	// computes the offset for a MD index
	template<typename... indices>
	constexpr int operator () (indices... idx) const {
		return offset<0>(idx...);
	}

	constexpr int operator [] (const int i) const {return str.offset(dim,i);}

	constexpr int size() const {
		return s;
	}

	constexpr int size(int D) const {
		return dim[D-1];
	}

	constexpr int stride(int D) const {
		return str[D-1];
	}

	const std::array<int,Rank>& get_dim() const {
		return dim;
	}

	const std::array<int,Rank>& get_str() const {
		return str.str;
	}

};

/***************** EQUALITY ********************/

template<int LRank, int LOrder, int LStride,
         int RRank, int ROrder, int RStride>
bool operator == (const Dimensions<LRank,LOrder,LStride>& lhs,
                  const Dimensions<RRank,ROrder,RStride>& rhs)
{
	static_assert(LRank == RRank,
	              "ARRAY EXPRESSION RANKS NOT EQUAL");
	static_assert(LOrder == ROrder,
	              "CANNOT MIX COLUMN AND ROW MAJOR ORDER IN ARRAY EXPRESSIONS");
	for (int i=1; i<=LRank; i++) {
		if ( lhs.size(i) != rhs.size(i)) {
			return 0;
		}
	}
	return 1;
}

}; // end namespace internal

}; // end namespace FortCpp





#endif
