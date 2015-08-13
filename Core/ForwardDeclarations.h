#ifndef FortCpp_FORWARD_DECLARATIONS_H
#define FortCpp_FORWARD_DECLARATIONS_H

namespace FortCpp
{

/**
 * See the pairing with the enumeration down below
 */
enum
{
  ColMajor  = 0,
  RowMajor  = 1,
  UnAligned = 0,
  Aligned   = 2
};



/**************************************************/

namespace internal
{

/**
 * This enum defines the option bits. These bits are:
 * MajorOrderBit : Defines which ordering to use, Column or Row Major
 *                 Defaults to 1 (Column) unless FortCpp_ROW_MAJOR is defined
 * AlignedBit    : Defines if this array is aligned on a 16 byte boundary
 *                 Defaults to 0 (not aligned)
 */
enum
{
  MajorOrderBit = 1,
  AlignedBit    = 2,
};

/** 
 * Various other definitions
 */
enum
{
  Full = -1,
  Unknown = -2,
  Pointer = 0,
  Static = 1,
  Contig = 0,
  Strided = 1
};

template<int Size> class OpSize;
template<typename Derived> struct traits;
template<typename Derived,int Size,int Dim> class Dimensions;
template<int _s> class StrideValue;
template<int... StridePack> class Strides;
template<typename Derived,int Dim, int Order, int Stride,int... StridePack> class Accessor;
template<int Type,int Dim=0,int L=Unknown,int S=1> class Slice;
template<typename Derived> class SliceBase;
template<int Value> class SliceValue;

}; // end namespace internal


template<typename Derived> class ArrayBase;
template<typename Derived> class ArrayBase1d;
template<typename Derived> class ArrayBase2d;
template<typename Derived> class ArrayBase3d;
template<typename Derived> class ArrayBase4d;
template<typename Derived> class ArrayBase5d;
template<typename T, 
         int D1=internal::Unknown, int Options=0> class Array1d;
template<typename T,
         int D1=internal::Unknown, 
         int D2=internal::Unknown, int Options=0> class Array2d;
template<typename T,int Options=0> class Array3d;
template<typename T,int Options=0> class Array4d;
template<typename T,int Options=0> class Array5d;

template<typename ArrayDerived,
         typename SliceDerived1> class StridedArray1d;
template<typename ArrayDerived,
         typename SliceDerived1,
         typename SliceDerived2> class StridedArray2d;
template<typename ArrayDerived,
         typename SliceDerived1,
         typename SliceDerived2,
         typename SliceDerived3> class StridedArray3d;
template<typename ArrayDerived,
         typename SliceDerived1,
         typename SliceDerived2,
         typename SliceDerived3,
         typename SliceDerived4> class StridedArray4d;
template<typename ArrayDerived,
         typename SliceDerived1,
         typename SliceDerived2,
         typename SliceDerived3,
         typename SliceDerived4,
         typename SliceDerived5> class StridedArray5d;

template<int Side, typename Derived,typename Op> class ConstantOp;
template<typename Lhs,typename Rhs,typename Op> class BinaryOp;
template<typename Rhs,typename Op> class UnaryOp;

}; // end namespace FortCpp


#endif
