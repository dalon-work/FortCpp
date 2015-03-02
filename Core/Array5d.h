#ifndef FortCpp_ARRAY5D_H
#define FortCpp_ARRAY5D_H

namespace FortCpp
{

namespace internal
{

template<typename _T, int _Options>
struct traits<Array5d<_T, _Options> >
{
  typedef _T Scalar;
  enum{
    // Size = (_D1 == Unknown) || (_D2 == Unknown) ? Unknown : _D1*_D2,
    Size = Unknown,
    D1   = Unknown,
    D2   = Unknown,
    D3   = Unknown,
    D4   = Unknown,
    D5   = Unknown,
    StorageType = (Size == Unknown) ? Pointer : Static,
    Stride = Contig,
    Dim  = 5,
    Order = _Options & MajorOrderBit,
    Align  = _Options & AlignedBit,
    Options = _Options
};

}; // end struct traits

}; // end namespace internal

template<typename T,int Options>
class Array5d : public ArrayBase5d<Array5d<T,Options> >
{

  typedef class Array5d<T,Options> Derived;
  typedef ArrayBase5d<Derived> Base;
  typedef internal::Storage<T,
          internal::traits<Derived>::StorageType,
          internal::traits<Derived>::Size,
          internal::traits<Derived>::Align> StorageDerived;
  typedef internal::Accessor<Derived,
          internal::traits<Derived>::Dim,
          internal::traits<Derived>::Order,
          internal::traits<Derived>::Stride> AccessorDerived;

  private:
  StorageDerived _storage;
  AccessorDerived _accessor;

  public:
  Array5d(const Derived &B)=default;
  Array5d(Derived &&B)=default;
  Array5d()=default;

  Array5d(const int d1, 
          const int d2,
          const int d3,
          const int d4,
          const int d5){
    _storage.allocate(d1*d2*d3*d4*d5);
    _accessor.set_dim(d1,d2,d3,d4,d5);
  }

  ~Array5d(){ 
    _storage.deallocate();
  }

  /***********************************************/

  /**
   * Assignment operator for a single value, delegates to base
   */
  inline const T& operator = (const T& B){
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

  /************************************************/

  void allocate(const int d1, 
                const int d2,
                const int d3,
                const int d4,
                const int d5){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    _storage.allocate(d1*d2*d3*d4*d5);
    _accessor.set_dim(d1,d2,d3,d4,d5);
  }

  /** 
   * Maps a pointer into the array. The size of the 
   * pointer array is given by i
   */
  void map(T* a, const int d1, 
                 const int d2,
                 const int d3,
                 const int d4,
                 const int d5){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    _storage.map(a,d1*d2*d3*d4*d5);
    _accessor.set_dim(d1,d2,d3,d4,d5);
  }

  void mold(const Derived &B){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    _storage.allocate(B.size());
    _accessor.set_dim(B.d1(),B.d2(),B.d3(),B.d4(),B.d5());
  }

  /**
   * Deallocates memory, or disassociates the array from the 
   * memory it points to if the array was not allocated
   */
  void deallocate(){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    _storage.deallocate();
  }

  /***************************************/

  /** 
   * indexing (parenthesis operator), two of them (for const correctness)
   * In debug mode, if a dimension is out-of-bounds, a fatal
   * exception is thrown.
   */
  const T& operator () (const int i, 
                        const int j,
                        const int k,
                        const int l,
                        const int m) const{
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    return _storage[_accessor(i,j,k,l,m)];
  }
  T& operator () (const int i, 
                  const int j,
                  const int k,
                  const int l,
                  const int m){
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    return _storage[_accessor(i,j,k,l,m)];
  }

  /*************************************************/

  /**
   * Linear indexer of the result. This varies from 0 to this->_size-1.
   * Throws a fatal exception in debug mode if i is outside these limits.
   */
  const T & operator [] (const int i) const{
    FortCpp_LINEAR_SIZE_CHECK(i)
    return _storage[_accessor[i]];
  }
  T & operator [] (const int &i){
    FortCpp_LINEAR_SIZE_CHECK(i)
    return _storage[_accessor[i]];
  }

  /*******************************************************/

  // 1d Slices
  template<FortCpp_SLICE_TEMPLATE(1)>
  inline StridedArray1d<Derived,FortCpp_SLICE(1) > operator () (const FortCpp_SLICE_0(1) &S1,
                                                                              const int j,
                                                                              const int k,
                                                                              const int l,
                                                                              const int m){
    typedef StridedArray1d<Derived,FortCpp_SLICE(1)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),j,k,l,m)],l1,s1);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2)>
  inline StridedArray1d<Derived,FortCpp_SLICE(2) > operator () (const int i,
                                                            const FortCpp_SLICE_0(2) &S2,
                                                            const int k,
                                                            const int l,
                                                            const int m){
    typedef StridedArray1d<Derived,FortCpp_SLICE(2)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(i,S2.b(),k,l,m)],l2,s2);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(3)>
  inline StridedArray1d<Derived,FortCpp_SLICE(3) > operator () (const int i,
                                                            const int j,
                                                            const FortCpp_SLICE_0(3) &S3,
                                                            const int l,
                                                            const int m){
    typedef StridedArray1d<Derived,FortCpp_SLICE(3)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
    };
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(1,3)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(i,j,S3.b(),l,m)],l3,s3);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray1d<Derived,FortCpp_SLICE(4) > operator () (const int i,
                                                            const int j,
                                                            const int k,
                                                            const FortCpp_SLICE_0(4) &S4,
                                                            const int m){
    typedef StridedArray1d<Derived,FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
    };
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(1,4)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(i,j,k,S4.b(),m)],l4,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray1d<Derived,FortCpp_SLICE(5) > operator () (const int i,
                                                            const int j,
                                                            const int k,
                                                            const int l,
                                                            const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray1d<Derived,FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
    };
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(1,5)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,j,k,l,S5.b())],l5,s5);
    return S;
  }

  // 2d Slices
  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2)>
  inline StridedArray2d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2)> operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const int l,
                                                           const int m){
    typedef StridedArray2d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1) 
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2) 
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1) 
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2) 
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),k,l,m)],l1,l2,s1,s2);
    return S;
  }

  template< FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(3)>
  inline StridedArray2d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(3)> operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const int m){
    typedef StridedArray2d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(3)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),j,S3.b(),l,m)],l1,l3,s1,s3);
    return S;
  }

  template< FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray2d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(4)> operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray2d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(2,4)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),j,k,S4.b(),m)],l1,l4,s1,s4);
    return S;
  }

  template< FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray2d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(5)> operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const int k,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray2d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(2,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),j,k,l,S5.b())],l1,l5,s1,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3)>
  inline StridedArray2d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(3) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const int m){
    typedef StridedArray2d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(3)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2) 
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3) 
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2) 
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3) 
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(i,S2.b(),S3.b(),l,m)],l2,l3,s2,s3);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray2d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(4) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray2d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2) 
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4) 
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2) 
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(2,4) 
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,4)
    ResultType S(&_storage[_accessor(i,S2.b(),k,S4.b(),m)],l2,l4,s2,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray2d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray2d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2) 
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5) 
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2) 
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(2,5) 
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,S2.b(),k,l,S5.b())],l2,l5,s2,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray2d<Derived,FortCpp_SLICE(3),
                                FortCpp_SLICE(4) >operator () (const int i,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray2d<Derived,FortCpp_SLICE(3),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3) 
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4) 
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(1,3) 
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(2,4) 
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(i,j,S3.b(),S4.b(),m)],l3,l4,s3,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray2d<Derived,FortCpp_SLICE(3),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray2d<Derived,FortCpp_SLICE(3),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3) 
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5) 
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(1,3) 
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(2,5) 
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,j,S3.b(),l,S5.b())],l3,l5,s3,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray2d<Derived,FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const int j,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray2d<Derived,FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
    };
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4) 
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5) 
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(1,4) 
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(2,5) 
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,j,k,S4.b(),S5.b())],l4,l5,s4,s5);
    return S;
  }

  // // 3d Slices
  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3)>
  inline StridedArray3d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2),
                                FortCpp_SLICE(3) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const int m){
    typedef StridedArray3d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2),FortCpp_SLICE(3)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(3,3)
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),S3.b(),l,m)],l1,l2,l3,s1,s2,s3);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray3d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2),
                                FortCpp_SLICE(4) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray3d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(3,4)
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),k,S4.b(),m)],l1,l2,l4,s1,s2,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray3d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(4) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray3d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(3),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(3,4)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),j,S3.b(),S4.b(),m)],l1,l3,l4,s1,s3,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray3d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(4) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray3d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(3),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(3,4)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(i,S2.b(),S3.b(),S4.b(),m)],l2,l3,l4,s2,s3,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray3d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2),
                                FortCpp_SLICE(5) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray3d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(3,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),k,l,S5.b())],l1,l2,l5,s1,s2,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray3d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(5) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray3d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(3),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(3,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),j,S3.b(),l,S5.b())],l1,l3,l5,s1,s3,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray3d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray3d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(3),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(3,5)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,S2.b(),S3.b(),l,S5.b())],l2,l3,l5,s2,s3,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray3d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray3d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(2,4)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(3,5)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,S2.b(),k,S4.b(),S5.b())],l2,l4,l5,s2,s4,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray3d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray3d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(2,4)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(3,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),j,k,S4.b(),S5.b())],l1,l4,l5,s1,s4,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray3d<Derived,FortCpp_SLICE(3),
                                FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray3d<Derived,FortCpp_SLICE(3),FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
    };
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(1,3)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(2,4)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(3,5)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,j,S3.b(),S4.b(),S5.b())],l3,l4,l5,s3,s4,s5);
    return S;
  }

  // // 4d Slices
  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4)>
  inline StridedArray4d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(4) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const int m){
    typedef StridedArray4d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2),FortCpp_SLICE(3),FortCpp_SLICE(4)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
      Stride4 = internal::traits<ResultType>::S4,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(3,3)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(4,4)
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_BOUNDS_CHECK(m,5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),S3.b(),S4.b(),m)],l1,l2,l3,l4,s1,s2,s3,s4);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray4d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(5) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const int l,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray4d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2),FortCpp_SLICE(3),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
      Stride4 = internal::traits<ResultType>::S5,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(3,3)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(4,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_BOUNDS_CHECK(l,4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),S3.b(),l,S5.b())],l1,l2,l3,l5,s1,s2,s3,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray4d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(2),
                                FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const int k,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray4d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2),FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
      Stride4 = internal::traits<ResultType>::S4,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(3,4)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(4,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_BOUNDS_CHECK(k,3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),S2.b(),k,S4.b(),S5.b())],l1,l2,l4,l5,s1,s2,s4,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(1),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray4d<Derived,FortCpp_SLICE(1),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const FortCpp_SLICE_0(1) &S1,
                                                           const int j,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray4d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(3),FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
      Stride4 = internal::traits<ResultType>::S4,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(3,4)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(4,5)
    FortCpp_SLICE_CHECK(1)
    FortCpp_BOUNDS_CHECK(j,2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(S1.b(),j,S3.b(),S4.b(),S5.b())],l1,l3,l4,l5,s1,s3,s4,s5);
    return S;
  }

  template<FortCpp_SLICE_TEMPLATE(2),FortCpp_SLICE_TEMPLATE(3),FortCpp_SLICE_TEMPLATE(4),FortCpp_SLICE_TEMPLATE(5)>
  inline StridedArray4d<Derived,FortCpp_SLICE(2),
                                FortCpp_SLICE(3),
                                FortCpp_SLICE(4),
                                FortCpp_SLICE(5) >operator () (const int i,
                                                           const FortCpp_SLICE_0(2) &S2,
                                                           const FortCpp_SLICE_0(3) &S3,
                                                           const FortCpp_SLICE_0(4) &S4,
                                                           const FortCpp_SLICE_0(5) &S5){
    typedef StridedArray4d<Derived,FortCpp_SLICE(2),FortCpp_SLICE(3),FortCpp_SLICE(4),FortCpp_SLICE(5)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
      Stride2 = internal::traits<ResultType>::S2,
      Stride3 = internal::traits<ResultType>::S3,
      Stride4 = internal::traits<ResultType>::S4,
    };
    const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
    const int l3 = FortCpp_SLICE_RUNTIME_LENGTH(3)
    const int l4 = FortCpp_SLICE_RUNTIME_LENGTH(4)
    const int l5 = FortCpp_SLICE_RUNTIME_LENGTH(5)
    const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
    const int s3 = FortCpp_SLICE_RUNTIME_STRIDE(2,3)
    const int s4 = FortCpp_SLICE_RUNTIME_STRIDE(3,4)
    const int s5 = FortCpp_SLICE_RUNTIME_STRIDE(4,5)
    FortCpp_BOUNDS_CHECK(i,1)
    FortCpp_SLICE_CHECK(2)
    FortCpp_SLICE_CHECK(3)
    FortCpp_SLICE_CHECK(4)
    FortCpp_SLICE_CHECK(5)
    ResultType S(&_storage[_accessor(i,S2.b(),S3.b(),S4.b(),S5.b())],l2,l3,l4,l5,s2,s3,s4,s5);
    return S;
  }

  /*******************************************************/

   const int d1() const { return _accessor.d1(); }
   const int d2() const { return _accessor.d2(); }
   const int d3() const { return _accessor.d3(); }
   const int d4() const { return _accessor.d4(); }
   const int d5() const { return _accessor.d5(); }
   inline const int size() const { return _storage.size(); }
   inline const bool allocated() const { return _storage.allocated(); }
   inline const bool associated() const { return _storage.associated(); }
   inline T*  data() { return _storage.data(); }

};

}; // end namespace FortCpp

#endif
