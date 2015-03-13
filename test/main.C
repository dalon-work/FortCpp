#define FortCpp_TEST
#include "../FortCpp.h"
#include "FortranSlicing.h"
#include <cstdlib>

using namespace FortCpp;

template<int _Type,int _Dim,int _L,int _S>
void SliceToFSlice(internal::Slice<_Type,_Dim,_L,_S> &s,FSlice &f){
  f.b = s.b();
  f.e = s.b()+s.l();
  f.s = s.s();
};

int main()
{

  // Array1d Slicing
  {
    int d1 = 10;
    Array1d<double> a;
    a.allocate(d1);
    for(int i=0;i<a.size();i++){
      a[i] = i;
    }

    auto slice = S(0,5,3); FSlice S1;
    SliceToFSlice(slice,S1);

    auto b = a( slice );
    double * c_array = a.data();
    double * f_array;

    Dim1 d,l;
    d.d1 = d1;

    Array1d_S(d,&c_array,S1,l,&f_array);

    Array1d<double> f;
    f.map(f_array,l.d1);

    bool passed=1;
    for(int i=0;i<f.size();i++){
      if(f[i] != b[i]){
        passed=0;
      }
    }

    free_1d(l,&f_array);
    if(passed){
      cout << "1d slice correct" << endl;
    }
    else{
      cout << "1d slice failed" << endl;
    }
  }

  // Array2d Slicing
  {
    Dim1 l1,l2;
    Dim2 d,l3;
    d.d1 = 10;
    d.d2 = 10;
    int i = 1;
    int j = 2;

    Array2d<double> a;
    a.allocate(d.d1,d.d2);
    for(int i=0;i<a.size();i++){
      a[i] = i;
    }

    auto slice1 = S(0,5,3); 
    auto slice2 = S(2,8,2); 
    FSlice fs1,fs2;
    SliceToFSlice(slice1,fs1);
    SliceToFSlice(slice2,fs2);

    auto a1 = a( i , slice2 );
    auto a2 = a( slice1 , j );
    auto a3 = a( slice1 , slice2 );

    double * cp = a.data();
    double * fp1, *fp2, *fp3;

    Array2d_iS(d,&cp,i  ,fs2,l1,&fp1);
    Array2d_Sj(d,&cp,fs1,j  ,l2,&fp2);
    Array2d_SS(d,&cp,fs1,fs2,l3,&fp3);

    Array1d<double> fa1,fa2;
    Array2d<double> fa3;
    fa1.map(fp1,l1.d1);
    fa2.map(fp2,l2.d1);
    fa3.map(fp3,l3.d1,l3.d2);

    bool passed=1;
    for(int i=0;i<fa1.size();i++){
      if(fa1[i] != a1[i]){
        passed=0;
      }
    }
    if(passed){
      cout << "2d-iS slice correct" << endl;
    }
    else{
      cout << "2d-iS slice failed" << endl;
      for(int i=0;i<fa1.size();i++){
        cout << i << ' ' << fa1[i] << ' ' << a1[i] << endl;
      }
      cout << endl;
    }


    passed=1;
    for(int i=0;i<fa2.size();i++){
      if(fa2[i] != a2[i]){
        passed=0;
      }
    }
    if(passed){
      cout << "2d-Sj slice correct" << endl;
    }
    else{
      cout << "2d-Sj slice failed" << endl;
      for(int i=0;i<fa2.size();i++){
        cout << i << ' ' << fa2[i] << ' ' << a2[i] << endl;
      }
      cout << endl;
    }

    passed=1;
    for(int i=0;i<fa3.size();i++){
      if(fa3[i] != a3[i]){
        passed=0;
      }
    }
    if(passed){
      cout << "2d-SS slice correct" << endl;
    }
    else{
      cout << "2d-SS slice failed" << endl;
      for(int i=0;i<fa3.size();i++){
        cout << i << ' ' << fa3[i] << ' ' << a3[i] << endl;
      }
      cout << endl;
    }

    fa1.deallocate();
    fa2.deallocate();
    fa3.deallocate();

    free_1d(l1,&fp1);
    free_1d(l2,&fp2);
    free_2d(l3,&fp3);
  }

}

