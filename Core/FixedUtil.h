#ifndef FortCpp_FIXEDUTIL_H
#define FortCpp_FIXEDUTIL_H

namespace FortCpp
{

namespace internal
{

/***** fixed_compute_strides ******/

// template<int k,int l,int... dims> struct fixed_compute_strides;
//
// template<int... dims>
// struct fixed_compute_strides<1,0,dims...>
// {
//    static const int value = 1;
// };
//
// template<int k,int d,int... dims>
// struct fixed_compute_strides<k,k-1,d,dims...>
// {
//    static const int value = d;
// };
//
// template<int k,int l,int d,int... dims>
// struct fixed_compute_strides<k,l,d,dims...>
// {
//    static const int value = d*fixed_compute_strides<k,l+1,dims...>::value;
// };

/****** FIXED_GET_DIM *************/

template<int D,int r,int d,int... dims> struct fixed_get_dim;

template<int D,int d,int... dims>
struct fixed_get_dim<D,D,d,dims...> {
	static const int value = d;
};

template<int D,int r,int d,int... dims>
struct fixed_get_dim {
	static const int value = fixed_get_dim<D+1,r,dims...>::value;
};

/***** FIXED_COMPUTE_OFFSET ******/

template<int S,int d>
unsigned fixed_compute_offset(int i)
{
	return i*S;
}

template<int S,int d,int... dims,typename... indices>
unsigned fixed_compute_offset(int i,indices... idx)
{
	return i*S+fixed_compute_offset<S* d,dims...>(idx...);
}

// template<int S,int... dims> struct fixed_compute_offset;
//
// template<int S,int d>
// struct fixed_compute_offset
// {
//    static unsigned exec(int i)
//    {
//       return i*S;
//    }
// };
//
// template<int S,int d,int... dims>
// struct fixed_compute_offset
// {
//    template<typename... indices>
//    static unsigned exec(int i,indices... idx)
//    {
//       return i*S+fixed_compute_offset<S*D,dims...>::exec(idx...);
//    }
// };

/***** FIXED_PRODUCT ****/

template<int... dim> struct fixed_product;

template<int d>
struct fixed_product<d> {
	static const unsigned value = d;
};

template<int d,int... dim>
struct fixed_product<d,dim...> {
	static const unsigned value = d* fixed_product<dim...>::value;
};


}; // end namespace internal

}; // end namespace FortCpp

#endif
