#ifndef ACCESSOR_CC_H
#define ACCESSOR_CC_H

namespace FortCpp
{

namespace internal
{

template<typename Derived,unsigned Rank>
class Accessor<Derived,Rank,ColMajor,Contig>
{
	enum {
	    Size = traits<Derived>::Size
	};
private:
	// Dimensions<Derived,Rank,Size> dim;
	unsigned dim[Rank];
public:

	Accessor(const Accessor& other)=default;
	inline explicit Accessor() {}

	template<unsigned indices...>
	inline const int operator () (indices... idx) const {
		return dim.offset(idx);
	}
	// {return i+dim.d1()*(j+dim.d2()*(k+dim.d3()*(l+dim.d4()*m)));}
	inline const int operator [] (const int i) const {return i;}
};


template<typename Derived,unsigned Rank>
class Accessor<Derived,Rank,RowMajor,Contig>
{
	enum {
	    Size = traits<Derived>::Size
	};
private:
	Dimensions<Derived,Size,5> dim;
public:
	Accessor(const Accessor& other)=default;
	inline explicit Accessor() {}
	inline void set_dim(const int d1,
	                    const int d2,
	                    const int d3,
	                    const int d4,
	                    const int d5) {dim.set_dim(d1,d2,d3,d4,d5);}
	inline const int operator () (const int i,
	                              const int j,
	                              const int k,
	                              const int l,
	                              const int m) const
	{return dim.d5()*(dim.d4()*(dim.d3()*(dim.d2()*i+j)+k)+l)+m;}
	inline const int operator [] (const int i) const {return i;}
	inline const int d1() const {return dim.d1();}
	inline const int d2() const {return dim.d2();}
	inline const int d3() const {return dim.d3();}
	inline const int d4() const {return dim.d4();}
	inline const int d5() const {return dim.d5();}
};

// #<{(|* One Dimensional Arrays *|)}>#
//
// template<typename Derived,int Order,int... StridePack>
// class Accessor<Derived,1,Order,Contig,StridePack...>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,1> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1) { dim.set_dim(d1); }
//   inline void set_str(const int d1) {}
//   inline const int operator () (const int i) const { return i; }
//   inline const int operator [] (const int i) const { return i; }
//   inline const int d1() const {return dim.d1();}
//   inline const int s1() const {return 1;}
// };
//
// template<typename Derived,int Order,int... StridePack>
// class Accessor<Derived,1,Order,Strided,StridePack...>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,1> dim;
//   Strides<StridePack...> sts;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1) { dim.set_dim(d1); }
//   inline void set_str(const int s1) { sts.set_str(s1); }
//   inline const int operator () (const int i) const { return i*sts.s1(); }
//   inline const int operator [] (const int i) const { return i*sts.s1(); }
//   inline const int d1() const {return dim.d1();}
//   inline const int s1() const {return sts.s1();}
// };
//
// #<{(|* Two Dimensional Accessors *|)}>#
// template<typename Derived>
// class Accessor<Derived,2,ColMajor,Contig>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,2> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2) { dim.set_dim(d1,d2); }
//   inline const int operator () (const int i,
//                                 const int j) const { return i+j*dim.d1(); }
//   inline const int operator [] (const int i) const {return i;}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
// };
//
// template<typename Derived,int... StridePack>
// class Accessor<Derived,2,ColMajor,Strided,StridePack...>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,2> dim;
//   Strides<StridePack...> sts;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2) { dim.set_dim(d1,d2); }
//   inline void set_str(const int s1,
//                       const int s2) { sts.set_str(s1,s2); }
//   inline const int operator () (const int i,
//                                 const int j) const {return i*sts.s1()+j*sts.s2; }
//   inline const int operator [] (const int i) const {return sts.s1()*(i%dim.d1())+sts.s2()*(i/dim.d1());}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int s1() const {return sts.s1();}
//   inline const int s2() const {return sts.s2();}
// };
//
// template<typename Derived>
// class Accessor<Derived,2,RowMajor,Contig>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,2> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2) { dim.set_dim(d1,d2); }
//   inline const int operator () (const int i,
//                                 const int j) const { return i*dim.d2()+j; }
//   inline const int operator [] (const int i) const {return i;}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
// };
//
// #<{(| 3 Dimensional Arrays |)}>#
// template<typename Derived>
// class Accessor<Derived,3,ColMajor,Contig>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,3> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2,
//                       const int d3) {dim.set_dim(d1,d2,d3);}
//   inline const int operator () (const int i,
//                                 const int j,
//                                 const int k) const
//                                 {return i+dim.d1()*(j+dim.d2()*k); }
//   inline const int operator [] (const int i) const {return i;}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int d3() const {return dim.d3();}
// };
//
// template<typename Derived,int... StridePack>
// class Accessor<Derived,3,ColMajor,Strided,StridePack...>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,3> dim;
//   Strides<StridePack...> sts;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2,
//                       const int d3) { dim.set_dim(d1,d2,d3); }
//   inline void set_str(const int s1,
//                       const int s2,
//                       const int s3) { sts.set_str(s1,s2,s3); }
//   inline const int operator () (const int i,
//                                 const int j,
//                                 const int k) const {return i*sts.s1()+j*sts.s2()+k*sts.s3(); }
//   inline const int operator [] (const int i) const {return sts.s1()*(i%dim.d1())
//                                                           +sts.s2()*((i/dim.d1())%dim.d2())
//                                                           +sts.s3()*((i/(dim.d1()*dim.d2())));}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int d3() const {return dim.d3();}
//   inline const int s1() const {return sts.s1();}
//   inline const int s2() const {return sts.s2();}
//   inline const int s3() const {return sts.s3();}
// };
//
// template<typename Derived>
// class Accessor<Derived,3,RowMajor,Contig>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,3> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2,
//                       const int d3) {dim.set_dim(d1,d2,d3);}
//   inline const int operator () (const int i,
//                                 const int j,
//                                 const int k) const
//                                 {return dim.d3()*(dim.d2()*i+j)+k; }
//   inline const int operator [] (const int i) const {return i;}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int d3() const {return dim.d3();}
// };
//
// #<{(|* 4 Dimensional Arrays |)}>#
// template<typename Derived>
// class Accessor<Derived,4,ColMajor,Contig>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,4> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2,
//                       const int d3,
//                       const int d4) {dim.set_dim(d1,d2,d3,d4);}
//   inline const int operator () (const int i,
//                                 const int j,
//                                 const int k,
//                                 const int l) const
//                                 {return i+dim.d1()*(j+dim.d2()*(k+l*dim.d3()));}
//   inline const int operator [] (const int i) const {return i;}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int d3() const {return dim.d3();}
//   inline const int d4() const {return dim.d4();}
// };
//
// template<typename Derived,int... StridePack>
// class Accessor<Derived,4,ColMajor,Strided,StridePack...>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,4> dim;
//   Strides<StridePack...> sts;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2,
//                       const int d3,
//                       const int d4) { dim.set_dim(d1,d2,d3,d4); }
//   inline void set_str(const int s1,
//                       const int s2,
//                       const int s3,
//                       const int s4) { sts.set_str(s1,s2,s3,s4); }
//   inline const int operator () (const int i,
//                                 const int j,
//                                 const int k,
//                                 const int l) const {return i*sts.s1()+j*sts.s2()+k*sts.s3()+l*sts.s4(); }
//   inline const int operator [] (const int i) const {return sts.s1()*(i%dim.d1())
//                                                           +sts.s2()*((i/dim.d1())%dim.d2())
//                                                           +sts.s3()*((i/(dim.d1()*dim.d2()))%dim.d3())
//                                                           +sts.s4()*((i/(dim.d1()*dim.d2()*dim.d3())));}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int d3() const {return dim.d3();}
//   inline const int d4() const {return dim.d4();}
//   inline const int s1() const {return sts.s1();}
//   inline const int s2() const {return sts.s2();}
//   inline const int s3() const {return sts.s3();}
//   inline const int s4() const {return sts.s4();}
// };
//
// template<typename Derived>
// class Accessor<Derived,4,RowMajor,Contig>
// {
//   enum{
//     Size = traits<Derived>::Size
//   };
//   private:
//   Dimensions<Derived,Size,4> dim;
//   public:
//   Accessor(const Accessor &other)=default;
//   inline explicit Accessor() {}
//   inline void set_dim(const int d1,
//                       const int d2,
//                       const int d3,
//                       const int d4) {dim.set_dim(d1,d2,d3,d4);}
//   inline const int operator () (const int i,
//                                 const int j,
//                                 const int k,
//                                 const int l) const
//                                 {return dim.d4()*(dim.d3()*(dim.d2()*i+j)+k)+l;}
//   inline const int operator [] (const int i) const {return i;}
//   inline const int d1() const {return dim.d1();}
//   inline const int d2() const {return dim.d2();}
//   inline const int d3() const {return dim.d3();}
//   inline const int d4() const {return dim.d4();}
// };
//
}; // end namespace internal

}; // end namespace FortCpp


#endif


