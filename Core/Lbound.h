#ifndef FortCpp_LBOUND
#define FortCpp_LBOUND

template<typename Derived>
class Lbound
{

private:

	typedef internal::traits<Derived>::Scalar T;
	static const int Rank = internal::traits<Derived>::Rank;
	static const int Order = internal::traits<Derived>::Order;
	static const int Stride = internal::traits<Derived>::Stride;
	static const int StorageType = internal::traits<Derived>::StorageType;
	static const int Align = internal::traits<Derived>::Align;

	Derived& _A;

	const std::array<int,Rank> _lb;

public:

	Lbound() = delete;
	Lbound(const Lbound& B) = delete;
	Lbound(Lbound&& B) = default;

	template<typename... indices>
	Lbound(Derived& A,indices... idx) : _A(A), _lb({{idx...}}) {
		static_assert(sizeof...(idx) == Rank,
		"NUMBER OF INDICES PASSED TO LBOUND DOES NOT MATCH RANK");
	}

	/*************************************************/

	template<typename... indices>
	const T& operator () (indices... idx) const {

		return _storage[
		           internal::offset<Order,Stride,Rank>::exec(_str,idx...)
		       ];
	}

	template<typename... indices>
	T& operator () (indices... idx) {
#ifndef NDEBUG
		internal::debug::is_negative<0>(idx...);
		internal::debug::in_bounds<0,Rank>(_dim,idx...);
#endif
		return _storage[
		           internal::offset<Order,Stride,Rank>::exec(_str,idx...)
		       ];
	}

	/*************************************************/

	const T& operator [] (int i) const {
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[
		           internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
		       ];
	}
	T& operator [] (int i) {
#ifndef NDEBUG
		internal::debug::in_size(i,size());
#endif
		return _storage[
		           internal::linear_index<Order,Stride,Rank>::exec(_dim,_str,i)
		       ];
	}

	/***********************************************/



#endif
