
#ifndef FortCpp_ARRAYBASE_H
#define FortCpp_ARRAYBASE_H

#include <array>
#include <cmath>

#include "Macros.h"
#include "ForwardDeclarations.h"

#ifndef NDEBUG
#include "ArrayException.h"
#include "Debug.h"
#endif

#include "Util.h"
#include "FixedUtil.h"
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

	ArrayBase()=default;
	ArrayBase(const ArrayBase<Derived>&)=default;
	ArrayBase(ArrayBase<Derived>&&)=default;
	~ArrayBase()=default;


	inline Derived& operator = (const ArrayBase<Derived>& B) {
#ifndef NDEBUG
		internal::debug::compare_dims(derived(),B.derived());
#endif
#pragma GCC ivdep
		for (int i=0; i<derived().size(); i++) {
			derived()[i] = B.derived()[i];
		}
		return derived();
	}

	inline Derived& operator = (ArrayBase<Derived>&& B) {
#ifndef NDEBUG
		internal::debug::compare_dims(derived(),B.derived());
#endif
#pragma GCC ivdep
		for (int i=0; i<derived().size(); i++) {
			derived()[i] = B.derived()[i];
		}
		return derived();
	}

	/**
	 * Sets the entire array to a given value
	 */
	inline const T& operator = (const T& B) {
#pragma GCC ivdep
		for (int i=0; i < derived().size(); i++) {
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
		for (int i=0; i<derived().size(); i++) {
			derived()[i] = B.derived()[i];
		}
		return derived();
	}


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

	FortCpp_BASE_BINARY_OP(AddBinOp,+)
	FortCpp_BASE_BINARY_OP(SubBinOp,-)
	FortCpp_BASE_BINARY_OP(MulBinOp,*)
	FortCpp_BASE_BINARY_OP(DivBinOp,/)
	FortCpp_BASE_BINARY_OP(LesBinOp,<)
	FortCpp_BASE_BINARY_OP(GreBinOp,>)
	FortCpp_BASE_BINARY_OP(LEqBinOp,<=)
	FortCpp_BASE_BINARY_OP(GEqBinOp,>=)
	FortCpp_BASE_BINARY_OP(EquBinOp,==)
	FortCpp_BASE_BINARY_OP(NEqBinOp,!=)

	FortCpp_BASE_UNARY_OP(SinUnOp,sin)
	FortCpp_BASE_UNARY_OP(CosUnOp,cos)
	FortCpp_BASE_UNARY_OP(SqtUnOp,sqrt)
	FortCpp_BASE_UNARY_OP(FloorUnOp,floor)

	inline const UnaryOp<Derived,NegUnOp<T> >operator -() const {
		return UnaryOp<Derived,NegUnOp<T> >(derived(),NegUnOp<T>());
	}

	const Derived& derived() const { return *static_cast<const Derived*>(this); }
	Derived& derived() { return *static_cast<Derived*>(this); }

	friend std::ostream& operator << (std::ostream& os, const ArrayBase<Derived>& A) {
		for (int i=0; i<A.derived().size(); i++) {
			os << A.derived()[i] << ' ';
		}
		return os;
	}
};

/**********************************************************************/

}; // end namespace FortCpp



#endif
