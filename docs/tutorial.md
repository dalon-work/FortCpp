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

Elements in the array can be accessed using the `()` and `[]` operators. 
`()` provides multi-dimensional access, while `[]` accesses elements in
a linear fashion. 

```c++
Alloc<int,3> a(10,10,10);

// This loop sets the elements of a in
// a linear fashion.
for(int i=0;i<a.size();i++){
  a[i] = i;
}

// This prints out `true`
cout <<  a(5,0,0) == a[6] << endl;
```

Contrary to Fortran, (which starts at 1 and goes until N), 
indexing starts at 0 and runs until N-1, for a more natural fit with C++.
However, in typical Fortran fashion, FortCpp defaults to using Column-Major
Ordering. Row-Major Ordering is fully supported, however, and can be specified
in the `Options` template parameter.

```c++
Alloc<bool,3,RowMajor> a(10,10,10);
```

## Views

Views of Alloc arrays can be created with the `view` member function.
`view` takes as its arguments a `Slice` and integer indices. 
A `Slice` specifies a range of a given dimension, while integer indices
specify a location in the given dimension.
A `Slice` is generated using the provided `Slice` function:

```c++
FullSlice S     = Slice();
ContigSlice cS  = Slice(2,7);
StridedSlice sS = Slice(0,8,2);
```

There are three different types of `Slices`: `FullSlice`, which
specifies that the entire dimension is to be used in a view.
`ContigSlice` specifies that the dimension from the starting index
(2 in the above example), up to, but not including the final index
(6 in the above example) is to be used in the view. A `StridedSlice`
adds a third argument, which is the step of the slice. In the above
example, `sS` specifies a slice starting at 0, and moving up to 7 
in steps of 2, or [0,2,4,6]. As a convenience, FortCpp provides
two special numbers, `BEG` and `END`. These numbers only have meaning
to a Slice, and refer to the beginning and end of a dimension, 
respectively.

In practice, it is easier to use the `auto` keyword to catch the result 
from the `Slice()` function. Slices can be used as:

```c++
Alloc<int,4> a(10,10,10,10);
auto S = Slice();

Alloc<int,2> b = a.view(S,S,4,6);
```

A View in this case is not a separate class, but generates another
`Alloc` object. This object does not own the data of the original array,
but is mapped to the specific region of the original data. Data of the 
original array can be read and modified as if it was the original array.

```c++
auto S = Slice(BEG,END,2); // view the even indices along a dimension
Alloc<int,1> a(100);
Alloc<int,1> b = a(S);

a = 1;
b = 0;

cout << a << endl; 
```

The above example will output an alternating sequence of 0's and 1's. 


### Debug

By default, FortC++ is in Debug mode. This includes bounds-checking and whole-array operation size checking.
If an error does occur, FortC++ throws an ArrayException. An ArrayException will print out an error message,
and then exit the program. 

To turn debugging off, define the macro NDEBUG. Use this for release code, as debugging does incur a performance hit.

### Compiling and Optimization

This library uses features from the C++11 standard, so you have to use a C++11 compliant compiler. 
With G++, this means compiling with the -`std=c++11` option. This library also relies heavily on the ability
of the compiler to inline everything, or else performance goes to zilch. For G++, this means compiling with the `-O3` flag.

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

