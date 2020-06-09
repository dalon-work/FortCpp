#ifndef FortCpp_SLICE_h
#define FortCpp_SLICE_h

namespace FortCpp
{

struct SliceDimBeg {};
struct SliceDimEnd {};
struct SliceDimContig {};

constexpr inline SliceDimBeg BEG;
constexpr inline SliceDimEnd END;

namespace internal
{

struct SliceBase {};
template<typename T> struct SliceBeg {};
template<typename T> struct SliceEnd {};
template<typename T> struct SliceStride {};
template<> struct SliceBeg<Index> { Index _beg; };
template<> struct SliceEnd<Index> { Index _end; };
template<> struct SliceStride<Index> { Index _str; };

template<typename _B, typename _E, typename _S>
struct traits< Slice<_B, _E, _S> > {
	using B = _B;
	using E = _E;
	using S = _S;
	enum {
		is_full = std::is_same_v<B,SliceDimBeg> && std::is_same_v<E,SliceDimEnd> && std::is_same_v<S,SliceDimContig>,
		is_contig = std::is_same_v<E,SliceDimEnd> && std::is_same_v<S,SliceDimContig>,
	};
};

template<typename B, typename E, typename S>
struct Slice : public SliceBase, SliceBeg<B>, SliceEnd<E>, SliceStride<S>
{

	constexpr Index get_beg() const {
		if constexpr ( std::is_same_v<Index, B> ) {
			return this->_beg;
		}
		else {
			return 0;
		}
	}

	/**
	 * i is the size of the given dimension
	 */
	void set_data(Index size, Index& beg, Index& len, Index& str)
	{
		Index end;

		// compute start index
		if constexpr ( std::is_same<B, SliceDimEnd>::value ) {
			beg = size-1;
		}
		else if constexpr ( std::is_same<B, SliceDimBeg>::value ) {
			beg = 0;
		}
		else {
			if (this->_beg < 0) {
				beg = size + this->_beg;
			}
			else {
				beg = this->_beg;
			}
			this->_beg = beg;
		}

		// compute end index
		if constexpr ( std::is_same<E, SliceDimEnd>::value ) {
			end = size;
		}
		else if constexpr ( std::is_same<E, SliceDimBeg>::value ) {
			end = -1;
		}
		else { 
			if (this->_end < 0) {
				end = size + this->_end;
			}
			else {
				end = this->_end;
			}
			this->_end = end;
		}

		if constexpr ( std::is_same<S, SliceDimContig>::value ) {
			str = 1;
		}
		else {
			str = this->_str;
		}

#ifndef NDEBUG
		// bounds check
		if ( beg < 0 ) {
			throw BoundSliceException(beg, size);
		}
		if ( beg >= size ) {
			throw BoundSliceException(beg, size);
		}

		// bounds check
		if ( end < -1 ) {
			throw BoundSliceException(end, size);
		}
		if ( end > size ) {
			throw BoundSliceException(end, size);
		}

		// zero slice check
		if ( (beg > end) && str > 0) {
			throw ZeroSliceException(beg,end,str);
		}
		if ( (end > beg) && str < 0) {
			throw ZeroSliceException(beg,end,str);
		}
#endif

		if ( str < 0 ) {
			len = 1+(end-beg+1)/str;
		}
		else {
			len = 1+(end-beg-1)/str;
		}
	}
};

template<typename T>
struct IndexOrSlice
{
	constexpr static bool is_slice = std::is_same<T, SliceDimBeg>::value ||
		                             std::is_same<T, SliceDimEnd>::value ||
							         std::is_same<T, SliceDimContig>::value;

	using type = typename std::conditional< is_slice, T, Index>::type;

};

} // end namespace internal

template<typename B = SliceDimBeg, typename E = SliceDimEnd, typename S = SliceDimContig>
auto Slice(B b = B{}, E e = E{}, S s = S{})
{
	using BT = typename internal::IndexOrSlice<B>::type;
	using ET = typename internal::IndexOrSlice<E>::type;
	using ST = typename internal::IndexOrSlice<S>::type;

	internal::Slice<BT, ET, ST> NewSlice;

	if constexpr ( !internal::IndexOrSlice<B>::is_slice ) {
		NewSlice._beg = static_cast<Index>(b);
	}
	if constexpr ( !internal::IndexOrSlice<E>::is_slice ) {
		NewSlice._end = static_cast<Index>(e);
	}
	if constexpr ( !internal::IndexOrSlice<S>::is_slice ) {
		NewSlice._str = static_cast<Index>(s);
	}

	return NewSlice;
};

}; // end namespace FortCpp






#endif
