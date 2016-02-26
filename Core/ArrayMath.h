#ifndef FortCpp_ARRAYMATH_H
#define FortCpp_ARRAYMATH_H

namespace FortCpp
{

namespace Math
{

template<typename Derived>
int count(const ArrayBase<Derived>& rhs)
{
	int c = 0;
	for (int i=0; i<rhs.size(); i++) {
		if (rhs.derived()[i]) { c++; }
	}
	return c;
}

template<typename Derived>
bool isnan(const ArrayBase<Derived>& rhs)
{
	for (int i=0; i<rhs.size(); i++) {
		if (rhs.derived()[i] != rhs.derived()[i]) { return 1; }
	}
	return 0;
}

template<typename Derived>
bool any(const ArrayBase<Derived>& rhs)
{
	for (int i=0; i<rhs.size(); i++) {
		if (rhs.derived()[i]) { return 1; }
	}
	return 0;
}

template<typename Derived>
bool all(const ArrayBase<Derived>& rhs)
{
	for (int i=0; i<rhs.size(); i++) {
		if (!rhs.derived()[i]) { return 0; }
	}
	return 1;
}

template<typename Derived>
typename internal::traits<Derived>::Scalar sum(const ArrayBase<Derived>& rhs)
{
	typename internal::traits<Derived>::Scalar s;
	s = 0;
	for (int i=0; i<rhs.size(); i++) {
		s += rhs.derived()[i];
	}
	return s;
}

template<typename Derived>
typename internal::traits<Derived>::Scalar min(const ArrayBase<Derived>& rhs)
{
	typename internal::traits<Derived>::Scalar m;
	m = rhs.derived()[0];
	for (int i=1; i<rhs.size(); i++) {
		m = m < rhs.derived()[i] ? m : rhs.derived()[i];
	}
	return m;
}

template<typename Derived>
typename internal::traits<Derived>::Scalar max(const ArrayBase<Derived>& rhs)
{
	typename internal::traits<Derived>::Scalar m;
	m = rhs.derived()[0];
	for (int i=1; i<rhs.size(); i++) {
		m = m > rhs.derived()[i] ? m : rhs.derived()[i];
	}
	return m;
}


template<typename Derived>
UnaryOp<Derived,SqtUnOp<typename internal::traits<Derived>::Scalar> > sqrt(const ArrayBase<Derived>& rhs)
{
	return rhs.sqrt();
};

template<typename Derived>
UnaryOp<Derived,SinUnOp<typename internal::traits<Derived>::Scalar> > sin(const ArrayBase<Derived>& rhs)
{
	return rhs.sin();
};

template<typename Derived>
UnaryOp<Derived,CosUnOp<typename internal::traits<Derived>::Scalar> > cos(const ArrayBase<Derived>& rhs)
{
	return rhs.cos();
};

}; // end namespace Math

}; // end namespace FortCpp



#endif
