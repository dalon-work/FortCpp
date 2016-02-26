
# FortC++

FortC++ is a templated header library designed to add Fortran-style arrays into C++.
This is not the first or the best such offering. Here is a list of others:

* [ndarray](https://github.com/ndarray/ndarray)
* [Blitz++](http://blitz.sourceforge.net/)
* [Boost::MultiArray](http://www.boost.org/doc/libs/1_44_0/libs/multi_array/doc/index.html)
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

This library is not intended to be a linear algebra package. It is intended to create
user-friendly arrays which provide whole-array operations, slicing, views, multi-d indexing,
and bounds checking. Whole-array operations do not generate temporaries (with appropriate inlining),
keeping the overhead of using the arrays down.

FortC++ supports any rank of array using variadic templates. Column-Major ordering of the data
is the default, but Row-Major ordering is fully supported. 
There are two main components to FortCpp:

```c++
FortCpp::Alloc< Type, Rank, Options >
FortCpp::Fixed< Type, Dims... >
```

The `Alloc` class specifies an allocatable array, whose dimensions are only known at runtime.
This is the main workhorse of FortC++. The `Fixed` class specifies a static array whose size
must be determined at compile time. These two classes are fully compatible with each other.

---

### How to use FortC++

Because FortC++ is a header-file only library, using the library is as straightforward as
placing the library in your project's directory and including `FortCpp.h`.

For a more detailed introduction to FortC++, see the [tutorial](docs/tutorial.md).















