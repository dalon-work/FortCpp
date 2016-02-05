#ifndef FortCpp_DIMENSIONS_H
#define FortCpp_DIMENSIONS_H
#include "ForwardDeclarations.h"
#include <array>

namespace FortCpp
{

namespace internal
{

/*********** PRODUCT ***********/

constexpr inline 
   unsigned product(unsigned i){
   return i;
}

template<typename... indices>
   constexpr inline 
   unsigned product(unsigned i,indices... idx){
      return i*product(idx...);
   }

template<unsigned Rank>
inline 
unsigned product(const std::array<unsigned,Rank> &idx){
   unsigned s=1;
   for(int r=0;r<Rank;r++){
      s *= idx[r];
   }
   return s;
}

/****** IS SAME HELPER **********/

// template<unsigned A,unsigned B>
// struct is_same
// {
//    static const bool value = 0;
// };

// template<unsigned A>
// struct is_same<A,A>
// {
//    static const bool value = 1;
// };

/****************** STRIDES ************************/

template<unsigned Rank,unsigned Order,unsigned Stride> struct Strides;

/****************** COLUMN MAJOR / CONTIG ************************/

template<unsigned Rank>
struct Strides<Rank,ColMajor,Contig>
{

   std::array<unsigned,Rank> str;

   Strides(){ for(int i=0;i<Rank;i++){ str[i]=1; } }

   void compute_strides(const std::array<unsigned,Rank>& dim)
   {
      for(int r=0;r<Rank-1;r++){
         for(int i=r+1;i<Rank;i++){
            str[r]*=dim[i];
         }
      }
   }

   constexpr
   unsigned offset (const std::array<unsigned,Rank>& dim, unsigned i) const
   {
      return i;
   }

};

/****************** COLUMN MAJOR / STRIDED ************************/

template<unsigned Rank>
struct Strides<Rank,ColMajor,Strided>
{

   std::array<unsigned,Rank> str;

   Strides(){ for(int i=0;i<Rank;i++){ str[i]=1; } }

   void compute_strides(const std::array<unsigned,Rank>& dim)
   {
      for(int r=0;r<Rank-1;r++){
         for(int i=r+1;i<Rank;i++){
            str[r]*=dim[i];
         }
      }
   }

   unsigned offset (const std::array<unsigned,Rank>& dim, unsigned i) const 
   {
      unsigned o= str[0]*(i%dim[0]);
      unsigned d=1;
      for(int r=1;r<Rank;r++){
         d *= dim[r-1];
         o += str[r]*((i/d)*dim[r]);
      }
      return o;
   }

};

/****************** ROW MAJOR / CONTIG ************************/

template<unsigned Rank>
struct Strides<Rank,RowMajor,Contig>
{
   std::array<unsigned,Rank> str;

   Strides(){ for(int i=0;i<Rank;i++){ str[i]=1; } }

   void compute_strides(const std::array<unsigned,Rank>& dim)
   {
      for(int r=1;r<Rank;r++){
         for(int i=0;i<Rank-r-1;i++){
            str[r] *= dim[i];
         }
      }
   }

  constexpr
   unsigned offset (const std::array<unsigned,Rank>& dim, unsigned i) const 
   {
      return i;
   }

};

/****************** ROW MAJOR / STRIDED ************************/

template<unsigned Rank>
struct Strides<Rank,RowMajor,Strided>
{
   std::array<unsigned,Rank> str;

   Strides(){ for(int i=0;i<Rank;i++){ str[i]=1; } }

   void compute_strides(const std::array<unsigned,Rank>& dim)
   {
      for(int r=1;r<Rank;r++){
         for(int i=0;i<Rank-r-1;i++){
            str[r] *= dim[i];
         }
      }
   }

   unsigned offset (const std::array<unsigned,Rank>& dim, unsigned i) const 
   {
      unsigned o = str[Rank-1]*(i%dim[Rank-1]);
      unsigned d=1;
      for(int r=Rank-1;r>=1;r--){
         d *= dim[r];
         o += str[r-1]*((i/d)*dim[r-1]);
      }
      return o;
   }

};

/************ DIMENSIONS *******************/

template<unsigned Rank,unsigned Order,unsigned Stride>
class Dimensions
{
  private:
     std::array<unsigned,Rank> dim = {{0}};
     Strides<Rank,Order,Stride> str;
     unsigned s = 0;

  // copy_dim copies the indices 
  // sent to set_dim into the dim array
  template<unsigned D>
     void copy_dim(unsigned i){
        dim[D] = i;
     }

  template<unsigned D,typename... indices>
     void copy_dim(unsigned i,indices... idx){
        dim[D] = i;
        copy_dim<D+1>(idx...);
     }


  // offset computes the index location of
  // an element in a MD Array
  template<unsigned D>
     constexpr unsigned offset(unsigned i) const{
        return  i*str.str[D];
     }

  template<unsigned D,typename... indices>
     constexpr unsigned offset(unsigned i,indices... idx) const{
        return i*str.str[D]+offset<D+1>(idx...);
     }

  public:

  Dimensions()=default;
  Dimensions(const Dimensions &other)=delete;
  Dimensions(const Dimensions &&other)=delete;
  ~Dimensions()=default;

  // set_dim sets the dimensions of the array
  template<typename... indices>
  void set_dim(indices... idx) {
     copy_dim<0>(idx...);
     str.compute_strides(dim);
     s = product(idx...);
  }

  void unset(){
     for(int r=0;r<Rank;r++){
        dim[r]=0;
        str.str[r]=0;
     }
     s=0;
  }

  void copy_dim(const std::array<unsigned,Rank>& otherdim,
        const std::array<unsigned,Rank>& otherstr){
     dim = otherdim;
     str.str = otherstr;
     s = product<Rank>(dim);
  }


  // computes the offset for a MD index
  template<typename... indices>
     constexpr unsigned operator () (indices... idx) const
     {
        return offset<0>(idx...);
     }

  constexpr unsigned operator [] (const unsigned i) const {return str.offset(dim,i);}

  constexpr unsigned size() const{
     return s;
  }

  constexpr unsigned size(unsigned D) const{
     return dim[D-1];
  }

  constexpr unsigned stride(unsigned D) const{
     return str[D-1];
  }

  const std::array<unsigned,Rank> & get_dim() const{
     return dim;
  }

  const std::array<unsigned,Rank> & get_str() const{
     return str.str;
  }

};

/***************** EQUALITY ********************/

template<unsigned LRank, unsigned LOrder, unsigned LStride,
         unsigned RRank, unsigned ROrder, unsigned RStride>
bool operator == (const Dimensions<LRank,LOrder,LStride> &lhs,
                  const Dimensions<RRank,ROrder,RStride> &rhs){
   static_assert(LRank == RRank,
         "ARRAY EXPRESSION RANKS NOT EQUAL");
   static_assert(LOrder == ROrder,
         "CANNOT MIX COLUMN AND ROW MAJOR ORDER IN ARRAY EXPRESSIONS");
   for(int i=1;i<=LRank;i++){
      if( lhs.size(i) != rhs.size(i)){
         return 0;
      }
   }
   return 1;
}
  
}; // end namespace internal

}; // end namespace FortCpp





#endif
