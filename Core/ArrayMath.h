#ifndef FortCpp_ARRAYMATH_H
#define FortCpp_ARRAYMATH_H

namespace FortCpp
{

namespace Math
{

template<typename T>
Alloc<T,1> reverse(const Alloc<T,1>& B)
{
   Alloc<T,1> A;
   A.mold(B);
   int m=B.size()-1;;
   for(int i=0;i<B.size();i++){
      A[i] = B[m--];
   }
}

template<typename T=double>
Alloc<T,1> linspace(long double start, long double stop, int num,bool endpoint=1){
 Alloc<T,1> A;
 A.allocate(num);
 int end = endpoint ? 1 : 0;
 long double dx = (stop-start)/static_cast<long double>(num-end);

 for(int i=0;i<num;i++){
   A[i] = static_cast<T>(start+static_cast<long double>(i)*dx);
 }

 return A;
}

template<typename T>
void cshift(Alloc<T,1>& A,int num=1){
   Alloc<T,1> keep(num);
   auto beg = A.view(Slice( BEG,num));
   auto end = A.view(Slice(-num,END));
 if(num > 0){
    Alloc<T,1> keep(num); 
    keep = beg;
    for(int i=0;i<A.size()-num;i++){
       A[i]=A[i+num];
    }
    end = keep;
 }
 else if(num < 0){
    Alloc<T,1> keep(num);
    keep = end;
    for(int i=A.size()-1;i>=num;i--){
       A[i] = A[i-num];
    }
    beg = keep;
 }
}

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

// UnaryOp stuff

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
