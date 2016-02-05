#ifndef CHECKDIMS_H
#define CHECKDIMS_H

namespace FortCpp
{
namespace internal
{

template<typename Lhs,typename T,unsigned Rank,unsigned Options>
bool compare_dims(const Lhs &lhs, const Alloc<T,Rank,Options> &rhs){
   return lhs.check_dim() == rhs.check_dim();
}

template<typename Lhs,typename Lhs2,typename Rhs,typename Op>
bool compare_dims(const Lhs & lhs, const BinaryOp<Lhs2,Rhs,Op>& rhs)
{
   return compare_dims(lhs,rhs.get_lhs()) && compare_dims(lhs,rhs.get_rhs());
}

template<typename Lhs,unsigned Side,typename Derived,typename Op>
bool compare_dims(const Lhs& lhs,const ConstantOp<Side,Derived,Op> &rhs){
   return compare_dims(lhs,rhs.getExpr());
}

template<typename Lhs,typename Rhs,typename Op>
bool compare_dims(const Lhs& lhs,const UnaryOp<Rhs,Op>& rhs){
   return compare_dims(lhs,rhs.getExpr());
}

};
};

#endif
