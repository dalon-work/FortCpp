
#include "Core/Macros.h"

#ifndef NDEBUG
#include "Core/ArrayException.h"
#endif

#include <iostream>
#include <cmath>

#include "Core/ForwardDeclarations.h"
#include "Core/OpUtil.h"
#include "Core/BinaryOp.h"
#include "Core/ConstantOp.h"
#include "Core/UnaryOp.h"
#include "Core/ArrayBase.h"
#include "Core/Dimensions.h"
#include "Core/Strides.h"
#include "Core/Accessor.h"
#include "Core/Storage.h"
#include "Core/Slice.h"
#include "Core/StrideCalc.h"
#include "Core/ArrayBase1d.h"
#include "Core/StridedArray1d.h"
#include "Core/Array1d.h"
#include "Core/ArrayMath.h"

/**
 * @mainpage FortCpp Array Classes
 *
 * @section intro Introduction
 *
 * FortCpp Array classes are an attempt to get Fortran / Python type
 * arrays in C++. 
 *
 * You must specify the main directory in the include path, and can then
 * include the necessary Arrays with 
 * @code{.cpp}
 * #include "ArrayNd.h"
 * @endcode
 * where N is the dimension required. All N dimension Arrays depend on the N-1 dimension Array,
 * and are added automatically.
 *
 * All Array classes are contained in a FortCpp namespace, and therefore must be 
 * referenced like this:
 *
 * @code{.cpp}
 * FortCpp::Array1d<double> a;
 * @endcode
 * or, you can  put the following in a header file
 * @code{.cpp}
 * using namespace FortCpp;
 * @endcode
 * and then use the Array classes as follows
 * @code{.cpp}
 * Array1d<double> a;
 * @endcode
 *
 * Basic usage of the arrays is as follows:
 * @code{.cpp}
 * FortCpp::ArrayNd<template type> {variable name}
 * @endcode
 * The above code creates a dynamic N-dimensional Array. The size is not known, and must be
 * allocated later as follows:
 * @code{.cpp}
 * a.allocate(d1,d2,...);
 * @endcode
 * This will allocate memory for the array, which can then be accessed by
 * @code{.cpp}
 * a(i,j,...);
 * @endcode
 * All arrays also include a _linear indexer_ , which will access the memory in a
 * contiguous manner. 
 * @code{.cpp}
 * a[i]
 * @endcode
 * 1d and 2d arrays can be given dimensions at compile-time, which allocates static
 * memory instead of heap memory. This can lead to performance increases.
 * @code{.cpp}
 * Array1d<double,3> a;
 * Array2d<double,4,4> b;
 * @endcode
 * This creates a 1d static array of size 3, and a 2d static array of size 4x4, or 16 elements.
 * Attempts to change these will result in compilation errors.
 * 
 * This library takes advantage of several C++11 features, and thus must be compiled
 * with a C++11 compliant compiler. For g++, this means compiling with the flag
 * @code
 * -std=c++11
 * @endcode
 *
 * Currently, a single g++ extension command called
 * @code{.cpp}
 * __restrict__
 * @endcode
 * is used in the Storage class. This command tells g++ to not perform aliasing checks,
 * which can bring performance advantages. This command is implemented by several other compilers.
 *
 * Currently, this library defaults to Column-Major Ordering. 
 * Row-Major Ordering is not yet implemented. But if it were, you could select it with
 * the following
 * @code{.cpp}
 * FortCpp::Array2d<double,FortCpp::Unknown,FortCpp::Unknown,FortCpp::RowMajor> b;
 *
 * or
 *
 * using namespace FortCpp;
 * Array2d<double,Unknown,Unknown,RowMajor> b;
 * @endcode
 * A preprocessor flag is in the works to select RowMajor by default.
 *
 * These classes implement element-wise array operations, for example:
 * @code{.cpp}
 * Array2d<double> a(3,3),b(3,3),c(3,3);
 * b = 2.0;
 * c = 3.0;
 * a = b+c;
 * @endcode
 * will assign a value of 5 to the entire `a` array.
 * The following math functions are implemented:
 * @code{.cpp}
 * cout << a; // prints the array to stdout
 * b = sqrt(a);
 * double m = max(a);
 * double m = min(a);
 * double m = max(a-b);
 * b = cos(a);
 * b = sin(a);
 * @endcode
 *
 * This library also provides runtime checks for index bounds errors
 * and size errors on whole-array operations. It is recommended for release code
 * to turn these off by specifying
 * @code{.cpp}
 * #define NDEBUG
 * @endcode
 * or by compiling with the `-DNDEBUG` flag with g++.
 *
 * The following is a list of available functions:
 * @code{.cpp}
 * a.allocate(...); // allocate to given dimensions
 * a.deallocate();  // deallocate an array
 * a.mold(b);       // allocate a to dimensions of b
 * a.map(*,...);    // assign the array to pointer *, with given dimensions
 * a.d1();          // return size of dimension 1
 * a.s1();          // return stride of dimension 1
 * a.allocated();   // returns true if allocated
 * a.associated();  // returns true if the internal pointer is in use
 * @endcode
 *
 *
 * \section slice Slicing
 *
 * Arrays can also be sliced, using the following helper function:
 * @code{.cpp}
 * S<L,S>(B,E,S);
 * @endcode
 * Don't get distracted by all the 'S's. 
 * - L -- is the length of the slice, must be known at compile time, default Unknown;
 * - S -- is the stride of the slice, default 1;
 * - B -- Beginning index of the dimension
 * - E -- Ending index of the slice. This element is not actually included in the slice.
 *        For example, a Slice `S(0,4)` starts at element 0 and goes to element 3. The length 
 *        of the Slice is E-B=L, or 4.
 * 
 * As a convenience, an empty function call defaults to the entire slice. 
 * @code{.cpp}
 * Array2d<int> b;
 * b.allocate(4,6);
 * for(int i=0;i<b.size();b++){
 *   b[i] = i;
 * }
 * cout << b(S(),1) << endl;
 * @endcode
 * This prints the entire column of length 4 that corresponds to the j-th index 1.
 * Other valid forms are:
 * @code{.cpp}
 * S<L>(B);
 * S<L,S>(B);
 * S(B,E);
 * S(B,E,S);
 * @endcode
 *
 * A `view` of an array can be saved and accessed later like:
 * @code{.cpp}
 * auto b = a(S(),0);
 * cout << b << endl;
 * @endcode
 * 
 */


