#ifndef FortCpp_ARRAYBASE1D_H
#define FortCpp_ARRAYBASE1D_H

namespace FortCpp
{

namespace internal
{

template<typename Derived>
struct traits<ArrayBase1d<Derived> > {
	typedef typename traits<Derived>::Scalar Scalar;
	enum {
	    Size   = traits<Derived>::Size,
	    D1     = traits<Derived>::D1,
	    StorageType = traits<Derived>::StorageType,
	    Dim    = 1,
	    Order  = traits<Derived>::Order,
	    Align  = traits<Derived>::Align,
	    Stride = traits<Derived>::Stride,
	    Options = traits<Derived>::Options
	};

};

}; // end namespace internal

template <typename Derived>
class ArrayBase1d : public ArrayBase< Derived >
{

	typedef typename internal::traits<Derived>::Scalar T;
	typedef ArrayBase1d<Derived> ThisDerived;
	typedef ArrayBase<Derived> Base;

private:

protected:
	ArrayBase1d(const ThisDerived&)=default;
	ArrayBase1d(ThisDerived&&)=default;
	ArrayBase1d()=default;
	~ArrayBase1d()=default;

public:

	inline const T& operator = (const T& B) {
		return Base::operator=(B);
	}

	template<typename OtherDerived>
	inline const Derived& operator = (const ArrayBase<OtherDerived>& B) {
		return Base::operator=(B);
	}
	inline const Derived& operator = (const ThisDerived& B) {
		return Base::operator=(B);
	}

	inline const T& operator += (const T& B) {
		return Base::operator+=(B);
	}

	template<typename OtherDerived>
	inline const Derived& operator += (const ArrayBase<OtherDerived>& B) {
		return Base::operator+=(B);
	}

	inline const T& operator -= (const T& B) {
		return Base::operator-=(B);
	}

	template<typename OtherDerived>
	inline const Derived& operator -= (const ArrayBase<OtherDerived>& B) {
		return Base::operator-=(B);
	}

	inline const int size() const { return derived().size(); }
	inline const int d1() const { return derived().d1(); }
	inline const int s1() const { return derived().s1(); }
	inline const bool allocated() const { return derived().allocated(); }
	inline const bool associated() const { return derived().associated(); }

	inline const T& operator () (const int& i) const {
		return derived()(i);
	}

	inline T& operator () (const int& i) {
		return derived()(i);
	}
	inline const T& operator [] (const int i) const {
		return derived()[i];
	}
	inline T& operator [] (const int i) {
		return derived()[i];
	}

	void cshift(const int num=1) {
		if (num > 0) {
			for (int n=0; n<num; n++) {
				T keep = derived()[d1()-1];
				for (int i=d1()-1; i>1; i--) {
					derived()[i] = derived()[i-1];
				}
				derived()[0] = keep;
			}
		}
		else if (num < 0) {
			for (int n=0; n>num; n--) {
				T keep = derived()[0];
				for (int i=1; i<d1(); i++) {
					derived()[i-1] = derived()[i];
				}
				derived()[d1()-1] = keep;
			}
		}
	}

	inline const Derived& derived() const { return *static_cast<const Derived*>(this); }
	inline       Derived& derived() { return *static_cast<Derived*>(this); }

	// /**
	//  * output operator
	//  */
	// friend std::ostream& operator << (std::ostream& os, const ArrayBase1d<Derived> &A){
	//   for(int i=0;i<A.size();i++){
	//     os << A.derived()[i] << ' ';
	//   }
	//   return os;
	// }
};

/**********************************************************************/

}; // end namespace FortCpp



#endif
