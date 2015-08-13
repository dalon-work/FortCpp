#ifndef FortCpp_sLICE_h
#define FortCpp_sLICE_h
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
template<int _type,int _dim,int _l,int _s>
struct traits<Slice<_type,_dim,_l,_s> >
{
  enum{
    Type = _type,
    Dim  = _dim,
    L    = _l,
    S    = _s
  };
};

/** Slice class **/
template<int _type,int _dim,int _l,int _s> 
class Slice
{
  private:
  const int _be;
  const SliceValue<_l> _le;
  const SliceValue<_s> _se;
  public:
  inline explicit Slice(const int B, const int E, const int S) : _be(B),_le(E-B),_se(S) {}
  inline const int b() const { return _be; }
  inline const int l() const { return _le.get(); }
  inline const int s() const { return _se.get(); }
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

// A full slice with stride _s
// template<int _s>
// inline static internal::Slice<internal::Full,0,internal::Unknown,_s> S() 
// {
//   return internal::Slice<internal::Full,0,internal::Unknown,_s>(0,0,0);
// }

// A contiguous slice of length _l
template<int _l> 
inline static internal::Slice<internal::Contig,0,_l> S(const int B)  
{
  return internal::Slice<internal::Contig,0,_l>(B,0,0);
}

// An unknown slice of length _l and stride _s
template<int _l,int _s>
inline static internal::Slice<internal::Unknown,0,_l,_s> S(const int B) 
{
  return internal::Slice<internal::Unknown,0,_l,_s>(B,0,0);
}

// a contiguous slice of unknown length
inline static internal::Slice<internal::Contig,0,internal::Unknown> S(const int B,const int E) 
{
  return internal::Slice<internal::Contig,0,internal::Unknown>(B,E,0);
}

// an unknown slice of unknown length and stride _s
template<int _s>
inline static internal::Slice<internal::Unknown,0,internal::Unknown,_s> S(const int B,const int E) 
{
  return internal::Slice<internal::Unknown,0,internal::Unknown,_s>(B,E,0);
}

// an unknown slice of unknown length and stride
inline static internal::Slice<internal::Unknown,0,internal::Unknown,internal::Unknown> S(const int &B,const int &E,const int &S) 
{
  return internal::Slice<internal::Unknown,0,internal::Unknown,internal::Unknown>(B,E,S);
}

/*****************************************************/

}; // end namespace FortCpp






#endif
