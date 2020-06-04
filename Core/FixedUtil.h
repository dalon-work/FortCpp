#ifndef FortCpp_FIXEDUTIL_H
#define FortCpp_FIXEDUTIL_H

namespace FortCpp
{

namespace internal
{

/****** FIXED_GET_DIM *************/

template<Index D,Index r,Index d,Index... dims> struct fixed_get_dim;

template<Index D,Index d,Index... dims>
struct fixed_get_dim<D,D,d,dims...> {
	static const Index value = d;
};

template<Index D,Index r,Index d,Index... dims>
struct fixed_get_dim {
	static const Index value = fixed_get_dim<D+1,r,dims...>::value;
};

/***** FIXED_COMPUTE_OFFSET ******/

template<Index S,Index d>
Index fixed_compute_offset(Index i)
{
	return i*S;
}

template<Index S,Index d,Index... dims,typename... indices>
Index fixed_compute_offset(Index i,indices... idx)
{
	return i*S+fixed_compute_offset<S* d,dims...>(idx...);
}

// template<Index S,Index... dims> struct fixed_compute_offset;
//
// template<Index S,Index d>
// struct fixed_compute_offset
// {
//    static Index exec(Index i)
//    {
//       return i*S;
//    }
// };
//
// template<Index S,Index d,Index... dims>
// struct fixed_compute_offset
// {
//    template<typename... indices>
//    static Index exec(Index i,indices... idx)
//    {
//       return i*S+fixed_compute_offset<S*D,dims...>::exec(idx...);
//    }
// };

/***** FIXED_PRODUCT ****/

template<Index... dim> struct fixed_product;

template<Index d>
struct fixed_product<d> {
	static const Index value = d;
};

template<Index d,Index... dim>
struct fixed_product<d,dim...> {
	static const Index value = d * fixed_product<dim...>::value;
};


}; // end namespace internal

}; // end namespace FortCpp

#endif
