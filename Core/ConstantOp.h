#ifndef FortCpp_CONSTANTOP_H
#define FortCpp_CONSTANTOP_H

namespace FortCpp
{

namespace internal
{

enum CONSTANT_SIDE
{
  LHS,
  RHS
};

/*
 * Traits specialization for ConstantOp
 */
template<int Side,typename Derived,typename Op>
struct traits<ConstantOp<Side,Derived,Op> >
{
  typedef typename traits<Derived>::Scalar Scalar;
  enum{
    Size = (int(traits<Derived>::Size) != int(Unknown)) ? int(traits<Derived>::Size) : int(Unknown)
  };
};

}; // end namespace internal

/*
 * Constant Op, with the constant on the LHS
 */
template<typename Rhs,typename Op>
class ConstantOp<internal::LHS,Rhs,Op> : public ArrayBase<ConstantOp<internal::LHS,Rhs,Op> >
{
  typedef typename internal::traits<Rhs>::Scalar T;
  typedef ConstantOp<internal::LHS,Rhs,Op> Derived;
  protected:
  const T &_C;
  const Rhs &_rhs;
  const Op  &_op;
  const internal::OpSize<internal::traits<Derived>::Size> _size;

  public:
  inline ConstantOp(const T &C, const Rhs &rhs,const Op &op): _C(C), _rhs(rhs), _op(op), _size(rhs.size())
  { }
  inline ConstantOp(const ConstantOp &A) : _C(A._C), _rhs(A._rhs), _op(A._op), _size(A.size())
  { }

  inline const T operator [] (const int &i) const{
    return _op.eval(_C,_rhs[i]);
  }

  inline const int size() const { return _size.size(); }
};

/*
 * Constant Op, with the constant on the RHS
 */
template<typename Lhs,typename Op>
class ConstantOp<internal::RHS,Lhs,Op> : public ArrayBase<ConstantOp<internal::RHS,Lhs,Op> >
{
  typedef typename internal::traits<Lhs>::Scalar T;
  typedef ConstantOp<internal::LHS,Lhs,Op> Derived;
  protected:
  const T &_C;
  const Lhs &_lhs;
  const Op  &_op;
  const internal::OpSize<internal::traits<Derived>::Size> _size;

  public:
  inline ConstantOp(const Lhs &lhs,const T &C, const Op &op): _C(C), _lhs(lhs), _op(op), _size(lhs.size())
  { }
  inline ConstantOp(const ConstantOp &A) : _C(A._C), _lhs(A._lhs), _op(A._op), _size(A.size())
  { }

  inline const T operator [] (const int &i) const{
    return _op.eval(_lhs[i],_C);
  }

  inline const int size() const { return _size.size(); }
};


/**
* Addition Operator of an Array and a Scalar
*/
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,AddBinOp<typename internal::traits<Derived>::Scalar> >operator +
 (const typename internal::traits<Derived>::Scalar &C,const Derived &rhs) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::LHS,Derived,AddBinOp<T> >(C,rhs,AddBinOp<T>());
}

template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,AddBinOp<typename internal::traits<Derived>::Scalar> >operator +
 (const Derived &lhs,const typename internal::traits<Derived>::Scalar &C) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::RHS,Derived,AddBinOp<T> >(lhs,C,AddBinOp<T>());
}

/**
* Subtraction Operator of an Array and a Scalar
*/
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,SubBinOp<typename internal::traits<Derived>::Scalar> >operator -
 (const typename internal::traits<Derived>::Scalar &C,const Derived &rhs) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::LHS,Derived,SubBinOp<T> >(C,rhs,SubBinOp<T>());
}

template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,SubBinOp<typename internal::traits<Derived>::Scalar> >operator -
 (const Derived &lhs,const typename internal::traits<Derived>::Scalar &C) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::RHS,Derived,SubBinOp<T> >(lhs,C,SubBinOp<T>());
}

/**
* Multiplication Operator of an Array and a Scalar
*/
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,MulBinOp<typename internal::traits<Derived>::Scalar> >operator *
 (const typename internal::traits<Derived>::Scalar &C,const Derived &rhs) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::LHS,Derived,MulBinOp<T> >(C,rhs,MulBinOp<T>());
}

template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,MulBinOp<typename internal::traits<Derived>::Scalar> >operator *
 (const Derived &lhs,const typename internal::traits<Derived>::Scalar &C) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::RHS,Derived,MulBinOp<T> >(lhs,C,MulBinOp<T>());
}

/**
* Division Operator of an Array and a Scalar
*/
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,DivBinOp<typename internal::traits<Derived>::Scalar> >operator /
 (const typename internal::traits<Derived>::Scalar &C,const Derived &rhs) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::LHS,Derived,DivBinOp<T> >(C,rhs,DivBinOp<T>());
}

template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,DivBinOp<typename internal::traits<Derived>::Scalar> >operator /
(const Derived &lhs,const typename internal::traits<Derived>::Scalar &C) 
{
  typedef typename internal::traits<Derived>::Scalar T;
 return ConstantOp<internal::RHS,Derived,DivBinOp<T> >(lhs,C,DivBinOp<T>());
}

/** 
 * Less Than Operator of an Array and a Scalar
 */
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,LessBinOp<typename internal::traits<Derived>::Scalar> > operator <
  (const typename internal::traits<Derived>::Scalar &lhs, const Derived &rhs)
{
  typedef typename internal::traits<Derived>::Scalar T;
  return ConstantOp<internal::LHS,Derived,LessBinOp<T> >(lhs,rhs,LessBinOp<T>());
}
template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,LessBinOp<typename internal::traits<Derived>::Scalar> > operator <
  (const Derived &lhs, const typename internal::traits<Derived>::Scalar &rhs)
{
  typedef typename internal::traits<Derived>::Scalar T;
  return ConstantOp<internal::RHS,Derived,LessBinOp<T> >(lhs,rhs,LessBinOp<T>());
}

/**
 * Equality Operator
 */
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,EqBinOp<typename internal::traits<Derived>::Scalar> > operator ==
  (const typename internal::traits<Derived>::Scalar &lhs, const Derived &rhs)
{
  typedef typename internal::traits<Derived>::Scalar T;
  return ConstantOp<internal::LHS,Derived,EqBinOp<T> >(lhs,rhs,EqBinOp<T>());
}
template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,EqBinOp<typename internal::traits<Derived>::Scalar> > operator ==
  (const Derived &lhs, const typename internal::traits<Derived>::Scalar &rhs)
{
  typedef typename internal::traits<Derived>::Scalar T;
  return ConstantOp<internal::RHS,Derived,EqBinOp<T> >(lhs,rhs,EqBinOp<T>());
}

/**
 * Not-Equality Operator
 */
template <typename Derived>
inline const ConstantOp<internal::LHS,Derived,NotEqBinOp<typename internal::traits<Derived>::Scalar> > operator !=
  (const typename internal::traits<Derived>::Scalar &lhs, const Derived &rhs)
{
  typedef typename internal::traits<Derived>::Scalar T;
  return ConstantOp<internal::LHS,Derived,NotEqBinOp<T> >(lhs,rhs,NotEqBinOp<T>());
}
template <typename Derived>
inline const ConstantOp<internal::RHS,Derived,NotEqBinOp<typename internal::traits<Derived>::Scalar> > operator !=
  (const Derived &lhs, const typename internal::traits<Derived>::Scalar &rhs)
{
  typedef typename internal::traits<Derived>::Scalar T;
  return ConstantOp<internal::RHS,Derived,NotEqBinOp<T> >(lhs,rhs,NotEqBinOp<T>());
}


}; // end namespace FortCpp
#endif