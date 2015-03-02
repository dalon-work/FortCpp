#ifndef FortCpp_SLICE_H
#define FortCpp_SLICE_H
#include "ForwardDeclarations.h"

namespace FortCpp
{

namespace internal
{


/** Class SliceValue **/
template<>
class SliceValue<Unknown>
{
  private:
  const int val;
  public:
  inline explicit SliceValue(const int v) : val(v) {}
  inline int get() const { return val; }
};

template<int Value>
class SliceValue
{
  public:
  inline explicit SliceValue(const int v) {}
  static inline const int get() { return Value; }
};

/** Traits Struct **/
template<int _Type,int _Dim,int _L,int _S>
struct traits<Slice<_Type,_Dim,_L,_S> >
{
  enum{
    Type = _Type,
    Dim  = _Dim,
    L    = _L,
    S    = _S
  };
};

/** Slice class **/
template<int _Type,int _Dim,int _L,int _S> 
class Slice
{
  private:
  const int _b;
  const SliceValue<_L> _l;
  const SliceValue<_S> _s;
  public:
  inline explicit Slice(const int B, const int E, const int S) : _b(B),_l(E-B),_s(S) {}
  inline const int b() const { return _b; }
  inline const int l() const { return _l.get(); }
  inline const int s() const { return _s.get(); }
};


}; // end namespace internal

/******************************************************/

/** 
 * These overloaded slicing functions hide the complexity
 * of using the Slicing structs from the user. It does, 
 * however take over the letter S for the global namespace
 *
 * Slice<Full,Dim,L,S> S() -- Returns a full slice of the dimension,
 *                            which indicates that the entire dimension 
 *                            should be used.
 * Slice<Contig,Dim,L,S> S(B,E) -- A Contiguous Slice is a slice of a dimension,
 *                                 in which S == 1. The length could or could not 
 *                                 be specified.
 * Slice<Unknown,Dim,L,S> S(B,E,S) -- An Unknown Slice is one where all conditions are not
 *                                    specified. The length and stride could or could not
 *                                    be specified.
 *
 * S()  -- Slice the whole dimension
 * S(B) -- The length, and possibly the stride, is known
 * S(B,E) -- The stride == 1, Length is computed at runtime
 * S(B,E,S) -- Length and Stride are all Unknown, everything computed at runtime
 */

 // A full slice
inline static internal::Slice<internal::Full> S()
{
  return internal::Slice<internal::Full>(0,0,0);
}

// A full slice with stride _S
// template<int _S>
// inline static internal::Slice<internal::Full,0,internal::Unknown,_S> S() 
// {
//   return internal::Slice<internal::Full,0,internal::Unknown,_S>(0,0,0);
// }

// A contiguous slice of length _L
template<int _L> 
inline static internal::Slice<internal::Contig,0,_L> S(const int B)  
{
  return internal::Slice<internal::Contig,0,_L>(B,0,0);
}

// An unknown slice of length _L and stride _S
template<int _L,int _S>
inline static internal::Slice<internal::Unknown,0,_L,_S> S(const int B) 
{
  return internal::Slice<internal::Unknown,0,_L,_S>(B,0,0);
}

// a contiguous slice of unknown length
inline static internal::Slice<internal::Contig,0,internal::Unknown> S(const int B,const int E) 
{
  return internal::Slice<internal::Contig,0,internal::Unknown>(B,E,0);
}

// an unknown slice of unknown length and stride _S
template<int _S>
inline static internal::Slice<internal::Unknown,0,internal::Unknown,_S> S(const int B,const int E) 
{
  return internal::Slice<internal::Unknown,0,internal::Unknown,_S>(B,E,0);
}

// an unknown slice of unknown length and stride
inline static internal::Slice<internal::Unknown,0,internal::Unknown,internal::Unknown> S(const int &B,const int &E,const int &S) 
{
  return internal::Slice<internal::Unknown,0,internal::Unknown,internal::Unknown>(B,E,S);
}

/*****************************************************/

}; // end namespace FortCpp






#endif
