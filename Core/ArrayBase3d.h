#ifndef FortCpp_ARRAYBASE3D_H
#define FortCpp_ARRAYBASE3D_H

namespace FortCpp
{
  
namespace internal
{

template<typename Derived>
struct traits<ArrayBase3d<Derived> >
{
  typedef typename traits<Derived>::Scalar Scalar;
  enum{
    Size   = traits<Derived>::Size,
    D1     = traits<Derived>::D1,
    D2     = traits<Derived>::D2,
    D3     = traits<Derived>::D3,
    StorageType = traits<Derived>::StorageType,
    Dim    = 3,
    Order  = traits<Derived>::Order,
    Align  = traits<Derived>::Align,
    Stride = traits<Derived>::Stride
    // Options = traits<Derived>::Options
  };

};

}; // end namespace internal

template <typename Derived>
class ArrayBase3d : public ArrayBase< Derived >
{

  typedef typename internal::traits<Derived>::Scalar T;
  typedef ArrayBase3d<Derived> BaseDerived;
  typedef ArrayBase<Derived> Base;

  private:

  protected:
  ArrayBase3d(const BaseDerived&)=default;
  ArrayBase3d(BaseDerived&&)=default;
  ArrayBase3d()=default;
  ~ArrayBase3d()=default;

  public:

   inline const T& operator = (const T &B) 
   {
     return Base::operator=(B);
   }

   template<typename OtherDerived>
   inline Derived& operator = (const ArrayBase<OtherDerived> &B) 
   {
     return Base::operator=(B);
  }
   inline Derived& operator = (const BaseDerived &B) 
   {
     return Base::operator=(B);
  }

  inline const int size() const { return derived().size(); }
   inline const int d1() const { return derived().d1(); }
   inline const int d2() const { return derived().d2(); }
   inline const int d3() const { return derived().d3(); }
   inline const int s1() const { return derived().s1(); }
   inline const int s2() const { return derived().s2(); }
   inline const int s3() const { return derived().s3(); }
   inline const bool allocated() const { return derived().allocated(); }
   inline const bool associated() const { return derived().associated(); }

  inline const T& operator () (const int i, const int j, const int k) const {
      return derived()(i,j,k);
  }

   inline T& operator () (const int i, const int j, const int k) {
      return derived()(i,j,k);
  }
  inline const T & operator [] (const int i) const{
    return derived()[i];
  }
  inline T & operator [] (const int i){
    return derived()[i];
  }

  inline const Derived& derived() const { return *static_cast<const Derived*>(this); }
  inline       Derived& derived() { return *static_cast<Derived*>(this); }

  // /**
  //  * output operator
  //  */
  // friend std::ostream& operator << (std::ostream& os, const ArrayBase3d<Derived> &A){
  //   for(int i=0;i<A.size();i++){
  //     os << A.derived()[i] << ' ';
  //   }
  //   return os;
  // }
};

/**********************************************************************/

}; // end namespace FortCpp



#endif
