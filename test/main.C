#define FortCpp_TEST
#include "../FortCpp.h"
#include "FortranSlicing.h"
#include <cstdlib>
#include <string>
#include <random>

using namespace FortCpp;

template<int _Type,int _Dim,int _L,int _S>
void SliceToFSlice(const internal::Slice<_Type,_Dim,_L,_S> &s,FSlice &f){
  f.b = s.b();
  f.e = s.b()+s.l();
  f.s = s.s();
};

template<typename Derived1,typename Derived2>
void printArrays(const ArrayBase<Derived1> &fortran, const ArrayBase<Derived2> &cpp){
  cout << "Values:" << endl;
  int loop = max(fortran.size(),cpp.size());
  for(int i=0;i<loop;i++){
    cout << i << ": ";
    if(i < fortran.size()){ 
      cout << fortran.derived()[i]; 
    }
    else{
      cout << ' ';
    }
    if(i < cpp.size()){
      cout << ' ' << cpp.derived()[i];
    }
    else{
      cout << '\t';
    }
    cout << endl;
  }
};

template<typename Derived1,typename Derived2>
void test1d(string name , const ArrayBase1d<Derived1> &fortran , const ArrayBase1d<Derived2> &cpp){
  bool passed = 1;
  cout << "-----------" << endl;
  cout << name << endl << "-----------" << endl;
  if( fortran.size() != cpp.size() ){
    passed = 0;
    cout << "Sizes don't match" << endl;
    cout << "Fortran D1: " << fortran.d1() << " | C++ D1: " << cpp.d1() << endl;
  }
  if(passed){
    passed = 1;
    if(!all(fortran == cpp)){
      passed=0;
      cout << "Slices do not match" << endl;
    }
  }
  if(passed){
    cout << "Passed" << endl;
    printArrays(fortran,cpp);
  }
  else{
    printArrays(fortran,cpp);
  }
  cout << endl;
};

template<typename Derived1,typename Derived2>
void test2d(string name , ArrayBase2d<Derived1> &fortran , ArrayBase2d<Derived2> &cpp){
  bool passed = 1;
  cout << "-----------" << endl;
  cout << name << endl << "-----------" << endl;
  if( fortran.size() != cpp.size() ){
    passed = 0;
    cout << "Sizes don't match" << endl;
    cout << "Fortran D1: " << fortran.d1() << " | C++ D1: " << cpp.d1() << endl;
    cout << "Fortran D2: " << fortran.d2() << " | C++ D2: " << cpp.d2() << endl;
  }
  if(passed){
    passed = 1;
    if(!all(fortran == cpp)){
      passed=0;
      cout << "Slices do not match" << endl;
    }
  }
  if(passed){
    cout << "Passed" << endl;
    printArrays(fortran,cpp);
  }
  else{
    printArrays(fortran,cpp);
  }
  cout << endl;
};

template<typename Derived1,typename Derived2>
void test3d(string name , ArrayBase3d<Derived1> &fortran , ArrayBase3d<Derived2> &cpp){
  bool passed = 1;
  cout << "-----------" << endl;
  cout << name << endl << "-----------" << endl;
  if( fortran.size() != cpp.size() ){
    passed = 0;
    cout << "Sizes don't match" << endl;
    cout << "Fortran D1: " << fortran.d1() << " | C++ D1: " << cpp.d1() << endl;
    cout << "Fortran D2: " << fortran.d2() << " | C++ D2: " << cpp.d2() << endl;
    cout << "Fortran D3: " << fortran.d3() << " | C++ D3: " << cpp.d3() << endl;
  }
  if(passed){
    passed = 1;
    if(!all(fortran == cpp)){ 
      passed=0;
      cout << "Slices do not match" << endl;
    }
  }
  if(passed){
    cout << "Passed" << endl;
    printArrays(fortran,cpp);
  }
  else{
    printArrays(fortran,cpp);
  }
  cout << endl;
};

template<typename Derived1,typename Derived2>
void test4d(string name , ArrayBase4d<Derived1> &fortran , ArrayBase4d<Derived2> &cpp){
  bool passed = 1;
  cout << "-----------" << endl;
  cout << name << endl << "-----------" << endl;
  if( fortran.size() != cpp.size() ){
    passed = 0;
    cout << "Sizes don't match" << endl;
    cout << "Fortran D1: " << fortran.d1() << " | C++ D1: " << cpp.d1() << endl;
    cout << "Fortran D2: " << fortran.d2() << " | C++ D2: " << cpp.d2() << endl;
    cout << "Fortran D3: " << fortran.d3() << " | C++ D3: " << cpp.d3() << endl;
    cout << "Fortran D4: " << fortran.d4() << " | C++ D4: " << cpp.d4() << endl;
  }
  if(passed){
    passed = 1;
    if(!all(fortran == cpp)){
      passed=0;
      cout << "Slices do not match" << endl;
    }
  }
  if(passed){
    cout << "Passed" << endl;
    printArrays(fortran,cpp);
  }
  else{
    printArrays(fortran,cpp);
  }
  cout << endl;
};

template<typename Derived1,typename Derived2>
void test5d(string name , ArrayBase5d<Derived1> &fortran , ArrayBase5d<Derived2> &cpp){
  bool passed = 1;
  cout << "-----------" << endl;
  cout << name << endl << "-----------" << endl;
  if( fortran.size() != cpp.size() ){
    passed = 0;
    cout << "Sizes don't match" << endl;
    cout << "Fortran D1: " << fortran.d1() << " | C++ D1: " << cpp.d1() << endl;
    cout << "Fortran D2: " << fortran.d2() << " | C++ D2: " << cpp.d2() << endl;
    cout << "Fortran D3: " << fortran.d3() << " | C++ D3: " << cpp.d3() << endl;
    cout << "Fortran D4: " << fortran.d4() << " | C++ D4: " << cpp.d4() << endl;
    cout << "Fortran D5: " << fortran.d5() << " | C++ D5: " << cpp.d5() << endl;
  }
  if(passed){
    passed = 1;
    if(!all(fortran == cpp)){
      passed=0;
      cout << "Slices do not match" << endl;
    }
  }
  if(passed){
    cout << "Passed" << endl;
    printArrays(fortran,cpp);
  }
  else{
    printArrays(fortran,cpp);
  }
  cout << endl;
};

/**********************************/

template<FortCpp_SLICE_TEMPLATE(1)>
void run1d(FortCpp_SLICE_0(1) &slice1){
  FSlice fs1;
  SliceToFSlice(slice1,fs1);

  int d1 = 10;
  Array1d<double> a;
  a.allocate(d1);
  for(int i=0;i<a.size();i++){
    a[i] = i;
  }

  auto a1 = a( slice1 );
  double * cp = a.data();
  double * fp1;

  Dim1 d,l1;
  d.d1 = d1;

  Array1d_S(d,&cp,fs1,l1,&fp1);

  Array1d<double> fa1;
  fa1.map(fp1,l1.d1);

  test1d("1d_S",fa1,a1);

  free_1d(l1,&fp1);
};

template<FortCpp_SLICE_TEMPLATE(1),
         FortCpp_SLICE_TEMPLATE(2)>
void run2d(const FortCpp_SLICE_0(1) &slice1,
           const FortCpp_SLICE_0(2) &slice2){
  FSlice fs1,fs2;
  SliceToFSlice(slice1,fs1);
  SliceToFSlice(slice2,fs2);
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

  test1d("2d_iS",fa1,a1);
  test1d("2d_Sj",fa2,a2);
  test2d("2d_SS",fa3,a3);

  fa1.deallocate();
  fa2.deallocate();
  fa3.deallocate();

  free_1d(l1,&fp1);
  free_1d(l2,&fp2);
  free_2d(l3,&fp3);
};

template<FortCpp_SLICE_TEMPLATE(1),
         FortCpp_SLICE_TEMPLATE(2),
         FortCpp_SLICE_TEMPLATE(3)>
void run3d(const FortCpp_SLICE_0(1) &slice1,
           const FortCpp_SLICE_0(2) &slice2,
           const FortCpp_SLICE_0(3) &slice3){
  FSlice fs1,fs2,fs3;
  SliceToFSlice(slice1,fs1);
  SliceToFSlice(slice2,fs2);
  SliceToFSlice(slice3,fs3);

  Dim1 l1,l2,l3;
  Dim2 l4,l5,l6;
  Dim3 d,l7;
  d.d1 = 10;
  d.d2 = 10;
  d.d3 = 10;
  int i = 1;
  int j = 2;
  int k = 7;

  Array3d<double> a;
  a.allocate(d.d1,d.d2,d.d3);
  for(int i=0;i<a.size();i++){
    a[i] = i;
  }


  auto a1 = a(i,j,slice3);
  auto a2 = a(i,slice2,k);
  auto a3 = a(slice1,j,k);
  auto a4 = a(i,slice2,slice3);
  auto a5 = a(slice1,j,slice3);
  auto a6 = a(slice1,slice2,k);
  auto a7 = a(slice1,slice2,slice3);

  double *cp = a.data();
  double *fp1, 
         *fp2, 
         *fp3,
         *fp4,
         *fp5,
         *fp6,
         *fp7;
  Array3d_ijS(d,&cp,i,j,fs3,l1,&fp1);
  Array3d_iSk(d,&cp,i,fs2,k,l2,&fp2);
  Array3d_Sjk(d,&cp,fs1,j,k,l3,&fp3);
  Array3d_iSS(d,&cp,i,fs2,fs3,l4,&fp4);
  Array3d_SjS(d,&cp,fs1,j,fs3,l5,&fp5);
  Array3d_SSk(d,&cp,fs1,fs2,k,l6,&fp6);
  Array3d_SSS(d,&cp,fs1,fs2,fs3,l7,&fp7);

  Array1d<double> fa1,fa2,fa3;
  Array2d<double> fa4,fa5,fa6;
  Array3d<double> fa7;
  fa1.map(fp1,l1.d1);
  fa2.map(fp2,l2.d1);
  fa3.map(fp3,l3.d1);
  fa4.map(fp4,l4.d1,l4.d2);
  fa5.map(fp5,l5.d1,l5.d2);
  fa6.map(fp6,l6.d1,l6.d2);
  fa7.map(fp7,l7.d1,l7.d2,l7.d3);

  test1d("3d_ijS",fa1,a1);
  test1d("3d_iSk",fa2,a2);
  test1d("3d_Sjk",fa3,a3);
  test2d("3d_iSS",fa4,a4);
  test2d("3d_SjS",fa5,a5);
  test2d("3d_SSk",fa6,a6);
  test3d("3d_SSS",fa7,a7);

  fa1.deallocate();
  fa2.deallocate();
  fa3.deallocate();
  fa4.deallocate();
  fa5.deallocate();
  fa6.deallocate();
  fa7.deallocate();

  free_1d(l1,&fp1);
  free_1d(l2,&fp2);
  free_1d(l3,&fp3);
  free_2d(l4,&fp4);
  free_2d(l5,&fp5);
  free_2d(l6,&fp6);
  free_3d(l7,&fp7);

};

internal::Slice<internal::Contig,0,internal::Unknown> 
ContigSlice(mt19937_64 &mt,
            uniform_int_distribution<int> &rnum){
  int start = min(9,rnum(mt));
  int end = rnum(mt);
  while( end <= start ){ end = rnum(mt); }
  return S(start,end);
};

internal::Slice<internal::Unknown,0,internal::Unknown,internal::Unknown>
RandomSlice(mt19937_64 &mt,
            uniform_int_distribution<int> &rnum){
  uniform_int_distribution<int> snum(1,4);
  int start = min(9,rnum(mt));
  int end = rnum(mt);
  while( end <= start ){ end = rnum(mt); }
  int step = snum(mt);
  return S(start,end,step);
};

/**************************************/

int main()
{
  mt19937_64 mt(0);
  uniform_int_distribution<int> rnum(0,10);

  // cout << "Full Slices" << endl;
  // cout << "-----------" << endl << endl;
  // {
  //   auto slice1 = S();
  //   auto slice2 = S();
  //   auto slice3 = S();
  //   auto slice4 = S();
  //   auto slice5 = S();
  //   run1d(slice1);
  //   run2d(slice1,slice2);
  //   run3d(slice1,slice2,slice3);
  // }
  cout << "Contig Slices" << endl;
  cout << "-------------" << endl;
  {
    auto slice1 = ContigSlice(mt,rnum);
    auto slice2 = ContigSlice(mt,rnum);
    auto slice3 = ContigSlice(mt,rnum);
    auto slice4 = ContigSlice(mt,rnum);
    auto slice5 = ContigSlice(mt,rnum);
    run1d(slice1);
    run2d(slice1,slice2);
    run3d(slice1,slice2,slice3);
  }
    cout << "Random Slices" << endl;
    cout << "-------------" << endl;
  {
    auto slice1 = RandomSlice(mt,rnum);
    auto slice2 = RandomSlice(mt,rnum);
    auto slice3 = RandomSlice(mt,rnum);
    auto slice4 = RandomSlice(mt,rnum);
    auto slice5 = RandomSlice(mt,rnum);
    run1d(slice1);
    run2d(slice1,slice2);
    run3d(slice1,slice2,slice3);
  }





}

