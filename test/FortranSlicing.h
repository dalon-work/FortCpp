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
void Array1d_S(Dim1 d,double **a, FSlice S1, Dim1 &rd, double **r);

extern "C"
void Array2d_iS(Dim2 d,double **a, int i,FSlice S2, Dim1 &rd, double **r);
extern "C"
void Array2d_Sj(Dim2 d,double **a ,FSlice S1, int j, Dim1 &rd, double **r);
extern "C"
void Array2d_SS(Dim2 d,double **a ,FSlice S1, FSlice S2, Dim2 &rd, double **r);

extern "C"
void Array3d_ijS(Dim3 d,double **a ,int i,int j, FSlice S3, Dim1 &rd, double **r);
extern "C"
void Array3d_iSk(Dim3 d,double **a ,int i,FSlice S2,int k, Dim1 &rd, double **r);
extern "C"
void Array3d_Sjk(Dim3 d,double **a ,FSlice S1,int j,int k, Dim1 &rd, double **r);
extern "C"
void Array3d_iSS(Dim3 d,double **a ,int i,FSlice S2,FSlice S3, Dim2 &rd, double **r);
extern "C"
void Array3d_SjS(Dim3 d,double **a ,FSlice S1,int j,FSlice S3, Dim2 &rd, double **r);
extern "C"
void Array3d_SSk(Dim3 d,double **a ,FSlice S1,FSlice S2,int k, Dim2 &rd, double **r);
extern "C"
void Array3d_SSS(Dim3 d,double **a ,FSlice S1,FSlice S2,FSlice S3, Dim3 &rd, double **r);

extern "C"
void Array4d_ijkS(Dim4 d,double **a ,int i,int j,int k,FSlice S4,Dim1 &rd, double **r);
extern "C"
void Array4d_ijSl(Dim4 d,double **a ,int i,int j,FSlice S3,int l,Dim1 &rd, double **r);
extern "C"
void Array4d_iSkl(Dim4 d,double **a ,int i,FSlice S2,int k,int l,Dim1 &rd, double **r);
extern "C"
void Array4d_Sjkl(Dim4 d,double **a ,FSlice S1,int j,int k,int l,Dim1 &rd, double **r);
extern "C"
void Array4d_ijSS(Dim4 d,double **a ,int i,int j,FSlice S3,FSlice S4,Dim2 &rd, double **r);
extern "C"
void Array4d_iSkS(Dim4 d,double **a ,int i,FSlice S2,int k,FSlice S4,Dim2 &rd, double **r);
extern "C"
void Array4d_SjkS(Dim4 d,double **a ,FSlice S1,int j,int k,FSlice S4,Dim2 &rd, double **r);
extern "C"
void Array4d_iSSl(Dim4 d,double **a ,int i,FSlice S2,FSlice S3,int l,Dim2 &rd, double **r);
extern "C"
void Array4d_SjSl(Dim4 d,double **a ,FSlice S1,int j,FSlice S3,int l,Dim2 &rd, double **r);
extern "C"
void Array4d_SSkl(Dim4 d,double **a ,FSlice S1,FSlice S2,int k,int l,Dim2 &rd, double **r);
extern "C"
void Array4d_iSSS(Dim4 d,double **a ,int i,FSlice S2,FSlice S3,FSlice S4,Dim3 &rd, double **r);
extern "C"
void Array4d_SjSS(Dim4 d,double **a ,FSlice S1,int j,FSlice S3,FSlice S4,Dim3 &rd, double **r);
extern "C"
void Array4d_SSkS(Dim4 d,double **a ,FSlice S1,FSlice S2,int k,FSlice S4,Dim3 &rd, double **r);
extern "C"
void Array4d_SSSl(Dim4 d,double **a ,FSlice S1,FSlice S2,FSlice S3,int l,Dim3 &rd, double **r);
extern "C"
void Array4d_SSSS(Dim4 d,double **a ,FSlice S1,FSlice S2,FSlice S3,FSlice S4,Dim4 &rd, double **r);

#endif

