#include "catch.hpp"
#include "../FortCpp.h"
#include <cmath>

using namespace FortCpp;
using namespace std;

TEST_CASE( "Alloc Basics","[Basics][Alloc]" ){

   Alloc<int,1> a(10);

   REQUIRE( a.allocated() );
   REQUIRE( a.associated() );
   REQUIRE( a.size() == 10 );
   REQUIRE( a.size(1) == 10 );

   SECTION( "Alloc Allocation" ){
      Alloc<int,1> b;
      b.allocate(10);

      REQUIRE( b.allocated() );
      REQUIRE( b.associated() );
      REQUIRE( b.size() == 10 );
      REQUIRE( b.size(1) == 10 );

      SECTION( "Alloc Deallocation" ){
         a.deallocate();
         b.deallocate();

         REQUIRE_FALSE( b.allocated() );
         REQUIRE_FALSE( b.associated() );
         REQUIRE( b.size() == 0 );
         REQUIRE( b.size(1) == 0);

         REQUIRE_FALSE( a.allocated() );
         REQUIRE_FALSE( a.associated() );
         REQUIRE( a.size() == 0 );
         REQUIRE( a.size(1) == 0);
      }
   }

}

TEST_CASE( "Alloc Dimensions & Strides","[Basics][Alloc]" ){

   Alloc<int,1> a(10);
   Alloc<int,1,RowMajor> b(10);

   const auto& ad = a.get_dim();
   const auto& as = a.get_str();

   REQUIRE( a.size(1)   == 10 );
   REQUIRE( a.stride(1) == 1  );

   REQUIRE( b.size  (1) == 10 );
   REQUIRE( b.stride(1) == 1  );

   SECTION( "Alloc MD","[MD]"){
      Alloc<int,4>          c(10,10,10,10);
      Alloc<int,4,RowMajor> d(10,10,10,10);

      SECTION( "Column-Major","[CM]" ){

         REQUIRE( c.size() == 10*10*10*10 );
         for(int i=0;i<4;i++){
            CAPTURE(i);
            CHECK( c.size(i+1) == 10 );
            CHECK( c.stride(i+1) == pow(10,i) );
         }
      }

      c.deallocate();
      c.allocate(2,3,4,5);

      REQUIRE( c.size() == 2*3*4*5 );
      CHECK( c.stride(1) == 1 );
      CHECK( c.stride(2) == 2 );
      CHECK( c.stride(3) == 2*3 );
      CHECK( c.stride(4) == 2*3*4 );

      SECTION( "Row-Major","[RM]" ){

         REQUIRE( d.size() == 10*10*10*10 );
         for(int i=0;i<4;i++){
            CAPTURE(i);
            CHECK( d.size(i+1) == 10 );
            CHECK( d.stride(i+1) == pow(10,4-1-i) );
         }

         d.deallocate();
         d.allocate(2,3,4,5);

         REQUIRE( d.size() == 2*3*4*5 );
         CHECK( d.stride(1) == 3*4*5 );
         CHECK( d.stride(2) == 3*4 );
         CHECK( d.stride(3) == 3 );
         CHECK( d.stride(4) == 1 );

      }
   }
}


