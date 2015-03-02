#ifndef FortCpp_STRIDEDARRAY1D_H
#define FortCpp_STRIDEDARRAY1D_H

namespace FortCpp
{

namespace internal
{

/** traits array for StridedArray1d **/
template<typename ArrayDerived,typename SliceDerived>
struct traits< StridedArray1d<ArrayDerived,SliceDerived> >
{
  typedef typename traits<ArrayDerived>::Scalar Scalar;
  enum{
    L1 = compute_slice_length<ArrayDerived,SliceDerived>::L,
    S1 = compute_slice_stride<ArrayDerived,SliceDerived>::S,
    sS1 = traits<SliceDerived>::S,
    D1 = ((int(L1) != int(Unknown)) && (int(sS1) != int(Unknown))) ? int(L1/sS1+L1%sS1) : int(Unknown),
    Size = D1,
    Dim = 1,
    Order = traits<ArrayDerived>::Order,
    Align = UnAligned,
    Stride = (S1 == 1) ? int(Contig) : int(Strided),
    StorageType = Pointer,
    Options = traits<ArrayDerived>::Options
  };

}; // end traits

}; // end internal

template<typename ArrayDerived,typename SliceDerived>
class StridedArray1d : public ArrayBase1d< StridedArray1d<ArrayDerived,SliceDerived> >
{
  typedef StridedArray1d<ArrayDerived,SliceDerived> Derived;
  typedef ArrayBase1d<Derived> Base;
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
                     internal::traits<Derived>::S1> _accessor;

  public:
  StridedArray1d(const Derived &B) = default;
  StridedArray1d(Derived &&B)=default;

  StridedArray1d(T* a,const int d1,const int s1){
    _storage.map(a,d1);
    _accessor.set_dim(d1);
    _accessor.set_str(s1);
  }

  ~StridedArray1d(){
    _storage.deallocate();
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
  inline const T& operator () (const int &i) const {
    FortCpp_BOUNDS_CHECK(i,1)
      return _storage[_accessor(i)];
  }

   inline T& operator () (const int &i) {
    FortCpp_BOUNDS_CHECK(i,1)
      return _storage[_accessor(i)];
  }

  /**
   * Linear indexer of the result. This varies from 0 to this->_size-1.
   * Throws a fatal exception in debug mode if i is outside these limits.
   */
  inline const T & operator [] (const int i) const{
    FortCpp_LINEAR_SIZE_CHECK(i)
    return _storage[_accessor[i]];
  }
  inline T & operator [] (const int i){
    FortCpp_LINEAR_SIZE_CHECK(i)
    return _storage[_accessor[i]];
  }

   inline const int d1() const { return _accessor.d1(); }
   inline const int s1() const { return _accessor.s1(); }
   inline const int size() const { return _storage.size(); }
   inline const bool allocated() const { return _storage.allocated(); }
   inline const bool associated() const { return _storage.associated(); }


}; // end class

}; // end namespace FortCpp

#endif
