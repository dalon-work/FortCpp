#ifndef FortCpp_BINARYOP_H
#define FortCpp_BINARYOP_H

#include "ForwardDeclarations.h"

namespace FortCpp
{

namespace internal
{

template<typename Lhs,typename Rhs,typename Op>
struct traits<BinaryOp<Lhs,Rhs,Op> > {
	typedef typename Op::ReturnType Scalar;
	enum {
	    Rank = traits<Lhs>::Rank
	};
};



}; // end namespace internal

/*
 * Binary Op
 */
template<typename Lhs,typename Rhs,typename Op>
class BinaryOp : public ArrayBase<BinaryOp<Lhs,Rhs,Op> >
{
   typedef typename Op::ReturnType ReturnType;
	typedef BinaryOp<Lhs,Rhs,Op> Derived;

protected:
	const Lhs& _lhs;
	const Rhs& _rhs;
	const Op&  _op;

public:
	BinaryOp(const Lhs& lhs,const Rhs& rhs,const Op& op): _lhs(lhs), _rhs(rhs), _op(op) {
	};
	BinaryOp(const BinaryOp& A) : _lhs(A._lhs), _rhs(A._rhs), _op(A._op) { }

	const ReturnType operator [] (const int& i) const {
		return _op.eval(static_cast<ReturnType>(_lhs[i]),static_cast<ReturnType>(_rhs[i]));
	}

	const Lhs& get_lhs() const { return _lhs; }
	const Rhs& get_rhs() const { return _rhs; }

};

FortCpp_BINARY_OP(AddBinOp,+)
FortCpp_BINARY_OP(SubBinOp,-)
FortCpp_BINARY_OP(MulBinOp,*)
FortCpp_BINARY_OP(DivBinOp,/)
FortCpp_BINARY_OP(LesBinOp,<)
FortCpp_BINARY_OP(GreBinOp,>)
FortCpp_BINARY_OP(LEqBinOp,<=)
FortCpp_BINARY_OP(GEqBinOp,>=)
FortCpp_BINARY_OP(EquBinOp,==)
FortCpp_BINARY_OP(NEqBinOp,!=)

}; // end namespace FortCpp
#endif
