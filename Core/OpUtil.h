#ifndef FortCpp_OPUTIL_H
#define FortCpp_OPUTIL_H

namespace FortCpp
{

namespace internal
{


template<int Size> 
class OpSize
{
  public:
    OpSize(const int size) {}
    const int size() const { return Size; }
};

template<>
class OpSize<Unknown>
{
  const int _size;
  public:
    OpSize(const int size) : _size(size) {}
    const int size() const { return _size; }
};

};

};

#endif
