#ifndef FortCpp_FORWARD_DECLARATIONS_H
#define FortCpp_FORWARD_DECLARATIONS_H

namespace FortCpp
{

/**
 * See the pairing with the enumeration down below
 */
enum {
	ColMajor  = 0,
	RowMajor  = 1,
	UnAligned = 0,
	Aligned   = 2,
	Contig    = 0,
	Strided   = 4,
	Static    = 0,
	Pointer   = 8
};

using Index = int64_t;

/**************************************************/

namespace internal
{

/**
 * This enum defines the option bits. These bits are:
 * MajorOrderBit : Defines which ordering to use, Column or Row Major
 *                 Defaults to 1 (Column) unless FortCpp_ROW_FortCppJOR is defined
 * AlignedBit    : Defines if this array is aligned on a 16 byte boundary
 *                 Defaults to 0 (not aligned)
 */
enum {
	MajorOrderBit = 1,
	AlignedBit    = 2,
	StrideBit     = 4,
	StorageBit    = 8
};

template<Index Size> class OpSize;
template<typename Derived> struct traits;
template<Index Rank,Index Order,Index Stride> class Dimensions;
template<typename T,Index StorageType,Index Align,Index... dims> class Storage;

}; // end namespace internal

template<typename Derived> class ArrayBase;
template<typename T,Index Rank, Index Options=0> class Alloc;
template<typename T,Index... dims> class Fixed;
template<typename Lhs,typename Rhs,typename Op> class BinaryOp;
template<Index Side,typename Lhs,typename Rhs,typename Op> class ConstantOp;
template<typename Rhs,typename Op> class UnaryOp;
template< typename Rhs, typename ReturnType, typename FuncType> class FuncTypeOp;

}; // end namespace FortCpp


#endif
