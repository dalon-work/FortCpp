#ifndef FortCpp_ARRAYBASE_H
#define FortCpp_ARRAYBASE_H

namespace FortCpp
{

template <typename Derived>
class ArrayBase
{

  typedef typename internal::traits<Derived>::Scalar T;

  private:

  protected:
  ArrayBase(const ArrayBase<Derived>&)=default;
  ArrayBase(ArrayBase<Derived>&&)=default;
  ArrayBase()=default;
  ~ArrayBase()=default;

  public:

  /** 
   * Sets the entire array to a given value
   */
   inline const T& operator = (const T &B) 
   {
#pragma GCC ivdep
     for(int i=0;i< this->size();i++){ 
       derived()[i] = B;
     }
         return B;
   }

  /**
   * This copies an array using the equals sign. The
   * sizes of the two arrays must match, or else a fatal error
   * is thrown in debug mode. This allows a MD ArrayBase to be
   * assigned to a 1D ArrayBase since and vice versa, since it
   * only takes size into account.
   */
   template<typename OtherDerived>
   inline Derived& operator = (const ArrayBase<OtherDerived> &B) 
   {
    FortCpp_SIZE_MATCH(this->size(),B.size())
#pragma GCC ivdep
    for(int i=0;i<this->size();i++){ 
      derived()[i] = B.derived()[i];
    }
    return derived();
   }

   inline Derived& operator = (const ArrayBase<Derived> &B) 
   {
    FortCpp_SIZE_MATCH(this->size(),B.size())
#pragma GCC ivdep
    for(int i=0;i<this->size();i++){ 
      derived()[i] = B.derived()[i];
    }
    return derived();
  }

   /**
    * += and -= operators
    */
   inline Derived& operator += (const T &B) 
   {
     derived() = derived() + B;
     return derived();
   }

   template<typename OtherDerived>
   inline Derived& operator += (const ArrayBase<OtherDerived> &B) 
   {
     derived() = derived() + B.derived();
     return derived();
   }

   inline Derived& operator -= (const T &B) 
   {
     derived() = derived() - B;
     return derived();
   }

   template<typename OtherDerived>
   inline Derived& operator -= (const ArrayBase<OtherDerived> &B) 
   {
     derived() = derived() - B.derived();
     return derived();
   }

   /************************************************/

   FortCpp_BINARY_OP(AddBinOp,+)
   FortCpp_BINARY_OP(SubBinOp,-)
   FortCpp_BINARY_OP(MulBinOp,*)
   FortCpp_BINARY_OP(DivBinOp,/)
   FortCpp_BINARY_OP(EqBinOp,==)
   FortCpp_BINARY_OP(NotEqBinOp,!=)

   FortCpp_UNARY_FUNC(SqrtUnOp,sqrt)
   FortCpp_UNARY_FUNC(SinUnOp,sin)
   FortCpp_UNARY_FUNC(CosUnOp,cos)

   inline const UnaryOp<Derived,NegUnOp<T> >operator -() const
   {
     return UnaryOp<Derived,NegUnOp<T> >(this->derived(),NegUnOp<T>());
   }

   inline explicit operator bool() const {
     for(int i=0;i<derived().size();i++){
       if(derived()[i]) return 1;
     }
     return 0;
   }
   /*********************************************/

  inline const int size() const { return this->derived().size(); }

  inline const Derived& derived() const { return *static_cast<const Derived*>(this); }
  inline       Derived& derived() { return *static_cast<Derived*>(this); }

  friend std::ostream& operator << (std::ostream& os, const ArrayBase<Derived> &A){
    for(int i=0;i<A.size();i++){
      os << A.derived()[i] << ' ';
    }
    return os;
  }
};

/**********************************************************************/

}; // end namespace FortCpp



#endif
