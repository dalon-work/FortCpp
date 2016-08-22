#ifndef FortCpp_ARRAYMATH_H
#define FortCpp_ARRAYMATH_H

namespace FortCpp
{

   using std::sqrt;
   using std::cos;
   using std::sin;
   using std::floor;

template<typename T>
Alloc<T,1> reverse(const Alloc<T,1>& B)
{
   Alloc<T,1> A;
   A.mold(B);
   int m=B.size()-1;
   for(int i=0;i<B.size();i++){
      A[i] = B[m--];
   }
   return A;
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
 if(num > 0){
    auto beg = A.view(Slice( BEG,num));
    auto end = A.view(Slice(-num,END));
    Alloc<T,1> keep(num); 
    keep = beg;
    for(int i=0;i<A.size()-num;i++){
       A[i]=A[i+num];
    }
    end = keep;
 }
 else if(num < 0){
    num = -num;
    auto beg = A.view(Slice( BEG,num));
    auto end = A.view(Slice(-num,END));
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
	for (int i=0; i<rhs.derived().size(); i++) {
		if (rhs.derived()[i]) { c++; }
	}
	return c;
}

template<typename Derived>
bool isnan(const ArrayBase<Derived>& rhs)
{
	for (int i=0; i<rhs.derived().size(); i++) {
		if (rhs.derived()[i] != rhs.derived()[i]) { return 1; }
	}
	return 0;
}

template<typename Derived>
bool any(const ArrayBase<Derived>& rhs)
{
	for (int i=0; i<rhs.derived().size(); i++) {
		if (rhs.derived()[i]) { return 1; }
	}
	return 0;
}

template<typename Derived>
bool all(const ArrayBase<Derived>& rhs)
{
	for (int i=0; i<rhs.derived().size(); i++) {
		if (!rhs.derived()[i]) { return 0; }
	}
	return 1;
}

template<typename Derived>
typename internal::traits<Derived>::Scalar sum(const ArrayBase<Derived>& rhs)
{
	typename internal::traits<Derived>::Scalar s;
	s = 0;
	for (int i=0; i<rhs.derived().size(); i++) {
		s += rhs.derived()[i];
	}
	return s;
}

template<typename Derived>
typename internal::traits<Derived>::Scalar min(const ArrayBase<Derived>& rhs)
{
	typename internal::traits<Derived>::Scalar m;
	m = rhs.derived()[0];
	for (int i=1; i<rhs.derived().size(); i++) {
		m = m < rhs.derived()[i] ? m : rhs.derived()[i];
	}
	return m;
}

template<typename Derived>
typename internal::traits<Derived>::Scalar max(const ArrayBase<Derived>& rhs)
{
	typename internal::traits<Derived>::Scalar m;
	m = rhs.derived()[0];
	for (int i=1; i<rhs.derived().size(); i++) {
		m = m > rhs.derived()[i] ? m : rhs.derived()[i];
	}
	return m;
}

// UnaryOp stuff

FortCpp_MATH_UNARY_OP(SqtUnOp,sqrt)
FortCpp_MATH_UNARY_OP(SinUnOp,sin)
FortCpp_MATH_UNARY_OP(CosUnOp,cos)
FortCpp_MATH_UNARY_OP(FloorUnOp,floor)

}; // end namespace FortCpp



#endif
