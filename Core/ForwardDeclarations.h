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
    Contig = 0,
    Strided = 4
};



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
    StrideBit     = 4
};

/**
 * Various other definitions
 */
enum {
    Full = -1,
    Unknown = -2,
    Pointer = 0,
    Static = 1,
};

template<int Size> class OpSize;
template<typename Derived> struct traits;
template<unsigned Rank ,unsigned Order,unsigned Stride> class Dimensions;
template<typename T,unsigned StorageType, unsigned Align> class Storage;

// template<int _s> class StrideValue;
// template<int... StridePack> class Strides;
// template<typename Derived,int Dim, int Order, int Stride,int... StridePack> class Accessor;
// template<int Type,int Dim=0,int L=Unknown,int S=1> class Slice;
template<typename Derived> class SliceBase;
// template<int Value> class SliceValue;

}; // end namespace internal


template<typename Derived> class ArrayBase;
template<typename T,unsigned Rank, unsigned Options=0> class Alloc;
template<int Side, typename Derived,typename Op> class ConstantOp;
template<typename Lhs,typename Rhs,typename Op> class BinaryOp;
template<typename Rhs,typename Op> class UnaryOp;

}; // end namespace FortCpp


#endif
