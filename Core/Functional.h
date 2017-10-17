#ifndef FortCpp_FUNCTIONAL_H
#define FortCpp_FUNCTIONAL_H

#include "ArrayBase.h"
#include <assert.h>
#include <iostream>

namespace FortCpp
{

namespace internal
{

template<typename Rhs,typename ReturnType,typename FuncType>
struct traits< FuncTypeOp<Rhs,ReturnType,FuncType> > {
	typedef ReturnType Scalar;
	enum {
	    Rank = traits<Rhs>::Rank
	};

}; // end namespace internal

};


template< typename Rhs, typename ReturnType, typename FuncType>
class FuncTypeOp : public ArrayBase< FuncTypeOp<Rhs,ReturnType,FuncType> >
{
	typedef class FuncTypeOp<Rhs, ReturnType, FuncType> Derived;
	typedef ArrayBase<Derived> Base;

   protected:
   Rhs& _rhs;
   FuncType _func;

   public:
   inline FuncTypeOp(Rhs& rhs,FuncType& func): _rhs(rhs), _func(func)
   { }
   inline FuncTypeOp(const FuncTypeOp& A) : _rhs(A._rhs), _func(A._func)
   { }
   inline ReturnType operator [] (const int& i) const {
      return _func(_rhs[i]);
   }

   inline const Rhs& getExpr() const {
      return _rhs;
   }

   unsigned size() const { return _rhs.size(); }

};

template< typename Rhs, typename FuncType>
class FuncTypeOp<Rhs,void,FuncType> 
{
   protected:
   public:
   inline FuncTypeOp(Rhs& rhs,FuncType& func)
   {
      for (int i=0; i<rhs.size(); i++) {
         func(rhs[i]);
      }
   }
};


}; // end namespace FortCpp

#endif
