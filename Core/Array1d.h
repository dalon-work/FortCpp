#ifndef FortCpp_ARRAY1D_H
#define FortCpp_ARRAY1D_H

namespace FortCpp
{

namespace internal
{

template<typename _T, int _D1, int _Options>
struct traits<Array1d<_T,_D1,_Options> >
{
  typedef _T Scalar;
  enum{
    Size   = _D1,
    D1     = _D1,
    StorageType = (Size == Unknown) ? Pointer : Static,
    Dim    = 1,
    Order  = _Options & MajorOrderBit,
    Align  = _Options & AlignedBit,
    Stride = Contig,
    Options = _Options
  };

};

}; // end namespace internal

template <typename T,int D1,int Options>
class Array1d : public ArrayBase1d<Array1d<T,D1,Options> >
{
  typedef Array1d<T,D1,Options> Derived;
  typedef ArrayBase1d<Derived> Base;

  private:
  typedef internal::Accessor<Derived,
                             internal::traits<Derived>::Dim,
                             internal::traits<Derived>::Order,
                             internal::traits<Derived>::Stride> AccessorDerived;
  typedef internal::Storage<T,
                            internal::traits<Derived>::StorageType,
                            internal::traits<Derived>::Size,
                            internal::traits<Derived>::Align> StorageDerived;
  AccessorDerived _accessor;
  StorageDerived _storage;

  public:

  Array1d(){ }

  Array1d(const int d1){
    _storage.allocate(d1);
    _accessor.set_dim(d1);
  }
  Array1d(const Derived &B) = default;
  Array1d(Derived &&B) = default;

  ~Array1d(){
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

  /**
   * += Assignment Operator
   */
  inline const T& operator += (const T &B)
  {
   return Base::operator += (B);
  }

  template<typename OtherDerived>
  inline const Derived& operator += (const ArrayBase<OtherDerived> &B){
    return Base::operator += (B);
  }
  inline const Derived& operator += (const Derived &B){
    return Base::operator += (B);
  }

  /**
   * -= Assignment Operator
   */
  inline const T& operator -= (const T &B)
  {
   return Base::operator -= (B);
  }
  template<typename OtherDerived>
  inline const Derived& operator -= (const ArrayBase<OtherDerived> &B){
    return Base::operator -= (B);
  }
  inline const Derived& operator -= (const Derived &B){
    return Base::operator -= (B);
  }

  /*************************************************/

  void allocate(const int d1){
    _storage.allocate(d1);
    _accessor.set_dim(d1);
  }

  void map(T* a, const int d1){
    _storage.map(a,d1);
    _accessor.set_dim(d1);
  }

  template<typename OtherDerived>
  void mold(const ArrayBase1d<OtherDerived> &B){
    _storage.allocate(B.size());
    _accessor.set_dim(B.d1());
  }

  /**
   * Deallocates memory, or disassociates the array from the 
   * memory it points to if the array was not allocated
   */
  void deallocate(){
    _storage.deallocate();
  }
  /*************************************************/

  Derived reverse(){
    Derived A;
    A.mold(*this);
    int r=size()-1;
    for(int i=0;i<size();i++){
      A[i] = (*this)[r];
      r--;
    }
    return A;
  }

  static Derived constant(T c){
    FortCpp_IS_STATIC_FUNCTION_ASSERT
    Derived A;
    A = c;
    return A;
  }

  template<typename OtherDerived>
  static Derived constant(const ArrayBase1d<OtherDerived> &B,T c){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    Derived A;
    A.mold(B);
    A = c;
    return A;
  }

  static Derived constant(int i,T c){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    Derived A;
    A.allocate(i);
    A = c;
    return A;
  }

  static Derived zeros(){
    FortCpp_IS_STATIC_FUNCTION_ASSERT
    Derived A;
    A = 0;
    return A;
  }

  template<typename OtherDerived>
  static Derived zeros(const ArrayBase1d<OtherDerived> &B){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    Derived A;
    A.mold(B);
    A = 0;
    return A;
  }

  static Derived Zeros(int i){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    Derived A;
    A.allocate(i);
    A = 0;
    return A;
  }

  static Derived linspace(long double start, long double stop, int num,bool endpoint=1){
    FortCpp_NOT_STATIC_FUNCTION_ASSERT
    Derived A;
    A.allocate(num);
    int end = endpoint ? 1 : 0;
    long double dx = (stop-start)/static_cast<long double>(num-end);
    for(int i=0;i<num;i++){
      A[i] = static_cast<T>(start+static_cast<long double>(i)*dx);
    }
    return A;
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

/******************************************************/

  template<typename OtherClass,typename OtherScalar>
  inline Array1d<OtherScalar, D1, Options> build(OtherScalar OtherClass::*member)
  {
    Array1d<OtherScalar,D1,Options> a(this->size());
    for(int i=0;i<this->size();i++){
      a(i) = this->operator()(i).*member;
    }
    return a;
  }

/******************************************************/

  template<FortCpp_SLICE_TEMPLATE(1)>
  inline StridedArray1d<Derived,FortCpp_SLICE(1) > operator () (const FortCpp_SLICE_0(1) &S1){
    typedef StridedArray1d<Derived,FortCpp_SLICE(1)> ResultType;
    enum{
      Stride1 = internal::traits<ResultType>::S1,
    };
    const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
    const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
      FortCpp_SLICE_CHECK(1)
    StridedArray1d<Derived,FortCpp_SLICE(1)> S(&_storage[_accessor(S1.b())],l1,s1);
    return S;
  }

  /*********************************************************/

   inline const int d1() const { return _accessor.d1(); }
   inline const int size() const { return _storage.size(); }
   inline const bool allocated() const { return _storage.allocated(); }
   inline const bool associated() const { return _storage.associated(); }
   inline T* data() { return _storage.data(); }

};

}; // end namespace FortCpp

#endif

