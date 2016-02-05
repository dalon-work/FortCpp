#ifndef FortCpp_UNARYOP_H
#define FortCpp_UNARYOP_H

namespace FortCpp
{

namespace internal
{
template<typename Rhs,typename Op>
struct traits<UnaryOp<Rhs,Op> >
{
  typedef typename traits<Rhs>::Scalar Scalar;
  enum{
     Rank = traits<Rhs>::Rank
  };
};
}; // end namespace internal

/*
 * Unary Op
 */
template<typename Rhs,typename Op>
class UnaryOp : public ArrayBase<UnaryOp<Rhs,Op> >
{
  typedef typename internal::traits<Rhs>::Scalar T;
  typedef UnaryOp<Rhs,Op> Derived;
  protected:
  const Rhs &_rhs;
  const Op  &_op;

  public:
  inline UnaryOp(const Rhs &rhs,const Op &op): _rhs(rhs), _op(op)
  { }
  inline UnaryOp(const UnaryOp &A) : _rhs(A._rhs), _op(A._op)
  { }

  inline const T operator [] (const int &i) const{
    return _op.eval(_rhs[i]);
  }

  inline const Rhs& getExpr() const {
     return _rhs;
  }

};

/*
 * Define functors
 * These should have matching functions in ArrayBase.h and ArrayMath.h
 */

template<typename T>
struct NegUnOp{
  inline static const T eval (const T &_rhs){
    return -_rhs;
  }
};

FortCpp_UNARY_OP(SqrtUnOp,sqrt)
FortCpp_UNARY_OP(SinUnOp,sin)
FortCpp_UNARY_OP(CosUnOp,cos)

}; // end namespace FortCpp
#endif
