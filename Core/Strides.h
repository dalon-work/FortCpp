#ifndef FortCpp_STRIDES_H
#define FortCpp_STRIDES_H
#include "ForwardDeclarations.h"

namespace FortCpp
{

namespace internal
{

/**
 * StrideValue
 */
template<>
class StrideValue<Unknown>
{
  private:
  int _s;
  public:
  StrideValue(const StrideValue &other)=default;
  inline explicit StrideValue() : _s(0) {}
  inline void set(const int s) { _s = s; }
  inline const int s() const {return _s;}
};

template<int _s>
class StrideValue
{
  private:
  public:
  StrideValue(const StrideValue &other)=default;
  inline explicit StrideValue() {}
  inline void set(const int s) {}
  static inline const int s() { return _s;}
};


/**
 * Strides
 */

template<int S1>
class Strides<S1>
{
  private:
  StrideValue<S1> _s1;
  public:
  Strides(const Strides &other)=default;
  inline explicit Strides() {}
  inline void set_str(const int s1) { _s1.set(s1); }
  inline const int s1() const {return _s1.s();}
};

template<int S1,int S2>
class Strides<S1,S2>
{
  private:
  StrideValue<S1> _s1;
  StrideValue<S2> _s2;
  public:
  Strides(const Strides &other)=default;
  inline explicit Strides() {}
  inline void set_str(const int s1,
                      const int s2) { _s1.set(s1); _s2.set(s2); }
  inline const int s1() const {return _s1.s();}
  inline const int s2() const {return _s2.s();}
};

template<int S1,int S2,int S3>
class Strides<S1,S2,S3>
{
  private:
  StrideValue<S1> _s1;
  StrideValue<S2> _s2;
  StrideValue<S3> _s3;
  public:
  Strides(const Strides &other)=default;
  inline explicit Strides() {}
  inline void set_str(const int s1,
                      const int s2,
                      const int s3) { _s1.set(s1); _s2.set(s2); _s3.set(s3); }
  inline const int s1() const {return _s1.s();}
  inline const int s2() const {return _s2.s();}
  inline const int s3() const {return _s3.s();}
};

template<int S1,int S2,int S3,int S4>
class Strides<S1,S2,S3,S4>
{
  private:
  StrideValue<S1> _s1;
  StrideValue<S2> _s2;
  StrideValue<S3> _s3;
  StrideValue<S4> _s4;
  public:
  Strides(const Strides &other)=default;
  inline explicit Strides() {}
  inline void set_str(const int s1,
                      const int s2,
                      const int s3,
                      const int s4) { _s1.set(s1); 
                                      _s2.set(s2); 
                                      _s3.set(s3);
                                      _s4.set(s4);}
  inline const int s1() const {return _s1.s();}
  inline const int s2() const {return _s2.s();}
  inline const int s3() const {return _s3.s();}
  inline const int s4() const {return _s4.s();}
};

template<int S1,int S2,int S3,int S4,int S5>
class Strides<S1,S2,S3,S4,S5>
{
  private:
  StrideValue<S1> _s1;
  StrideValue<S2> _s2;
  StrideValue<S3> _s3;
  StrideValue<S4> _s4;
  StrideValue<S5> _s5;
  public:
  Strides(const Strides &other)=default;
  inline explicit Strides() {}
  inline void set_str(const int s1,
                      const int s2,
                      const int s3,
                      const int s4,
                      const int s5) { _s1.set(s1); 
                                      _s2.set(s2); 
                                      _s3.set(s3);
                                      _s4.set(s4);
                                      _s5.set(s5);}
  inline const int s1() const {return _s1.s();}
  inline const int s2() const {return _s2.s();}
  inline const int s3() const {return _s3.s();}
  inline const int s4() const {return _s4.s();}
  inline const int s5() const {return _s5.s();}
};

};

};




#endif
