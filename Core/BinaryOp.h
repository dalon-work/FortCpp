#ifndef FortCpp_BINARYOP_H
#define FortCpp_BINARYOP_H

namespace FortCpp
{

namespace internal
{

template<typename Lhs,typename Rhs,typename Op>
struct traits<BinaryOp<Lhs,Rhs,Op> >
{
  typedef typename traits<Rhs>::Scalar Scalar;
  enum{
    Size = (int(traits<Lhs>::Size) != int(Unknown)) ? int(traits<Lhs>::Size) : (int(traits<Rhs>::Size) != int(Unknown)) ? int(traits<Rhs>::Size) : int(Unknown)
  };
};
}; // end namespace internal

/*
 * Binary Op
 */
template<typename Lhs,typename Rhs,typename Op>
class BinaryOp : public ArrayBase<BinaryOp<Lhs,Rhs,Op> >
{
  typedef typename internal::traits<Rhs>::Scalar T;
  typedef BinaryOp<Lhs,Rhs,Op> Derived;
  protected:
  const Lhs &_lhs;
  const Rhs &_rhs;
  const Op  &_op;
  const internal::OpSize<internal::traits<Derived>::Size> _size;

  public:
  inline BinaryOp(const Lhs &lhs,const Rhs &rhs,const Op &op): _lhs(lhs), _rhs(rhs), _op(op), _size(lhs.size()) { }
  inline BinaryOp(const BinaryOp &A) : _lhs(A._lhs), _rhs(A._rhs), _op(A._op),_size(A.size()) { }

  inline const T operator [] (const int &i) const{
    return _op.eval(_lhs[i],_rhs[i]);
  }
  inline const int size() const { return _size.size(); }
};

/*
 * Addition Operation
 */
template<typename T>
struct AddBinOp{
  inline static const T eval(const T & _lhs,const T &_rhs){
    return _lhs + _rhs;
  }
};

/*
 * Subtraction Operation
 */
template<typename T>
struct SubBinOp{
  inline static const T eval(const T & _lhs,const T & _rhs){
    return _lhs - _rhs;
  }
};

/*
 * Multiplication Operation
 */
template<typename T>
struct MulBinOp{
  inline static const T eval(const T & _lhs,const T & _rhs){
    return _lhs * _rhs;
  }
};

/*
 * Division Operation
 */
template<typename T>
struct DivBinOp{
  inline static const T eval(const T & _lhs,const T & _rhs){
    return _lhs / _rhs;
  }
};

/*
 * Less than Operation
 */
template<typename T>
struct LessBinOp{
  inline static const bool eval(const T & _lhs,const T & _rhs){
    return _lhs < _rhs;
  }
};

/* 
 * Equal Operation
 */
template<typename T>
struct EqBinOp{
  inline static const bool eval(const T & _lhs,const T &_rhs){
    return _lhs == _rhs;
  }
};

/*
 * Not-Equal Operation
 */
template<typename T>
struct NotEqBinOp{
  inline static const bool eval(const T & _lhs,const T & _rhs){
    return _lhs != _rhs;
  }
};
}; // end namespace FortCpp
#endif
