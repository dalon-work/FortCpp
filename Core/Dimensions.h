#ifndef FortCpp_DIMENSIONS_H
#define FortCpp_DIMENSIONS_H
#include "ForwardDeclarations.h"

namespace FortCpp
{

namespace internal
{



template<typename Derived>
class Dimensions<Derived,Unknown,1>
{
  private:
  int _d1;
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() : _d1(0) {}
  inline void set_dim(const int d1) { _d1 = d1; }
  const inline int d1() const {return _d1;}
};

template<typename Derived>
class Dimensions<Derived,Unknown,2>
{
  private:
  int _d1,_d2;
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() : _d1(0),_d2(0) {}
  inline void set_dim(const int d1,
                      const int d2) { _d1 = d1;
                                      _d2 = d2;}
  inline const int d1() const {return _d1;}
  inline const int d2() const {return _d2;}
};

template<typename Derived>
class Dimensions<Derived,Unknown,3>
{
  private:
  int _d1,_d2,_d3;
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() : _d1(0),_d2(0),_d3(0) {}
  inline void set_dim(const int d1,
                      const int d2,
                      const int d3) { _d1 = d1;
                                      _d2 = d2;
                                      _d3 = d3;}
  inline const int d1() const {return _d1;}
  inline const int d2() const {return _d2;}
  inline const int d3() const {return _d3;}
};

template<typename Derived>
class Dimensions<Derived,Unknown,4>
{
  private:
  int _d1,_d2,_d3,_d4;
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() : _d1(0),_d2(0),_d3(0),_d4(0) {}
  inline void set_dim(const int d1,
                      const int d2,
                      const int d3,
                      const int d4) { _d1 = d1;
                                      _d2 = d2;
                                      _d3 = d3;
                                      _d4 = d4;}
  inline const int d1() const {return _d1;}
  inline const int d2() const {return _d2;}
  inline const int d3() const {return _d3;}
  inline const int d4() const {return _d4;}
};

template<typename Derived>
class Dimensions<Derived,Unknown,5>
{
  private:
  int _d1,_d2,_d3,_d4,_d5;
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() : _d1(0),_d2(0),_d3(0),_d4(0),_d5(0) {}
  inline void set_dim(const int d1,
                      const int d2, 
                      const int d3, 
                      const int d4, 
                      const int d5) { _d1 = d1; 
                                      _d2 = d2;
                                      _d3 = d3;
                                      _d4 = d4;
                                      _d5 = d5;}
  inline const int d1() const {return _d1;}
  inline const int d2() const {return _d2;}
  inline const int d3() const {return _d3;}
  inline const int d4() const {return _d4;}
  inline const int d5() const {return _d5;}
};

template<typename Derived,int Size>
class Dimensions<Derived,Size,1>
{
  enum{
    D1 = traits<Derived>::D1
  };
  private:
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() {}
  inline void set_dim(const int d1) {}
  static inline const int d1() {return D1;}
};

template<typename Derived,int Size>
class Dimensions<Derived,Size,2>
{
  enum{
    D1 = traits<Derived>::D1,
    D2 = traits<Derived>::D2
  };
  private:
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() {}
  inline void set_dim(const int d1,
                      const int d2) {}
  static inline const int d1() {return D1;}
  static inline const int d2() {return D2;}
};

template<typename Derived,int Size>
class Dimensions<Derived,Size,3>
{
  enum{
    D1 = traits<Derived>::D1,
    D2 = traits<Derived>::D2,
    D3 = traits<Derived>::D3
  };
  private:
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() {}
  inline void set_dim(const int d1,
                      const int d2,
                      const int d3) {}
  static inline const int d1() {return D1;}
  static inline const int d2() {return D2;}
  static inline const int d3() {return D3;}
};

template<typename Derived,int Size>
class Dimensions<Derived,Size,4>
{
  enum{
    D1 = traits<Derived>::D1,
    D2 = traits<Derived>::D2,
    D3 = traits<Derived>::D3,
    D4 = traits<Derived>::D4
  };
  private:
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() {}
  inline void set_dim(const int d1,
                      const int d2,
                      const int d3,
                      const int d4) {}
  static inline const int d1() {return D1;}
  static inline const int d2() {return D2;}
  static inline const int d3() {return D3;}
  static inline const int d4() {return D4;}
};

template<typename Derived,int Size>
class Dimensions<Derived,Size,5>
{
  enum{
    D1 = traits<Derived>::D1,
    D2 = traits<Derived>::D2,
    D3 = traits<Derived>::D3,
    D4 = traits<Derived>::D4,
    D5 = traits<Derived>::D5
  };
  private:
  public:
  Dimensions(const Dimensions &other)=default;
  inline explicit Dimensions() {}
  inline void set_dim(const int d1,
                      const int d2,
                      const int d3,
                      const int d4,
                      const int d5) {}
  static inline const int d1() {return D1;}
  static inline const int d2() {return D2;}
  static inline const int d3() {return D3;}
  static inline const int d4() {return D4;}
  static inline const int d5() {return D5;}
};


};

};




#endif
