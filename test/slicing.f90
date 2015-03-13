module slicing
use iso_c_binding
implicit none

public

type, bind(C) :: Slice
  integer(c_int) :: b
  integer(c_int) :: e
  integer(c_int) :: s
end type

type,bind(C) :: Dim1
  integer(c_int) :: d1
end type

type,bind(C) :: Dim2
  integer(c_int) :: d1
  integer(c_int) :: d2
end type

type,bind(C) :: Dim3
  integer(c_int) :: d1
  integer(c_int) :: d2
  integer(c_int) :: d3
end type

type,bind(C) :: Dim4
  integer(c_int) :: d1
  integer(c_int) :: d2
  integer(c_int) :: d3
  integer(c_int) :: d4
end type

type,bind(C) :: Dim5
  integer(c_int) :: d1
  integer(c_int) :: d2
  integer(c_int) :: d3
  integer(c_int) :: d4
  integer(c_int) :: d5
end type

interface assignment (=)
  module procedure assign_dim1
  module procedure assign_dim2
  module procedure assign_dim3
  module procedure assign_dim4
  module procedure assign_dim5
end interface


contains

subroutine convert_slice(S)
  type(Slice),intent(inout) :: S
  S%b = S%b+1
end subroutine convert_slice

subroutine assign_dim1(self,a)
  type(Dim1),intent(out) :: self
  integer,dimension(1),intent(in) :: a
  self%d1 = a(1)
end subroutine

subroutine assign_dim2(self,a)
  type(Dim2),intent(out) :: self
  integer,dimension(2),intent(in) :: a
  self%d1 = a(1)
  self%d2 = a(2)
end subroutine

subroutine assign_dim3(self,a)
  type(Dim3),intent(out) :: self
  integer,dimension(3),intent(in) :: a
  self%d1 = a(1)
  self%d2 = a(2)
  self%d3 = a(3)
end subroutine

subroutine assign_dim4(self,a)
  type(Dim4),intent(out) :: self
  integer,dimension(4),intent(in) :: a
  self%d1 = a(1)
  self%d2 = a(2)
  self%d3 = a(3)
  self%d4 = a(4)
end subroutine

subroutine assign_dim5(self,a)
  type(Dim5),intent(out) :: self
  integer,dimension(5),intent(in) :: a
  self%d1 = a(1)
  self%d2 = a(2)
  self%d3 = a(3)
  self%d4 = a(4)
  self%d5 = a(5)
end subroutine

!*********************!

subroutine free_1d(d,r) bind(C,name="free_1d")
  type(Dim1),intent(in) :: d
  type(c_ptr), intent(inout) :: r

  real(c_double),pointer :: c_array

  call c_f_pointer(r,c_array,[d%d1])

  deallocate(c_array)
  r = C_NULL_PTR
end subroutine free_1d

subroutine free_2d(d,r) bind(C,name="free_2d")
  type(Dim2),intent(in) :: d
  type(c_ptr), intent(inout) :: r

  real(c_double),pointer :: c_array

  call c_f_pointer(r,c_array,[d%d1,d%d2])

  deallocate(c_array)
  r = C_NULL_PTR
end subroutine free_2d

subroutine free_3d(d,r) bind(C,name="free_3d")
  type(Dim3),intent(in) :: d
  type(c_ptr), intent(inout) :: r

  real(c_double),pointer :: c_array

  call c_f_pointer(r,c_array,[d%d1,d%d2,d%d3])

  deallocate(c_array)
  r = C_NULL_PTR
end subroutine free_3d

subroutine free_4d(d,r) bind(C,name="free_4d")
  type(Dim4),intent(in) :: d
  type(c_ptr), intent(inout) :: r

  real(c_double),pointer :: c_array

  call c_f_pointer(r,c_array,[d%d1,d%d2,d%d3,d%d4])

  deallocate(c_array)
  r = C_NULL_PTR
end subroutine free_4d

subroutine free_5d(d,r) bind(C,name="free_5d")
  type(Dim5),intent(in) :: d
  type(c_ptr), intent(inout) :: r

  real(c_double),pointer :: c_array

  call c_f_pointer(r,c_array,[d%d1,d%d2,d%d3,d%d4,d%d5])

  deallocate(c_array)
  r = C_NULL_PTR
end subroutine free_5d

!*********************!

subroutine Array1d_S(d,a,S1,l,r) bind(C,name="Array1d_S")
  type(Dim1),value  :: d,l
  type(c_ptr),intent(in)    :: a
  type(Slice),value :: S1
  type(c_ptr),intent(out)   :: r

  real(c_double),pointer,dimension(:) :: c_array,f_array
  real(c_double),allocatable,dimension(:) :: temp

  call convert_slice(S1)
  call c_f_pointer(a,c_array,[d%d1])
  temp = c_array(S1%b : S1%e : S1%s)
  l = shape(temp)

  allocate(f_array(l%d1))
  f_array = temp
  r = c_loc(f_array(1))

end subroutine Array1d_S

!*********************!

subroutine Array2d_iS(d,a,i,S2,l,r) bind(C,name="Array2d_iS")
  type(Dim2),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim1),value :: l
  integer(c_int),value :: i
  type(Slice),value :: s2
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:),f_array(:)
  real(c_double),allocatable :: temp(:)

  i=i+1
  call convert_slice(S2)
  call c_f_pointer(a,c_array,[d%d1,d%d2])
  temp = c_array(i,S2%b : S2%e : S2%s)
  l = shape(temp)

  allocate(f_array(l%d1))
  f_array=temp
  r = c_loc(f_array(1))
  
end subroutine Array2d_iS

subroutine Array2d_Sj(d,a,S1,j,l,r) bind(C,name="Array2d_Sj")
  type(Dim2),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim1),value :: l
  integer(c_int),value :: j
  type(Slice),value :: S1
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:),f_array(:)
  real(c_double),allocatable :: temp(:)

  j=j+1
  call convert_slice(s1)
  call c_f_pointer(a,c_array,[d%d1,d%d2])
  temp = c_array(S1%b : S1%e : S1%s,j)
  l = shape(temp)

  allocate(f_array(l%d1))
  f_array=temp
  r = c_loc(f_array(1))
  
end subroutine Array2d_Sj

subroutine Array2d_SS(d,a,S1,S2,l,r) bind(C,name="Array2d_SS")
  type(Dim2),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim2),value :: l
  type(Slice),value :: S1,S2
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:),f_array(:,:)
  real(c_double),allocatable :: temp(:,:)

  call convert_slice(s1)
  call convert_slice(s2)
  call c_f_pointer(a,c_array,[d%d1,d%d2])
  temp = c_array(S1%b : S1%e : S1%s, S2%b : S2%e : S2%s)
  l = shape(temp)

  allocate(f_array(l%d1,l%d2))
  f_array=temp
  r = c_loc(f_array(1,1))
  
end subroutine Array2d_SS

!*********************!

subroutine Array3d_ijS(d,a,i,j,S3,l,r) bind(C,name="Array3d_ijS")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim1),value :: l
  integer(c_int),value :: i,j
  type(Slice),value :: s3
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:)
  real(c_double),allocatable :: temp(:)

  i=i+1
  j=j+1
  call convert_slice(S3)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(i,j,S3%b : S3%e : S3%s)
  l = shape(temp)

  allocate(f_array(l%d1))
  f_array=temp
  r = c_loc(f_array(1))
  
end subroutine Array3d_ijS

subroutine Array3d_iSk(d,a,i,S2,k,l,r) bind(C,name="Array3d_iSk")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim1),value :: l
  integer(c_int),value :: i,k
  type(Slice),value :: S2
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:)
  real(c_double),allocatable :: temp(:)

  i=i+1
  k=k+1
  call convert_slice(s2)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(i,S2%b : S2%e : S2%s,k)
  l = shape(temp)

  allocate(f_array(l%d1))
  f_array=temp
  r = c_loc(f_array(1))
  
end subroutine Array3d_iSk

subroutine Array3d_Sjk(d,a,S1,j,k,l,r) bind(C,name="Array3d_Sjk")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim1),value :: l
  integer(c_int),value :: j,k
  type(Slice),value :: S1
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:)
  real(c_double),allocatable :: temp(:)

  j=j+1
  k=k+1
  call convert_slice(S1)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(S1%b : S1%e : S1%s,j,k)
  l = shape(temp)

  allocate(f_array(l%d1))
  f_array=temp
  r = c_loc(f_array(1))
  
end subroutine Array3d_Sjk

subroutine Array3d_iSS(d,a,i,S2,S3,l,r) bind(C,name="Array3d_iSS")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  integer(c_int),value :: i
  type(Dim2),value :: l
  type(Slice),value :: S2,S3
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:,:)
  real(c_double),allocatable :: temp(:,:)

  call convert_slice(s2)
  call convert_slice(s3)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(i,S2%b : S2%e : S2%s, S3%b : S3%e : S3%s)
  l = shape(temp)

  allocate(f_array(l%d1,l%d2))
  f_array=temp
  r = c_loc(f_array(1,1))
  
end subroutine Array3d_iSS

subroutine Array3d_SjS(d,a,S1,j,S3,l,r) bind(C,name="Array3d_SjS")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  integer(c_int),value :: j
  type(Dim2),value :: l
  type(Slice),value :: S1,S3
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:,:)
  real(c_double),allocatable :: temp(:,:)

  call convert_slice(s1)
  call convert_slice(s3)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(S1%b : S1%e : S1%s,j, S3%b : S3%e : S3%s)
  l = shape(temp)

  allocate(f_array(l%d1,l%d2))
  f_array=temp
  r = c_loc(f_array(1,1))
  
end subroutine Array3d_SjS

subroutine Array3d_SSk(d,a,S1,S2,k,l,r) bind(C,name="Array3d_SSk")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  integer(c_int),value :: k
  type(Dim2),value :: l
  type(Slice),value :: S1,S2
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:,:)
  real(c_double),allocatable :: temp(:,:)

  call convert_slice(s1)
  call convert_slice(s2)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(S1%b : S1%e : S1%s, S2%b : S2%e : S2%s,k)
  l = shape(temp)

  allocate(f_array(l%d1,l%d2))
  f_array=temp
  r = c_loc(f_array(1,1))
  
end subroutine Array3d_SSk

subroutine Array3d_SSS(d,a,S1,S2,S3,l,r) bind(C,name="Array3d_SSS")
  type(Dim3),value :: d
  type(c_ptr),intent(in) :: a
  type(Dim3),value :: l
  type(Slice),value :: S1,S2,S3
  type(c_ptr),intent(out) :: r

  real(c_double),pointer :: c_array(:,:,:),f_array(:,:,:)
  real(c_double),allocatable :: temp(:,:,:)

  call convert_slice(s1)
  call convert_slice(s2)
  call convert_slice(s3)
  call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
  temp = c_array(S1%b : S1%e : S1%s, S2%b : S2%e : S2%s, S3%b : S3%e : S3%s)
  l = shape(temp)

  allocate(f_array(l%d1,l%d2,l%d3))
  f_array=temp
  r = c_loc(f_array(1,1,1))
  
end subroutine Array3d_SSS

!*********************!

! subroutine Array3d_ijS(d,a,i,j,S3,l,r) bind(C,name="Array3d_ijS")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   type(Dim1),value :: l
!   integer(c_int),value :: i,j
!   type(Slice),value :: s3
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:)
!   real(c_double),allocatable :: temp(:)
!
!   i=i+1
!   j=j+1
!   call convert_slice(S3)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(i,j,S3%b : S3%e : S3%s)
!   l = shape(temp)
!
!   allocate(f_array(l%d1))
!   f_array=temp
!   r = c_loc(f_array(1))
!   
! end subroutine Array3d_ijS
!
! subroutine Array3d_iSk(d,a,i,S2,k,l,r) bind(C,name="Array3d_iSk")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   type(Dim1),value :: l
!   integer(c_int),value :: i,k
!   type(Slice),value :: S2
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:)
!   real(c_double),allocatable :: temp(:)
!
!   i=i+1
!   k=k+1
!   call convert_slice(s2)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(i,S2%b : S2%e : S2%s,k)
!   l = shape(temp)
!
!   allocate(f_array(l%d1))
!   f_array=temp
!   r = c_loc(f_array(1))
!   
! end subroutine Array3d_iSk
!
! subroutine Array3d_Sjk(d,a,S1,j,k,l,r) bind(C,name="Array3d_Sjk")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   type(Dim1),value :: l
!   integer(c_int),value :: j,k
!   type(Slice),value :: S1
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:)
!   real(c_double),allocatable :: temp(:)
!
!   j=j+1
!   k=k+1
!   call convert_slice(S1)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(S1%b : S1%e : S1%s,j,k)
!   l = shape(temp)
!
!   allocate(f_array(l%d1))
!   f_array=temp
!   r = c_loc(f_array(1))
!   
! end subroutine Array3d_Sjk
!
! subroutine Array3d_iSS(d,a,i,S2,S3,l,r) bind(C,name="Array3d_iSS")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   integer(c_int),value :: i
!   type(Dim2),value :: l
!   type(Slice),value :: S2,S3
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:,:)
!   real(c_double),allocatable :: temp(:,:)
!
!   call convert_slice(s2)
!   call convert_slice(s3)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(i,S2%b : S2%e : S2%s, S3%b : S3%e : S3%s)
!   l = shape(temp)
!
!   allocate(f_array(l%d1,l%d2))
!   f_array=temp
!   r = c_loc(f_array(1,1))
!   
! end subroutine Array3d_iSS
!
! subroutine Array3d_SjS(d,a,S1,j,S3,l,r) bind(C,name="Array3d_SjS")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   integer(c_int),value :: j
!   type(Dim2),value :: l
!   type(Slice),value :: S1,S3
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:,:)
!   real(c_double),allocatable :: temp(:,:)
!
!   call convert_slice(s1)
!   call convert_slice(s3)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(S1%b : S1%e : S1%s,j, S3%b : S3%e : S3%s)
!   l = shape(temp)
!
!   allocate(f_array(l%d1,l%d2))
!   f_array=temp
!   r = c_loc(f_array(1,1))
!   
! end subroutine Array3d_SjS
!
! subroutine Array3d_SSk(d,a,S1,S2,k,l,r) bind(C,name="Array3d_SSk")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   integer(c_int),value :: k
!   type(Dim2),value :: l
!   type(Slice),value :: S1,S2
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:,:)
!   real(c_double),allocatable :: temp(:,:)
!
!   call convert_slice(s1)
!   call convert_slice(s2)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(S1%b : S1%e : S1%s, S2%b : S2%e : S2%s,k)
!   l = shape(temp)
!
!   allocate(f_array(l%d1,l%d2))
!   f_array=temp
!   r = c_loc(f_array(1,1))
!   
! end subroutine Array3d_SSk
!
! subroutine Array3d_SSS(d,a,S1,S2,S3,l,r) bind(C,name="Array3d_SSS")
!   type(Dim3),value :: d
!   type(c_ptr),intent(in) :: a
!   type(Dim3),value :: l
!   type(Slice),value :: S1,S2,S3
!   type(c_ptr),intent(out) :: r
!
!   real(c_double),pointer :: c_array(:,:,:),f_array(:,:,:)
!   real(c_double),allocatable :: temp(:,:,:)
!
!   call convert_slice(s1)
!   call convert_slice(s2)
!   call convert_slice(s3)
!   call c_f_pointer(a,c_array,[d%d1,d%d2,d%d3])
!   temp = c_array(S1%b : S1%e : S1%s, S2%b : S2%e : S2%s, S3%b : S3%e : S3%s)
!   l = shape(temp)
!
!   allocate(f_array(l%d1,l%d2,l%d3))
!   f_array=temp
!   r = c_loc(f_array(1,1,1))
!   
! end subroutine Array3d_SSS

end module slicing



  


