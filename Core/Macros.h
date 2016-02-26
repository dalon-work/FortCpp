
#define FortCpp_BASE_BINARY_OP(OP,X) template <typename OtherDerived> \
   inline const BinaryOp<Derived,OtherDerived,OP<T,typename internal::traits<OtherDerived>::Scalar> >operator X \
     (const ArrayBase<OtherDerived> &rhs) const \
   { \
      typedef typename internal::traits<OtherDerived>::Scalar T2; \
     return BinaryOp<Derived,OtherDerived,OP<T,T2> >(this->derived(),rhs.derived(),OP<T,T2>()); \
   }

#define FortCpp_BINARY_OP(OP,X) \
template<typename T1,typename T2> \
struct OP { \
   typedef decltype( std::declval<T1>() X std::declval<T2>() ) ReturnType; \
	inline static const ReturnType eval(const T1& _lhs,const T2& _rhs) { \
		return _lhs X _rhs; \
	} \
}; 
 
#define FortCpp_UNARY_FUNC(OP,X) \
  inline const UnaryOp<Derived,OP<T> > X() const \
  { \
    return UnaryOp<Derived,OP<T> >(this->derived(),OP<T>()); \
  }

#define FortCpp_UNARY_OP(NAME,FUNC) \
  template<typename T> \
  struct NAME{ \
    inline static const T eval (const T &_rhs){ \
     return FUNC(_rhs); \
    } \
  };

#define FortCpp_LHS_CONSTANT_OP(OP,X)  \
   template <typename Lhs,typename Derived> \
   inline const ConstantOp<internal::LHS, \
                           typename std::enable_if<!internal::is_array<Lhs>::value,Lhs>::type, \
                           Derived, \
                           OP<Lhs,typename internal::traits<Derived>::Scalar> > \
                           operator X \
     (const Lhs& lhs,const ArrayBase<Derived> &rhs) \
   { \
      typedef Lhs T1; \
      typedef typename internal::traits<Derived>::Scalar T2; \
     return ConstantOp<internal::LHS,Lhs,Derived,OP<T1,T2> >(lhs,rhs.derived(),OP<T1,T2>()); \
   }

#define FortCpp_RHS_CONSTANT_OP(OP,X) \
   template <typename Derived,typename Rhs> \
   inline const ConstantOp<internal::RHS, \
                           Derived, \
                           typename std::enable_if<!internal::is_array<Rhs>::value,Rhs>::type, \
                           OP<typename internal::traits<Derived>::Scalar,Rhs> > \
                           operator X \
     (const ArrayBase<Derived> &lhs,const Rhs& rhs) \
   { \
      typedef typename internal::traits<Derived>::Scalar T1; \
      typedef Rhs T2; \
     return ConstantOp<internal::RHS,Derived,Rhs,OP<T1,T2> >(lhs.derived(),rhs,OP<T1,T2>()); \
   }

