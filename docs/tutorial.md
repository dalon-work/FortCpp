# The Alloc class

The Alloc class is a class templated on the type of storage, and the rank of the array.
Declaring a 2D allocatable array of floats:

```c++
Alloc<float,2> a;  // currently not allocated

a.allocate(10,10); // now allocated to be a 10x10 array
a.deallocate();    // memory has been deallocated 
```

`Alloc` also provides a constructor which takes the size of the array, performing the 
two previous steps in one:

```c++
Alloc<float,2> a(10,10); // constructed and allocated
a.deallocate();          // memory deallocated
```
By default, the allocated memory is not initialized. 
The whole array can be set to a single value by assignment:

```c++
Alloc<float,2> a(10,10); 
a = 2.0;                 // all 100 elements of a now set to 2.0
```



### Column-Major vs. Row-Major

### Debug

By default, FortC++ is in Debug mode. This includes bounds-checking and whole-array operation size checking.
If an error does occur, FortC++ throws an ArrayException. An ArrayException will print out an error message,
and then exit the program. 

To turn debugging off, define the macro NDEBUG. Use this for release code, as debugging does incur a performance hit.

### Compiling and Optimization

This library uses features from the C++11 standard, so you have to use a C++11 compliant compiler. 
With G++, this means compiling with the -`std=c++11` option. This library also relies heavily on the ability
of the compiler to inline everything, or else performance goes to zilch. For G++, this means compiling with the `-O3` flag.

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

