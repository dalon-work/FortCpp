
#define FortCpp_NOT_STATIC_FUNCTION_ASSERT static_assert(int(internal::traits<Derived>::Size) == int(internal::Unknown),"YOU_TRIED_TO_CALL_A_DYNAMIC_FUNCTION_ON_A_STATIC_ARRAY");
#define FortCpp_IS_STATIC_FUNCTION_ASSERT static_assert(int(internal::traits<Derived>::Size) != int(internal::Unknown),"YOU_TRIED_TO_CALL_A_STATIC_FUNCTION_ON_A_DYNAMIC_ARRAY");

#define FortCpp_SLICE_TEMPLATE(D) int _Type ## D , int _L ## D , int _S ## D

#define FortCpp_SLICE_0(D) internal::Slice<_Type ## D, 0, _L ## D, _S ## D>

#define FortCpp_SLICE(D) internal::Slice<_Type ## D, D, _L ## D, _S ## D>

#define FortCpp_SLICE_RUNTIME_LENGTH(D) internal::runtime_length<_Type ## D , D ,AccessorDerived , FortCpp_SLICE_0( D )>::result(_accessor,S ## D);

#define FortCpp_SLICE_RUNTIME_STRIDE(ST,D) internal::runtime_stride<Stride ## ST, D ,AccessorDerived, FortCpp_SLICE_0( D ) >::result(_accessor,S ## D);


#define FortCpp_BINARY_OP(OP,X) template <typename OtherDerived> \
   inline const BinaryOp<Derived,OtherDerived,OP<T> >operator X \
     (const OtherDerived &rhs) const \
   { \
     return BinaryOp<Derived,OtherDerived,OP<T> >(this->derived(),rhs,OP<T>()); \
   } \

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

#ifdef NDEBUG
#define FortCpp_SIZE_MATCH(a,b)
#define FortCpp_BOUNDS_CHECK(i,D)
#define FortCpp_LINEAR_SIZE_CHECK(i)
#define FortCpp_SLICE_CHECK(D)
#else


#define FortCpp_SIZE_MATCH(a,b) try{                            \
                                if((a) != (b)){                 \
                                  throw SizeException((a),(b)); \
                                }                           \
                            }                               \
                            catch(std::exception &e){       \
                              FortCpp::ArrayException::handle_exception(e); \
                            }

#define FortCpp_BOUNDS_CHECK(i,D) try{ \
                                  if( (i) >= d ## D() ){ \
                                    throw BoundsException((i),(D),d ## D()); \
                                  } \
                                  else if( (i) < 0 ){ \
                                    throw BoundsException((i),(D),d ## D()); \
                                  } \
                              } \
                              catch(std::exception &e){ \
                                FortCpp::ArrayException::handle_exception(e); \
                              }

#define FortCpp_LINEAR_SIZE_CHECK(i) try{ \
                                   if( (i) >= size() ){ \
                                     throw LinearSizeException( (i),size() ); \
                                   } \
                                   else if( (i) < 0 ){ \
                                     throw LinearSizeException( (i),size() ); \
                                   } \
                                 } \
                                 catch(std::exception &e){ \
                                   FortCpp::ArrayException::handle_exception(e); \
                                 }

#define FortCpp_SLICE_CHECK(D) try{ \
                                int sl = S ## D.b() + l ## D  - 1; \
                                   if( sl >= d ## D() ){ \
                                     throw BoundsException(sl, D , d ## D() ); \
                                   } \
                                   else if( sl < 0 ){ \
                                     throw BoundsException(sl, D , d ## D() ); \
                                   } \
                                 } \
                                 catch(std::exception &e){ \
                                   FortCpp::ArrayException::handle_exception(e); \
                                 }

#endif

