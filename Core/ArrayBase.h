#ifndef FortCpp_ARRAYBASE_H
#define FortCpp_ARRAYBASE_H

#include "Macros.h"
#include "ForwardDeclarations.h"

#ifndef NDEBUG
#include "ArrayException.h"
#include "Debug.h"
#endif

#include "Util.h"
#include "Storage.h"
#include "BinaryOp.h"
#include "ConstantOp.h"
#include "UnaryOp.h"
#include "Slice.h"
#include "StrideCalc.h"

namespace FortCpp
{

template <typename Derived>
class ArrayBase
{

public:

	typedef typename internal::traits<Derived>::Scalar T;
	static const int Rank = internal::traits<Derived>::Rank;

	ArrayBase()=default;
	ArrayBase(const ArrayBase<Derived>&)=default;
	ArrayBase(ArrayBase<Derived>&&)=default;
	~ArrayBase()=default;

	/**
	 * Sets the entire array to a given value
	 */
	inline const T& operator = (const T& B) {
#pragma GCC ivdep
		for (int i=0; i < size(); i++) {
			derived()[i] = B;
		}
		return B;
	}

	template<typename OtherDerived>
	inline Derived& operator = (const ArrayBase<OtherDerived>& B) {
#ifndef NDEBUG
      internal::debug::compare_dims(derived(),B.derived());
#endif
#pragma GCC ivdep
		for (int i=0; i<this->size(); i++) {
			derived()[i] = B.derived()[i];
		}
		return derived();
	}

	inline Derived& operator = (const ArrayBase<Derived>& B) {
#ifndef NDEBUG
      internal::debug::compare_dims(derived(),B.derived());
#endif
#pragma GCC ivdep
		for (int i=0; i<this->size(); i++) {
			derived()[i] = B.derived()[i];
		}
		return derived();
	}

	constexpr int size() const { return derived().size(); }
	constexpr int size(int i) const { return derived().size(i); }

	/**
	 * += and -= operators
	 */
	inline Derived& operator += (const T& B) {
		derived() = derived() + B;
		return derived();
	}

	template<typename OtherDerived>
	inline Derived& operator += (const ArrayBase<OtherDerived>& B) {
#ifndef NDEBUG
      internal::debug::compare_dims(derived(),B.derived());
#endif
		derived() = derived() + B.derived();
		return derived();
	}

	inline Derived& operator -= (const T& B) {
		derived() = derived() - B;
		return derived();
	}

	template<typename OtherDerived>
	inline Derived& operator -= (const ArrayBase<OtherDerived>& B) {
#ifndef NDEBUG
      internal::debug::compare_dims(derived(),B.derived());
#endif
		derived() = derived() - B.derived();
		return derived();
	}

	/************************************************/


	FortCpp_BINARY_OP(AddBinOp,+)
	FortCpp_BINARY_OP(SubBinOp,-)
	FortCpp_BINARY_OP(MulBinOp,*)
	FortCpp_BINARY_OP(DivBinOp,/)

	FortCpp_BINARY_OP(EqBinOp,==)
	FortCpp_BINARY_OP(NotEqBinOp,!=)

	FortCpp_UNARY_FUNC(SqrtUnOp,sqrt)
	FortCpp_UNARY_FUNC(SinUnOp,sin)
	FortCpp_UNARY_FUNC(CosUnOp,cos)

	inline const UnaryOp<Derived,NegUnOp<T> >operator -() const {
		return UnaryOp<Derived,NegUnOp<T> >(this->derived(),NegUnOp<T>());
	}

	//
	//  inline explicit operator bool() const {
	//    for(int i=0;i<derived().size();i++){
	//      if(derived()[i]) return 1;
	//    }
	//    return 0;
	//  }

	const Derived& derived() const { return *static_cast<const Derived*>(this); }
	Derived& derived() { return *static_cast<Derived*>(this); }

	friend std::ostream& operator << (std::ostream& os, const ArrayBase<Derived>& A) {
		for (int i=0; i<A.size(); i++) {
			os << A.derived()[i] << ' ';
		}
		return os;
	}
};

/**********************************************************************/

}; // end namespace FortCpp



#endif
