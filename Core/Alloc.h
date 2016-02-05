#ifndef FortCpp_ARRAY2D_H
#define FortCpp_ARRAY2D_H

#include "ArrayBase.h"
#include <assert.h>
#include "Slice.h"
#include "StrideCalc.h"

namespace FortCpp
{

namespace internal
{

template<typename _T, unsigned _Rank, unsigned _Options>
struct traits<Alloc<_T,_Rank,_Options> >
{
  typedef _T Scalar;
  enum{
    Size = Unknown,
    Rank = _Rank,
    StorageType = (Size == Unknown) ? Pointer : Static,
    Order = _Options & MajorOrderBit,
    Align  = _Options & AlignedBit,
    Stride = _Options & StrideBit,
    Options = _Options
};

}; // end struct traits


}; // end namespace internal

template<typename T,unsigned Rank, unsigned Options>
class Alloc : public ArrayBase<Alloc<T,Rank,Options> >
{

  typedef class Alloc<T,Rank,Options> Derived;
  typedef ArrayBase<Derived> Base;
  typedef internal::Storage<T,
          internal::traits<Derived>::StorageType,
          internal::traits<Derived>::Align> StorageDerived;
  typedef internal::Dimensions<Rank,
          internal::traits<Derived>::Order,
          internal::traits<Derived>::Stride> DimensionsDerived;
  // static const unsigned Order = internal::traits<Derived>::Order;

  private:
  StorageDerived storage;
  DimensionsDerived dim;

  public:
  Alloc()=default;
  Alloc(const Derived &B)=delete;
  Alloc(Derived &&B)=delete;
  ~Alloc(){
     deallocate();
  }

  void deallocate(){
     dim.unset();
    storage.deallocate();
  }

  template<typename... indices>
  Alloc(indices... idx){
    static_assert(sizeof...(idx) == Rank,"NUMBER OF INDICES DOES NOT MATCH RANK OF ARRAY");
    dim.set_dim(idx...);
    storage.allocate(dim.size());
  }

  /***********************************************/

   const T& operator = (const T& B){
    return Base::operator = (B);
  }

  template<typename OtherDerived>
   const Derived& operator = (const ArrayBase<OtherDerived> &B){
     static_assert(Rank == internal::traits<OtherDerived>::Rank,"RANKS DO NOT MATCH IN ASSIGNMENT OPERATION");
    return Base::operator = (B);
  }
   const Derived& operator = (const Derived &B){
    return Base::operator = (B);
  }

  /************************************************/

  template<typename... indices>
  void allocate(indices... idx){
     if( internal::is_negative(idx...) ){ assert(0); }
    dim.set_dim(static_cast<unsigned>(idx)...);
    storage.allocate(dim.size());
  }

  template<typename... indices>
  void map(T* a, indices... idx){
    static_assert(internal::traits<Derived>::Align == UnAligned,"MAPPING ONTO AN ALIGNED ARRAY IS NOT ALLOWED");
    static_assert(sizeof...(idx) == Rank, "NUMBER OF INDICES DOES NOT MATCH RANK OF ARRAY");
     if( internal::is_negative(idx...) ){ assert(0); }
    dim.set_dim(static_cast<unsigned>(idx)...);
    storage.map(a,dim.size());
  }

  template<typename OtherDerived>
     void mold(const ArrayBase<OtherDerived> &B){
     static_assert(Rank == internal::traits<OtherDerived>::Rank,"RANKS DO NOT MATCH IN MOLD OPERATION");
     deallocate();
     dim.copy_dim(B.derived().get_dim(),B.derived().get_str());
     storage.allocate(dim.size());
     }

  /***************************************/

  template<typename... indices>
  const T& operator () (indices... idx) const{
     if( internal::is_negative(idx...) ){ assert(0); }
    // FortCpp_BOUNDS_CHECK(i,1)
    // FortCpp_BOUNDS_CHECK(j,2)
    return storage[dim(static_cast<unsigned>(idx)...)];
  }

  template<typename... indices>
  T& operator () (indices... idx){
     if( internal::is_negative(idx...) ){ assert(0); }
    // FortCpp_BOUNDS_CHECK(i,1)
    // FortCpp_BOUNDS_CHECK(j,2)
    return storage[dim(static_cast<unsigned>(idx)...)];
  }

  /*************************************************/

  const T & operator [] (int i) const{
     if( internal::is_negative(i) ){ assert(0); }
    return storage[dim[i]];
  }
  T & operator [] (int i){
     if( internal::is_negative(i) ){ assert(0); }
    return storage[dim[i]];
  }

  /***********************************************/

  unsigned size() const { return dim.size(); }
   unsigned size(unsigned r) const { return dim.size(r); }
   const std::array<unsigned,Rank>& get_dim() const { return dim.get_dim(); }
   const std::array<unsigned,Rank>& get_str() const { return dim.get_str(); }
   const DimensionsDerived & check_dim() const { return dim; }
    bool allocated() const { return storage.allocated(); }
    bool associated() const { return storage.associated(); }
    T*  data() { return storage.data(); }

  /***********************************************/



    // Alloc<T,internal::count_slice<indices...>::count,Order|UnAligned|Strided>
 template<typename... indices>
    void view(indices... idx){
       Alloc<T,internal::count_slice<indices...>::count,Order|UnAligned|Strided> S;
       // cout << internal::count_slice<indices...>::count << endl;

    }

 template<typename OtherDerived,typename... indices>
    void map_view(ArrayBase<OtherDerived>& A,indices... idx){

       std::array<unsigned,internal::traits<OtherDerived>::Rank> beg;
    }








  // template< FortCpp_SLICE_TEMPLATE(1) >
  //  StridedArray1d<Derived,FortCpp_SLICE(1) > operator () (const FortCpp_SLICE_0(1) &S1,
  //                                                            const int j){
  //   typedef StridedArray1d<Derived,FortCpp_SLICE(1)> ResultType;
  //   enum{
  //     Stride1 = internal::traits<ResultType>::S1
  //   };
  //   const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
  //   const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
  //   FortCpp_SLICE_CHECK(1)
  //   FortCpp_BOUNDS_CHECK(j,2)
  //   ResultType S(&storage[dim(S1.b(),j)],l1,s1);
  //   return S;
  // }

  // template<FortCpp_SLICE_TEMPLATE(2)>
  //  StridedArray1d<Derived,FortCpp_SLICE(2)> operator () (const int i,
  //                                                          const FortCpp_SLICE_0(2) &S2){
  //   typedef StridedArray1d<Derived,FortCpp_SLICE(2)> ResultType;
  //   enum{
  //     Stride1 = internal::traits<ResultType>::S1
  //   };
  //   const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
  //   const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
  //   FortCpp_BOUNDS_CHECK(i,1)
  //   FortCpp_SLICE_CHECK(2)
  //   ResultType S(&storage[dim(i,S2.b())],l2,s2);
  //   return S;
  // }
  //
  // template<FortCpp_SLICE_TEMPLATE(1), FortCpp_SLICE_TEMPLATE(2)>
  //  StridedAlloc<Derived,FortCpp_SLICE(1),
  //                               FortCpp_SLICE(2) >operator () (const FortCpp_SLICE_0(1) &S1,
  //                                                          const FortCpp_SLICE_0(2) &S2){
  //   typedef StridedAlloc<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2)> ResultType;
  //   enum{
  //     Stride1 = internal::traits<ResultType>::S1,
  //     Stride2 = internal::traits<ResultType>::S2
  //   };
  //   const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
  //   const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
  //   const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
  //   const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
  //   FortCpp_SLICE_CHECK(1)
  //   FortCpp_SLICE_CHECK(2)
  //   ResultType S(&storage[dim(S1.b(),S2.b())],l1,l2,s1,s2);
  //   return S;
  // }

  /*******************************************************/


};

}; // end namespace FortCpp

#endif
