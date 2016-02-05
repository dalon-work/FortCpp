#ifndef FortCpp_ARRAY2D_H
#define FortCpp_ARRAY2D_H

namespace FortCpp
{

namespace internal
{

template<typename _T, int _D1, int _D2, int _Options>
struct traits<Array2d<_T,_D1,_D2,_Options> > {
	typedef _T Scalar;
	enum {
	    Size = (_D1 == Unknown) || (_D2 == Unknown) ? Unknown : _D1*_D2,
	    D1   = _D1,
	    D2   = _D2,
	    StorageType = (Size == Unknown) ? Pointer : Static,
	    Dim  = 2,
	    Order = _Options & MajorOrderBit,
	    Align  = _Options & AlignedBit,
	    Stride = Contig,
	    Options = _Options
	};

}; // end struct traits

}; // end namespace internal

template<typename T,int D1,int D2, int Options>
class Array2d : public ArrayBase2d<Array2d<T,D1,D2,Options> >
{

	typedef class Array2d<T,D1,D2,Options> Derived;
	typedef ArrayBase2d<Derived> Base;
	typedef internal::Storage<T,
	        internal::traits<Derived>::StorageType,
	        internal::traits<Derived>::Size,
	        internal::traits<Derived>::Align> StorageDerived;
	typedef internal::Accessor<Derived,
	        internal::traits<Derived>::Dim,
	        internal::traits<Derived>::Order,
	        internal::traits<Derived>::Stride> AccessorDerived;

private:
	StorageDerived _storage;
	AccessorDerived _accessor;


public:
	Array2d(const Derived& B)=default;
	Array2d(Derived&& B)=default;
	Array2d()=default;
	Array2d(const int d1,
	        const int d2) {
		_storage.allocate(d1*d2);
		_accessor.set_dim(d1,d2);
	}

	~Array2d() {
		_storage.deallocate();
	}

	/***********************************************/

	/**
	 * Assignment operator for a single value, delegates to base
	 */
	inline const T& operator = (const T& B) {
		return Base::operator = (B);
	}

	/**
	 * Assignment operator for an entire array, delegates to base
	 */
	template<typename OtherDerived>
	inline const Derived& operator = (const ArrayBase<OtherDerived>& B) {
		return Base::operator = (B);
	}
	inline const Derived& operator = (const Derived& B) {
		return Base::operator = (B);
	}

	/************************************************/

	void allocate(const int d1,
	              const int d2) {
		FortCpp_NOT_STATIC_FUNCTION_ASSERT
		_storage.allocate(d1*d2);
		_accessor.set_dim(d1,d2);
	}

	/**
	 * Maps a pointer into the array. The size of the
	 * pointer array is given by i
	 */
	void map(T* a, const int d1,
	         const int d2) {
		FortCpp_NOT_STATIC_FUNCTION_ASSERT
		_storage.map(a,d1*d2);
		_accessor.set_dim(d1,d2);
	}

	template<typename OtherDerived>
	void mold(const ArrayBase2d<OtherDerived>& B) {
		FortCpp_NOT_STATIC_FUNCTION_ASSERT
		_storage.allocate(B.size());
		_accessor.set_dim(B.d1(),B.d2());
	}

	/**
	 * Deallocates memory, or disassociates the array from the
	 * memory it points to if the array was not allocated
	 */
	void deallocate() {
		FortCpp_NOT_STATIC_FUNCTION_ASSERT
		_storage.deallocate();
	}

	/***************************************/

	/**
	 * indexing (parenthesis operator), two of them (for const correctness)
	 * In debug mode, if a dimension is out-of-bounds, a fatal
	 * exception is thrown.
	 */
	const T& operator () (const int i, const int j) const {
		FortCpp_BOUNDS_CHECK(i,1)
		FortCpp_BOUNDS_CHECK(j,2)
		return _storage[_accessor(i,j)];
	}
	T& operator () (const int i, const int j) {
		FortCpp_BOUNDS_CHECK(i,1)
		FortCpp_BOUNDS_CHECK(j,2)
		return _storage[_accessor(i,j)];
	}

	/*************************************************/

	/**
	 * Linear indexer of the result. This varies from 0 to this->_size-1.
	 * Throws a fatal exception in debug mode if i is outside these limits.
	 */
	const T& operator [] (const int i) const {
		FortCpp_LINEAR_SIZE_CHECK(i)
		return _storage[_accessor[i]];
	}
	T& operator [] (const int& i) {
		FortCpp_LINEAR_SIZE_CHECK(i)
		return _storage[_accessor[i]];
	}

	/***********************************************/

	template< FortCpp_SLICE_TEMPLATE(1) >
	inline StridedArray1d<Derived,FortCpp_SLICE(1) > operator () (const FortCpp_SLICE_0(1) &S1,
	                                                              const int j) {
		typedef StridedArray1d<Derived,FortCpp_SLICE(1)> ResultType;
		enum {
		    Stride1 = internal::traits<ResultType>::S1
		};
		const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
		               const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
		                              FortCpp_SLICE_CHECK(1)
		                              FortCpp_BOUNDS_CHECK(j,2)
		                              ResultType S(&_storage[_accessor(S1.b(),j)],l1,s1);
		return S;
	}

	template<FortCpp_SLICE_TEMPLATE(2)>
	inline StridedArray1d<Derived,FortCpp_SLICE(2)> operator () (const int i,
	                                                             const FortCpp_SLICE_0(2) &S2) {
		typedef StridedArray1d<Derived,FortCpp_SLICE(2)> ResultType;
		enum {
		    Stride1 = internal::traits<ResultType>::S1
		};
		const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
		               const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(1,2)
		                              FortCpp_BOUNDS_CHECK(i,1)
		                              FortCpp_SLICE_CHECK(2)
		                              ResultType S(&_storage[_accessor(i,S2.b())],l2,s2);
		return S;
	}

	template<FortCpp_SLICE_TEMPLATE(1), FortCpp_SLICE_TEMPLATE(2)>
	inline StridedArray2d<Derived,FortCpp_SLICE(1),
	       FortCpp_SLICE(2) >operator () (const FortCpp_SLICE_0(1) &S1,
	const FortCpp_SLICE_0(2) &S2) {
		typedef StridedArray2d<Derived,FortCpp_SLICE(1),FortCpp_SLICE(2)> ResultType;
		enum {
		    Stride1 = internal::traits<ResultType>::S1,
		    Stride2 = internal::traits<ResultType>::S2
		};
		const int l1 = FortCpp_SLICE_RUNTIME_LENGTH(1)
		               const int l2 = FortCpp_SLICE_RUNTIME_LENGTH(2)
		                              const int s1 = FortCpp_SLICE_RUNTIME_STRIDE(1,1)
		                                             const int s2 = FortCpp_SLICE_RUNTIME_STRIDE(2,2)
		                                                            FortCpp_SLICE_CHECK(1)
		                                                            FortCpp_SLICE_CHECK(2)
		                                                            ResultType S(&_storage[_accessor(S1.b(),S2.b())],l1,l2,s1,s2);
		return S;
	}

	/*******************************************************/

	const int d1() const { return _accessor.d1(); }
	const int d2() const { return _accessor.d2(); }
	inline const int size() const { return _storage.size(); }
	inline const bool allocated() const { return _storage.allocated(); }
	inline const bool associated() const { return _storage.associated(); }
	inline T*  data() { return _storage.data(); }

};

}; // end namespace FortCpp

#endif
