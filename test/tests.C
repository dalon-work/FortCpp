#include "catch.hpp"
#include "../FortCpp.h"
#include <cmath>
#include <iostream>

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

      SECTION( "Row-Major" ){

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
         CHECK( d.stride(2) == 4*5 );
         CHECK( d.stride(3) == 5 );
         CHECK( d.stride(4) == 1 );

      }
   }
}

TEST_CASE("Mapping","[Basics][Alloc]"){

   Alloc<int,1> a;
   Alloc<int,1,RowMajor> b;
   int p[10];
   a.map(p,10);

   REQUIRE( a.size() == 10);
   REQUIRE( a.stride(1) == 1);
   REQUIRE( a.size(1) == 10);
   REQUIRE( a.data() == p );

   a.deallocate();

   REQUIRE( a.data() == nullptr );
   REQUIRE( a.size() == 0 );
   REQUIRE( a.size(1) == 0 );
   REQUIRE( a.stride(1) == 0 );

   b.map(p,10);

   REQUIRE( b.size() == 10);
   REQUIRE( b.stride(1) == 1);
   REQUIRE( b.size(1) == 10);
   REQUIRE( b.data() == p );


   SECTION("Alloc MD"){

      int p[2*3*4*5];

      SECTION("Column-Major"){

         Alloc<int,4> c;
         c.map(p,2,3,4,5);

         REQUIRE( c.data() == p );
         REQUIRE( c.size() == 2*3*4*5 );
         CHECK( c.size(1) == 2 );
         CHECK( c.size(2) == 3 );
         CHECK( c.size(3) == 4 );
         CHECK( c.size(4) == 5 );

         CHECK( c.stride(1) == 1 );
         CHECK( c.stride(2) == 2 );
         CHECK( c.stride(3) == 2*3 );
         CHECK( c.stride(4) == 2*3*4 );
      }

      SECTION("Row-Major"){
         Alloc<int,4,RowMajor> d;
         d.map(p,2,3,4,5);

         REQUIRE( d.data() == p );
         REQUIRE( d.size() == 2*3*4*5 );
         CHECK( d.size(1) == 2 );
         CHECK( d.size(2) == 3 );
         CHECK( d.size(3) == 4 );
         CHECK( d.size(4) == 5 );

         CHECK( d.stride(1) == 3*4*5 );
         CHECK( d.stride(2) == 4*5 );
         CHECK( d.stride(3) == 5 );
         CHECK( d.stride(4) == 1 );
      }
   }

}

TEST_CASE("Alloc Indexing","[Basic][Alloc][Contig]"){

   Alloc<int,1> a(10);

   for(int i=0;i<a.size();i++){
      a[i] = i;
   }

   for(int i=0;i<a.size();i++){
      CAPTURE(i);
      CHECK(a[i] == i);
   }

   a.deallocate();
   a.allocate(10);

   for(int i=0;i<a.size();i++){
      a(i) = i;
   }

   for(int i=0;i<a.size();i++){
      CAPTURE(i);
      CHECK(a(i) == i);
   }

   Alloc<int,4> b(2,3,4,5);
   Alloc<int,4,RowMajor> c(2,3,4,5);

   for(int i=0;i<b.size();i++){
      b[i] = i;
      c[i] = i;
   }

   for(int i=0;i<b.size();i++){
      CAPTURE(i);
      REQUIRE( b[i] == i );
      REQUIRE( c[i] == i );
   }


   SECTION("Column-Major") {
      int count=0;
      for(int l=0;l<b.size(4);l++){
         for(int k=0;k<b.size(3);k++){
            for(int j=0;j<b.size(2);j++){
               for(int i=0;i<b.size(1);i++){
                  CHECK(b(i,j,k,l) == count);
                  count++;
               }
            }
         }
      }
   }

   SECTION("Row-Major") {
      int count=0;
      for(int i=0;i<c.size(1);i++){
         for(int j=0;j<c.size(2);j++){
            for(int k=0;k<c.size(3);k++){
               for(int l=0;l<c.size(4);l++){
                  CHECK(c(i,j,k,l) == count);
                  count++;
               }
            }
         }
      }
   }
}

TEST_CASE("Array Operations","[Op][Alloc]"){
   
   Alloc<int,1> a(10),b(10),c(10);

   a = 1;
   b = 2;

   for(int i=0;i<a.size();i++){
      REQUIRE( a[i] == 1 );
      REQUIRE( b[i] == 2 );
   }

   b = a;

   for(int i=0;i<b.size();i++){
      CHECK( b[i] == a[i] );
   }

}

TEST_CASE("Alloc View","[Alloc][View]"){

   SECTION("Column-Major"){

      Alloc<int,4> a(10,10,10,10);

      auto f = Slice();
      auto c = Slice(2,3);
      auto s = Slice(1,10,4);

      auto s1 = a.view(f,0,0,0);
      auto s2 = a.view(f,f,0,0);
      auto s3 = a.view(f,f,f,0);
      auto s4 = a.view(f,f,f,f);

      CHECK(s1.contiguous());
      CHECK(s2.contiguous());
      CHECK(s3.contiguous());
      CHECK(s4.contiguous());

      CHECK(s1.rank() == 1);
      CHECK(s2.rank() == 2);
      CHECK(s3.rank() == 3);
      CHECK(s4.rank() == 4);

      auto s5 = a.view(c,0,0,0);
      auto s6 = a.view(c,c,0,0);
      auto s7 = a.view(c,c,c,0);
      auto s8 = a.view(c,c,c,c);

      CHECK_FALSE(s5.contiguous());
      CHECK_FALSE(s6.contiguous());
      CHECK_FALSE(s7.contiguous());
      CHECK_FALSE(s8.contiguous());

      CHECK(s5.rank() == 1);
      CHECK(s6.rank() == 2);
      CHECK(s7.rank() == 3);
      CHECK(s8.rank() == 4);

      auto s9  = a.view(s,0,0,0);
      auto s10 = a.view(s,s,0,0);
      auto s11 = a.view(s,s,s,0);
      auto s12 = a.view(s,s,s,s);

      CHECK_FALSE(s9 .contiguous());
      CHECK_FALSE(s10.contiguous());
      CHECK_FALSE(s11.contiguous());
      CHECK_FALSE(s12.contiguous());

      CHECK(s9 .rank() == 1);
      CHECK(s10.rank() == 2);
      CHECK(s11.rank() == 3);
      CHECK(s12.rank() == 4);

      auto s13 = a.view(0,f,0,0);
      auto s14 = a.view(f,0,f,0);
      auto s15 = a.view(f,0,0,f);
      auto s16 = a.view(f,0,s,0);

      CHECK_FALSE(s13.contiguous());
      CHECK_FALSE(s14.contiguous());
      CHECK_FALSE(s15.contiguous());
      CHECK_FALSE(s16.contiguous());

      CHECK(s13.rank() == 1);
      CHECK(s14.rank() == 2);
      CHECK(s15.rank() == 2);
      CHECK(s16.rank() == 2);
   }

   SECTION("Row-Major"){

      Alloc<int,4,RowMajor> a(10,10,10,10);

      auto f = Slice();
      auto c = Slice(2,3);
      auto s = Slice(1,3,4);

      auto s1 = a.view(0,0,0,f);
      auto s2 = a.view(0,0,f,f);
      auto s3 = a.view(0,f,f,f);
      auto s4 = a.view(f,f,f,f);

      CHECK(s1.contiguous());
      CHECK(s2.contiguous());
      CHECK(s3.contiguous());
      CHECK(s4.contiguous());

      auto s5 = a.view(0,0,0,c);
      auto s6 = a.view(0,0,c,c);
      auto s7 = a.view(0,c,c,c);
      auto s8 = a.view(c,c,c,c);

      CHECK_FALSE(s5.contiguous());
      CHECK_FALSE(s6.contiguous());
      CHECK_FALSE(s7.contiguous());
      CHECK_FALSE(s8.contiguous());

      auto s9  = a.view(0,0,0,s);
      auto s10 = a.view(0,0,s,s);
      auto s11 = a.view(0,s,s,s);
      auto s12 = a.view(s,s,s,s);

      CHECK_FALSE(s9 .contiguous());
      CHECK_FALSE(s10.contiguous());
      CHECK_FALSE(s11.contiguous());
      CHECK_FALSE(s12.contiguous());

      auto s13 = a.view(0,f,0,0);
      auto s14 = a.view(0,f,0,f);
      auto s15 = a.view(0,c,0,f);
      auto s16 = a.view(f,0,s,0);

      CHECK_FALSE(s13.contiguous());
      CHECK_FALSE(s14.contiguous());
      CHECK_FALSE(s15.contiguous());
      CHECK_FALSE(s16.contiguous());
   }

}

#ifndef NDEBUG
TEST_CASE("Alloc Exceptions","[Alloc][Exception]"){
   Alloc<int,1> a(10),b(11);

   CHECK_THROWS_AS( a(-1) , NegIdxException );
   CHECK_THROWS_AS( a(10) , BoundsException );
   CHECK_THROWS_AS( a[-1] , SizeException );

   a = 1;

   CHECK_THROWS_AS( a=b , DimException );

   Alloc<int,4> c(2,2,2,2),d(2,2,3,2),e(2,2,2,2),f(2,2,2,2);

   c = 2;
   d = 3;
   e = 4;

   CHECK_THROWS_AS( c(0,2,0,0), BoundsException );
   CHECK_THROWS_AS( d=c, DimException );
   CHECK_THROWS_AS( d(1,1,2,-1), NegIdxException );

   CHECK_THROWS_AS( e = f+c+d , DimException );

}
#endif








