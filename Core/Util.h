#ifndef FortCpp_UTIL_H
#define FortCpp_UTIL_H

namespace FortCpp
{

namespace internal
{

bool is_negative(int i){
   if( i < 0 ){
      return 1;
   }
   else{
      return 0;
   }
}


template<typename... indices>
bool is_negative(int i,indices... idx){
   if( i < 0 ) {
      return 1;
   }
   else{
      return is_negative(idx...);
   }
};

}

#endif
