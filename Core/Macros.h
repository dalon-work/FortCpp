
#define FortCpp_BASE_BINARY_OP(OP,X) template <typename OtherDerived,typename T2> \
   inline const BinaryOp<Derived,OtherDerived,OP<T,T2> >operator X \
     (const ArrayBase<OtherDerived> &rhs) const \
   { \
      typedef typename internal::traits<OtherDerived>::Scalar T2; \
     return BinaryOp<Derived,OtherDerived,OP<T,T2> >(this->derived(),rhs.derived(),OP<OpT>()); \
   }

#define FortCpp_BINARY_OP(OP,X) \
template<typename T1,typename T2> \
struct OP { \
   typedef decltype( T + T2 ) OpT; \
	inline static const auto eval(const T& _lhs,const T& _rhs) { \
		return static_cast<OpT>(_lhs) X static_cast<OpT>(_rhs); \
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

#endif

