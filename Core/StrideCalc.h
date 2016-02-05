#ifndef FortCpp_STRIDECALC_H
#define FortCpp_STRIDECALC_H

namespace FortCpp
{

namespace internal
{

   /*** Counts the number of slices in a view ***/
template<typename front,typename... indices> struct count_slice;

template<typename last>
struct count_slice<last>
{
   enum
   {
      count = std::is_same<last,class Slice>::value
   };
};

template<typename front,typename... indices>
struct count_slice
{
   enum
   {
      count = std::is_same<front,class Slice>::value + count_slice<indices...>::count
   };
};





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

