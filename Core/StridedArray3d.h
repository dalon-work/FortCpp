#ifndef FortCpp_STRIDEDARRAY3D_H
#define FortCpp_STRIDEDARRAY3D_H

namespace FortCpp
{

namespace internal
{

/** traits array for StridedArray3d **/
template<typename ArrayDerived,typename SliceDerived1,typename SliceDerived2,typename SliceDerived3>
struct traits< StridedArray3d<ArrayDerived,SliceDerived1,SliceDerived2,SliceDerived3> >
{
  typedef typename traits<ArrayDerived>::Scalar Scalar;
  enum{
    L1 = compute_slice_length<ArrayDerived,SliceDerived1>::L,
    S1 = compute_slice_stride<ArrayDerived,SliceDerived1>::S,
    L2 = compute_slice_length<ArrayDerived,SliceDerived2>::L,
    S2 = compute_slice_stride<ArrayDerived,SliceDerived2>::S,
    L3 = compute_slice_length<ArrayDerived,SliceDerived3>::L,
    S3 = compute_slice_stride<ArrayDerived,SliceDerived3>::S,
    sS1 = traits<SliceDerived1>::S,
    sS2 = traits<SliceDerived2>::S,
    sS3 = traits<SliceDerived3>::S,
    T1 = traits<SliceDerived1>::Type,
    T2 = traits<SliceDerived2>::Type,
    T3 = traits<SliceDerived3>::Type,
    D1 = ((int(L1) != int(Unknown)) && (int(sS1) != int(Unknown))) ? int(L1/sS1+L1%sS1) : int(Unknown),
    D2 = ((int(L2) != int(Unknown)) && (int(sS2) != int(Unknown))) ? int(L2/sS2+L2%sS2) : int(Unknown),
    D3 = ((int(L3) != int(Unknown)) && (int(sS3) != int(Unknown))) ? int(L3/sS3+L3%sS3) : int(Unknown),
    Size = (int(D1) != int(Unknown)) && 
           (int(D2) != int(Unknown)) && 
           (int(D3) != int(Unknown)) ? int(D1*D2*D3) : int(Unknown),
    Dim = 3,
    Order = traits<ArrayDerived>::Order,
    Align = UnAligned,
    Stride = (int(T1) == int(Full)) && 
             (int(T2) == int(Full)) && 
             (S3 == 1) ? int(Contig) : int(Strided),
    StorageType = Pointer
  };

}; // end traits

}; // end internal

template<typename ArrayDerived,typename SliceDerived1,typename SliceDerived2,typename SliceDerived3>
class StridedArray3d : public ArrayBase3d< StridedArray3d<ArrayDerived,SliceDerived1,SliceDerived2,SliceDerived3> >
{
  typedef class StridedArray3d<ArrayDerived,SliceDerived1,SliceDerived2,SliceDerived3> Derived;
  typedef ArrayBase3d<Derived> Base;
  typedef typename internal::traits<Derived>::Scalar T;

  private:
  internal::Storage<T,
                    internal::traits<Derived>::StorageType,
                    internal::traits<Derived>::Size,
                    internal::traits<Derived>::Align> _storage;
  internal::Accessor<Derived,
                     internal::traits<Derived>::Dim,
                     internal::traits<Derived>::Order,
                     internal::traits<Derived>::Stride,
                     internal::traits<Derived>::S1,
                     internal::traits<Derived>::S2,
                     internal::traits<Derived>::S3> _accessor;

  public:
  StridedArray3d(const Derived &B) = default;
  StridedArray3d(Derived &&B)=default;
  StridedArray3d()=default;

  StridedArray3d(T* a,const int d1,const int d2,const int d3,const int s1,const int s2,const int s3){
    _storage.map(a,d1*d2*d3);
    _accessor.set_dim(d1,d2,d3);
    _accessor.set_str(s1,s2,s3);
  }

  ~StridedArray3d(){
    deallocate();
  }

  /**
   * Assignment operator for a single value, delegates to base
   */
  inline const T& operator = (const T &B)
  {
   return Base::operator = (B);
  }

 /** 
  * Assignment operator for an entire array, delegates to base
  */
  template<typename OtherDerived>
  inline const Derived& operator = (const ArrayBase<OtherDerived> &B){
    return Base::operator = (B);
  }
  inline const Derived& operator = (const Derived &B){
    return Base::operator = (B);
  }

  /*************************************************/

  /**
   * Deallocates memory, or disassociates the array from the 
   * memory it points to if the array was not allocated
   */
  void deallocate(){
    _storage.deallocate();
  }
  /*************************************************/

  /** 
   * indexing (parenthesis operator), two of them (for const correctness)
   * In debug mode, if a dimension is out-of-bounds, a fatal
   * exception is thrown.
   */
  inline const T& operator () (const int i, const int j, const int k) const {
      return _storage[_accessor(i,j,k)];
  }

   inline T& operator () (const int i,const int j, const int k) {
      return _storage[_accessor(i,j,k)];
  }

  /**
   * Linear indexer of the result. This varies from 0 to this->_size-1.
   * Throws a fatal exception in debug mode if i is outside these limits.
   */
  inline const T & operator [] (const int i) const{
    return _storage[_accessor[i]];
  }
  inline T & operator [] (const int i){
    return _storage[_accessor[i]];
  }

   inline const int d1() const { return _accessor.d1(); }
   inline const int d2() const { return _accessor.d2(); }
   inline const int d3() const { return _accessor.d3(); }
   inline const int s1() const { return _accessor.s1(); }
   inline const int s2() const { return _accessor.s2(); }
   inline const int s3() const { return _accessor.s3(); }
   inline const int size() const { return _storage.size(); }
   inline const bool allocated() const { return _storage.allocated(); }
   inline const bool associated() const { return _storage.associated(); }


}; // end class

}; // end namespace FortCpp

#endif
