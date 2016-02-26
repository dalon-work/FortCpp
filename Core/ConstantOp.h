#ifndef FortCpp_cONSTANTOP_h
#define FortCpp_cONSTANTOP_h

namespace FortCpp
{

namespace internal
{

enum
{
   LHS,
   RHS
};

/*
 * Traits specialization for ConstantOp
 */
template<typename Lhs,typename Rhs,typename Op>
struct traits<ConstantOp<LHS,Lhs,Rhs,Op> > {
	typedef typename Op::ReturnType Scalar;
   static const int Rank = traits<Rhs>::Rank;
};

template<typename Lhs,typename Rhs,typename Op>
struct traits<ConstantOp<RHS,Lhs,Rhs,Op> > {
	typedef typename Op::ReturnType Scalar;
   static const int Rank = traits<Lhs>::Rank;
};

}; // end namespace internal

/*
 * Constant Op, with the constant on the LHS
 */
template<typename Lhs,typename Rhs,typename Op>
class ConstantOp<internal::LHS,Lhs,Rhs,Op> : public ArrayBase<ConstantOp<internal::LHS,Lhs,Rhs,Op> >
{
  typedef ConstantOp<internal::LHS,Lhs,Rhs,Op> Derived;
  typedef typename Op::ReturnType T;
  protected:
  const Lhs &_c;
  const Rhs &_rhs;
  const Op  &_op;

  public:
  inline ConstantOp(const Lhs &C, const Rhs &rhs,const Op &op): _c(C), _rhs(rhs), _op(op) {};
  inline ConstantOp(const ConstantOp &A) : _c(A._c), _rhs(A._rhs), _op(A._op)
  { }

  inline const T operator [] (const int &i) const{
    return _op.eval(_c,_rhs[i]);
  }

  inline const Rhs& getExpr() const { return _rhs; }

};

/*
 * Constant Op, with the constant on the RHS
 */
template<typename Lhs,typename Rhs,typename Op>
class ConstantOp<internal::RHS,Lhs,Rhs,Op> : public ArrayBase<ConstantOp<internal::RHS,Lhs,Rhs,Op> >
{
  typedef ConstantOp<internal::LHS,Lhs,Rhs,Op> Derived;
  typedef typename Op::ReturnType T;
  protected:
  const Lhs &_lhs;
  const Rhs &_c;
  const Op  &_op;

  public:
  inline ConstantOp(const Lhs &lhs,const Rhs &C, const Op &op): _c(C), _lhs(lhs), _op(op) {}
  inline ConstantOp(const ConstantOp &A) : _c(A._c), _lhs(A._lhs), _op(A._op) {}

  inline const T operator [] (const int &i) const{
    return _op.eval(_lhs[i],_c);
  }

  inline const Lhs& getExpr() const { return _lhs; }

};

FortCpp_LHS_CONSTANT_OP(AddBinOp,+)
FortCpp_LHS_CONSTANT_OP(SubBinOp,-)
FortCpp_LHS_CONSTANT_OP(MulBinOp,*)
FortCpp_LHS_CONSTANT_OP(DivBinOp,/)
FortCpp_LHS_CONSTANT_OP(LesBinOp,<)
FortCpp_LHS_CONSTANT_OP(GreBinOp,>)
FortCpp_LHS_CONSTANT_OP(LEqBinOp,<=)
FortCpp_LHS_CONSTANT_OP(GEqBinOp,>=)
FortCpp_LHS_CONSTANT_OP(EquBinOp,==)
FortCpp_LHS_CONSTANT_OP(NEqBinOp,!=)

FortCpp_RHS_CONSTANT_OP(AddBinOp,+)
FortCpp_RHS_CONSTANT_OP(SubBinOp,-)
FortCpp_RHS_CONSTANT_OP(MulBinOp,*)
FortCpp_RHS_CONSTANT_OP(DivBinOp,/)
FortCpp_RHS_CONSTANT_OP(LesBinOp,<)
FortCpp_RHS_CONSTANT_OP(GreBinOp,>)
FortCpp_RHS_CONSTANT_OP(LEqBinOp,<=)
FortCpp_RHS_CONSTANT_OP(GEqBinOp,>=)
FortCpp_RHS_CONSTANT_OP(EquBinOp,==)
FortCpp_RHS_CONSTANT_OP(NEqBinOp,!=)

}; // end namespace FortCpp
#endif
