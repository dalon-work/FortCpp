
# FortC++

FortC++ is a templated header library designed to add Fortran-style arrays into C++.
This is not the first or the best such offering. Here is a list of others:

* [ndarray](https://github.com/ndarray/ndarray)
* [Blitz++](http://blitz.sourceforge.net/)
* [Boost::MultiArray](http://www.boost.org/doc/libs/1_44_0/libs/multi_array/doc/index.html)
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

This library is not intended to be a linear algebra package. 
If you want one of those, I suggest Eigen. 
It's really awesome, and if they added higher than 2d arrays, I might just switch to Eigen.
I needed slicing, and whole-array operations, bounds and size checking, and a few nice bells and whistles. 
I also wanted some C++ practice. Haven't done that since high school.
I add to this library whenever I need something, but if you really want something, email me and let me know.
Or fork it and send me a pull request. 

I didn't like any of the above for a a few different reasons. 
1. Too much C++ fluff (complicated interface)
2. External Dependencies (read: Boost)
3. Not maintained
4. Features not available

I'm not a C++ guru, I'm a engineer who started with Fortran, MATLAB, and Python (NumPy), so don't rake me over the coals for not doing "proper" C++. Sometimes I feel like C++ takes itself way too seriously.

## How to use FortC++

FortC++ has up to 5d arrays. This number might go up later. 
The basic functionality can be had as follows:

```c++
#include "FortCpp.h"

using namespace FortCpp;

Array1d<double> a;    // Dynamic 1d array of type double
Array1d<int> b;       // Dynamic 1d array of type int
Array1d<float,10> c;  // Static 1d array (size 10) of type float
Array2d<float,3,3> d; // Static 2d array (size 9) of type float
Array2d<int> e;       // Dynamic 2d array of type int
Array2d<double> f;    // Dynamic 2d array of type double
                      // NOTE: STATIC ARRAYS DO NOT EXIST FOR DIMENSIONS HIGHER THAN 2
Array3d<int> g;       // Dynamic 3d array ot type int

a.allocate(10);       // allocate `a` to size 10
b.mold(a);            // allocate `b` to same size as `a`

e.mold(d);            // allocate `e` to same size of `d`
f.allocate(3,3);      // allocate `f` to size 3x3

b = 0;                // set all of `b` equal to zero
a = b;                // set `a` equal to `b`. This is a deep-copy operation. None of this shallow copy business

for(int i=0;i<a.size();i++) // loop through the size of array `a`
{   
  a[i] = i;                 // [] are linear indexers, accessing memory sequentially  
  b(i) = i-1;               // () are non-linear indexers, but for 1d arrays, are the same as []
}

a.deallocate();             // deallocate the memory allocated by the dynamic arrays
b.deallocate();

for(int i=0;i<d.size();i++) // this is accessing the 2d array linearly
{
  d[i] = i;
}

for(int j=0;j<e.d2();j++){    // this is accessing the 2d array using the indexing operations
  for(int i=0;i<e.d1();i++){  // Note: this is Column-Major, more below
    e(i,j) = i*j;
  }
}

f = d+e; // this is an element-wise addition operation, 
         // which through the magic of inlining, does not create temporaries.
         // In debug mode, this makes sure that both arrays have the same size.
         
g.map( function_returning_external_int_ptr() , 10,10,10); // This maps an external pointer to a 3d array, and 
                                                          // makes it of size 10x10x10

```

### Column-Major vs. Row-Major

FortC++ defaults to column-major. Why would I do such a thing when C++ is a row-major language?
Because I needed to interface with a column-major library. Row-major is an option however, and can be specified as:
```c++
Array1d<double,3,RowMajor> a;            // Static 1d array of size 3, RowMajor option.
Array1d<double,Unknown,RowMajor> b(100); // Dynamic 1d array, RowMajor option, allocated to a length of 100
```
However, row-major slicing is not yet implemented. 
I think I made a bad assumption when making the slicing stuff,
and haven't needed it, so I haven't fixed it yet.

### Debug

By default, FortC++ is in Debug mode. This includes bounds-checking and whole-array operation size checking.
If an error does occur, FortC++ throws an ArrayException. An ArrayException will print out an error message,
and then exit the program. 

If the macro FortCpp_EXCEPTION_PAUSE is defined, then before exiting, 
the program will pause and wait for a keypress. 

To turn debugging off, define the macro NDEBUG. Use this for release code, as debugging does incur a performance hit.

### Compiling and Optimization

This library uses features from the C++11 standard, so you have to use a C++11 compliant compiler. 
With G++, this means compiling with the -`std=c++11` option. This library also relies heavily on the ability
of the compiler to inline everything, or else performance goes to zilch. For G++, this means compiling with the `-O3` flag.

I've tried several ways to disable aliasing checks in whole-array operations, and have succeeded (I think)
with the G++ compiler through the use of `#pragma GCC ivdep`. The important point here is: This library assumes
no aliasing (or it will, when I can figure out a better way to do it). Don't do it.

## Slicing

A slice can be created and used as follows:
```c++
Array1d<int> a(100); // Dynamic 1d array of size 100

a = 1; // set all of a equal to 1

auto b = a(S(0,100,2)); // This creates a StridedArray1d, which points to every other element of array `a`
                        // and use the `auto` keyword, these templates are super-annoying.
                        // NumPy calls this a `view`.
```
The function `S` is a helper function that generates a `Slice` object. I know, I know, I've imported a nasty
capital `S` into the global namespace. How many times do you actually use that particular name?
I might have this function name be a `#define`, so the name can be changed to something you're okay with.

StridedArrays know their size and dimensions, and can be accessed like a normal array. 
Continuing with the above example:
```c++
b = 0; // this sets every other element of `a` equal to 0

for(int i=0;i<b.size();i++) // set every other element of `a` equal to i
{
  b[i] = i;
}
```
These can also be used in whole-array operations like:
```c++
Array3d<double> a(100,100,100);
Array2d<int> b;
auto c = a( S(0,50) ,2, S(10,30) );
b.mold( c ); // This allocates b to be equal to the 
             // same size as the 2d slice of the 3d array `a`.
                                   
b = 2.0*c;
```
So, what is the function `S`? `S` is:
```c++
S(b,e);    
S(b,e,s);
S<L>(b);
S<L,s>(b);
```
where the template parameter `L` is the length of the slice, and `b-e=L , stepping by s`.
Due to the nature of C++, the slice does not include the last element specified (`e`).
For example, slice `S(0,10)`, is length 10, starting at element 0, which means it actually runs from 0 to 9.
Slices can also be saved and used later with the `auto` keyword:
```c++
auto a = S(1,4);
Array3d<double> b(7,9,12);
b( a, 10, 2 ) = 2.0;
```
A `StridedArray` knows when it is contiguous, allowing the compiler to optimize those operations.

## Copy operations

FortC++ uses default copy/move constructors. I know, I'm a horrible C++ programmer, but right now, I have
zero need for it, and it was too confusing trying to account for all possible situations.
In other words: DON'T COPY ANYTHING, IF YOU DO YOU WILL GET DOUBLE FREES AND OTHER MEMORY PROBLEMS.
All FortC++ arrays must be passed by reference. The one exception is returning an array from a function.
Such as:
```c++
const Array1d<double> func()
{
  Array1d<double> a(10);
  a = 1.0;
  return a;
};
...
a += func();
```
This will invoke the assignment operator, not the copy constructor. 
Someone tell me if I'm wrong, but I've never seen a problem with this.

## Array Math

There are several functions defined that take in arrays and perform element wise operations on them.
This list will grow as I need something from it. Elemental functions actually do not evaluate,
they return an expression that can be used in other operations. This prevents temporary arrays
from being formed. More below.

* sqrt - elemental
* sin  - elemental
* cos  - elemental
* addition (*) - elemental
* subtraction (-) - elemental
* multiplication (-) - elemental
* division (/) - elemental
* Less Than (<) - elemental
* Equality (==) - elemental
* Non-Equality (!=) - elemental
* sum - sums the array given it
* min - finds the minimum in the array
* max - finds the maximum in the array
* any - returns true if any value in the array is true
* all - returns true if all values in the array are true
* isnan - returns true if a nan is found in the array

FortC++ does not deal well with mixed-mode arithmatic. 
If you get a huge bundle of errors talking about `BinaryOp`, 
check that you don't have mixed-mode arithmetic somewhere.

To find a dot product of two arrays can be found as:
```c++
double dot = sum( a*b );
```
Once the compiler is finished inlining everything, this becomes:
```c++
double dot = 0.0;
for(int i=0;i<a.size();i++){
  dot += a[i]*b[i];
}
```
For checking if all values of an array are negative:
```c++
if( all( a<0 ) )
```














