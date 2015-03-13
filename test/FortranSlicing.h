#ifndef FortCpp_FORTRANSLICING_H
#define FortCpp_FORTRANSLICING_H

struct FSlice
{
  int b = 0;
  int e = 0;
  int s = 0;
};

struct Dim1
{
  int d1 = 0;
};

struct Dim2
{
  int d1 = 0;
  int d2 = 0;
};

struct Dim3
{
  int d1 = 0;
  int d2 = 0;
  int d3 = 0;
};

struct Dim4
{
  int d1 = 0;
  int d2 = 0;
  int d3 = 0;
  int d4 = 0;
};

struct Dim5
{
  int d1 = 0;
  int d2 = 0;
  int d3 = 0;
  int d4 = 0;
  int d5 = 0;
};


extern "C"
void free_1d(Dim1 &d1,double **r);
extern "C"
void free_2d(Dim2 &d2,double **r);
extern "C"
void free_3d(Dim3 &d3,double **r);
extern "C"
void free_4d(Dim4 &d4,double **r);
extern "C"
void free_5d(Dim5 &d5,double **r);

extern "C"
void Array1d_S(Dim1 d,double **a, FSlice S1, Dim1 l, double **r);

extern "C"
void Array2d_iS(Dim2 d,double **a, int i,FSlice S2, Dim1 l, double **r);
extern "C"
void Array2d_Sj(Dim2 d,double **a ,FSlice S1, int j, Dim1 l, double **r);
extern "C"
void Array2d_SS(Dim2 d,double **a ,FSlice S1, FSlice S2, Dim2 l, double **r);

#endif

