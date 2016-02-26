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

   Alloc<int,1> a(100);

   for(int i=0;i<100;i++){
      a[i] = i;
   }

   auto b = a.view(Slice(0,10));

   REQUIRE( b.size() == 10 );
   REQUIRE( b.size(1) == 10 );
   REQUIRE( b.stride(1) == 1 );

   for(int i=0;i<b.size(1);i++){
      REQUIRE(b[i] == i);
   }

   b = 1000;
   for(int i=0;i<10;i++){
      REQUIRE(a[i] == 1000);
   }

   REQUIRE( a[10] == 10 );

   auto c = a.view(Slice(0,100,2));

   a = 0;
   c = 1;

   for(int i=0;i<100;i++){
      if( i%2 == 0 ){
         REQUIRE(a[i] == 1);
      }
      else
      {
         REQUIRE(a[i] == 0);
      }
   }

   SECTION( "Multi-dimensions","[MD]") {
      SECTION( "Column Major" ){

         Alloc<int,2> d(10,10);
         for(int i=0;i<d.size();i++){
            d[i] = i;
         }
         auto e = d.view(Slice(),0);
         auto f = d.view(0,Slice());

         REQUIRE(e.contiguous());
         REQUIRE(e.rank() == 1);
         REQUIRE(e.size(1) == 10);

         REQUIRE_FALSE(f.contiguous());
         REQUIRE(f.rank() == 1);
         REQUIRE(f.size(1) == 10);

         for(int i=0;i<10;i++){
            CHECK(e[i] == i);
            CHECK(f[i] == i*10);
         }

         d = 0;
         auto g = d.view(Slice(BEG,END,2),Slice(BEG,END,2));


         REQUIRE_FALSE(g.contiguous());
         REQUIRE(g.rank() == 2);
         CHECK(g.size(1) == 5);
         CHECK(g.size(2) == 5);
         CHECK(g.size() == 25);

         for(int j=0;j<g.size(2);j++){
            for(int i=0;i<g.size(1);i++){
               g(i,j) = 1;
            }
         }

         for(int j=0;j<d.size(2);j++){
            for(int i=0;i<d.size(1);i++){
               if( j%2 == 0 && i%2==0 ){
                  CHECK(d(i,j) == 1);
               }
               else{
                  CHECK(d(i,j) == 0);
               }
            }
         }

         d = 0;
         g = 1;

         for(int j=0;j<d.size(2);j++){
            for(int i=0;i<d.size(1);i++){
               INFO( "i: " << i << " j: " << j << '\n';);
               if( j%2 == 0 && i%2==0 ){
                  CHECK(d(i,j) == 1);
               }
               else{
                  CHECK(d(i,j) == 0);
               }
            }
         }
      }

      SECTION("Row-Major"){
         Alloc<int,2,RowMajor> h(10,10);
         for(int i=0;i<h.size();i++){
            h[i] = i;
         }
         auto m = h.view(Slice(),0);
         auto n = h.view(0,Slice());

         REQUIRE(n.contiguous());
         REQUIRE(n.rank() == 1);
         REQUIRE(n.size(1) == 10);

         REQUIRE_FALSE(m.contiguous());
         REQUIRE(m.rank() == 1);
         REQUIRE(m.size(1) == 10);

         for(int i=0;i<10;i++){
            CHECK(n[i] == i);
            CHECK(m[i] == i*10);
         }

         h = 0;
         auto p = h.view(Slice(BEG,END,2),Slice(BEG,END,2));

         REQUIRE_FALSE(p.contiguous());
         REQUIRE(p.rank() == 2);
         CHECK(p.size(1) == 5);
         CHECK(p.size(2) == 5);
         CHECK(p.size() == 25);

         for(int i=0;i<p.size(1);i++){
            for(int j=0;j<p.size(2);j++){
               p(i,j) = 1;
            }
         }

         for(int i=0;i<h.size(1);i++){
            for(int j=0;j<h.size(2);j++){
               if( j%2 == 0 && i%2==0 ){
                  CHECK(h(i,j) == 1);
               }
               else{
                  CHECK(h(i,j) == 0);
               }
            }
         }

         h = 0;
         p = 1;

         for(int i=0;i<h.size(1);i++){
            for(int j=0;j<h.size(2);j++){
               INFO( "i: " << i << " j: " << j << '\n';);
               if( j%2 == 0 && i%2==0 ){
                  CHECK(h(i,j) == 1);
               }
               else{
                  CHECK(h(i,j) == 0);
               }
            }
         }
      }

      SECTION("Really big MultiD View","[Alloc][MD][View]"){

         SECTION("Column-Major","[CM]"){

            Alloc<int,4> d(10,10,10,10);

            auto s = Slice(BEG,END,2);
            auto e = d.view(s,s,s,s);

            d = 0;
            e = 1;

            for(int l=0;l<d.size(4);l++){
               for(int k=0;k<d.size(3);k++){
                  for(int j=0;j<d.size(2);j++){
                     for(int i=0;i<d.size(1);i++){
                        INFO( "i:" << i << " j:" << j << " k:" << k << " l:" << l);
                        if(l%2 == 0 && k%2==0 && j%2==0 && i%2==0){
                           CHECK(d(i,j,k,l) == 1);
                        }
                        else{
                           CHECK(d(i,j,k,l) == 0);
                        }
                     }
                  }
               }
            }

            auto f = e.view(Slice(BEG,END),0,0,0);

            CHECK( f.rank() == 1 );
            CHECK( f.size() == 5 );
            CHECK( f.stride(1) == 2 );

            f = 2;

            CHECK( d[0] == 2 );
            CHECK( d[2] == 2 );
            CHECK( d[4] == 2 );
            CHECK( d[6] == 2 );
            CHECK( d[8] == 2 );

         }
         
         SECTION("Row-Major","[RM]"){

            Alloc<int,4,RowMajor> d(10,10,10,10);

            auto s = Slice(BEG,END,2);
            auto e = d.view(s,s,s,s);

            d = 0;
            e = 1;

            for(int i=0;i<d.size(1);i++){
               for(int j=0;j<d.size(2);j++){
                  for(int k=0;k<d.size(3);k++){
                     for(int l=0;l<d.size(4);l++){
                        INFO( "i:" << i << " j:" << j << " k:" << k << " l:" << l);
                        if(l%2 == 0 && k%2==0 && j%2==0 && i%2==0){
                           CHECK(d(i,j,k,l) == 1);
                        }
                        else{
                           CHECK(d(i,j,k,l) == 0);
                        }
                     }
                  }
               }
            }

            auto f = e.view(0,0,0,Slice(BEG,END));

            CHECK( f.rank() == 1 );
            CHECK( f.size() == 5 );
            CHECK( f.stride(1) == 2 );

            f = 2;

            CHECK( d[0] == 2 );
            CHECK( d[2] == 2 );
            CHECK( d[4] == 2 );
            CHECK( d[6] == 2 );
            CHECK( d[8] == 2 );

         }
      }
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

TEST_CASE("Fixed","[Fixed]"){
   Fixed<int,3> a,b,c;

   REQUIRE( a.size<1>() == 3 );
   REQUIRE( a.size() == 3 );

   a = 1;
   b = 2;
   c = a+b;

   for(int i=0;i<3;i++){
      REQUIRE( c[i] == 3 );
   }

   for(int i=0;i<3; i++)
   {
      a[i] = i;
      REQUIRE( a(i) == i );
   }

   SECTION("MULTI-D"){
      Fixed<int,10,10,10,10> d;

      int m=0;
      for(int l=0;l<d.size<4>();l++){
         for(int k=0;k<d.size<3>();k++){
            for(int j=0;j<d.size<2>();j++){
               for(int i=0;i<d.size<1>();i++){
                  d(i,j,k,l) = m++;
               }
            }
         }
      }

      for(int i=0;i<d.size();i++){
         REQUIRE( d[i] == i );
      }
   }

}

TEST_CASE("Array Math","[Math]"){
   Alloc<double,1> a(10),c(10);
   Alloc<int,1> b(10);

   a = 2.0;
   b = 3.0;

   c = a*b;
   c = a+1;
   c = a+1.0;
   c = 1+a;
   c = 1.0+a;

   for(int i=0;i<c.size();i++){
      REQUIRE( c[i] == 3.0 );
   }

   c = a*b+2;

   for(int i=0;i<c.size();i++){
      REQUIRE(c[i] == 8.0);
   }

   c = -a;

   for(int i=0;i<c.size();i++){
      REQUIRE(c[i] == -a[i]);
   }

   c = Math::sqrt(a);

   for(int i=0;i<c.size();i++){
      REQUIRE(c[i] == Approx( std::sqrt(a[i]) ) );
   }

   c = Math::cos(a);

   for(int i=0;i<c.size();i++){
      REQUIRE(c[i] == Approx( std::cos(a[i]) ) );
   }

   using namespace Math;

   c = a*b+sqrt(a)*cos(b);

   for(int i=0;i<c.size();i++){
      REQUIRE(c[i] == Approx( a[i]*b[i]+std::sqrt(a[i])*std::cos(b[i]) ) );
   }


}









