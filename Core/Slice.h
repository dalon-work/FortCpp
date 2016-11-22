#ifndef FortCpp_SLICE_h
#define FortCpp_SLICE_h

namespace FortCpp
{

static const int BEG = 156700;
static const int END = -156700;

struct SliceBase {
	int beg;
	int end;
	int str;

	SliceBase() : beg(0),end(END),str(1) {}
	SliceBase(int b) : beg(b),end(END),str(1) {}
	SliceBase(int b,int e) : beg(b),end(e),str(1) {}
	SliceBase(int b,int e,int s) : beg(b),end(e),str(s) {}

   /**
    * i is the size of the given dimension
    */
	int len(int i) {
		using namespace std;

#ifndef NDEBUG
		if ( str == 0 ) {
			throw ZeroSliceException(beg,end,str);
		}
#endif

		// compute start index
		if (beg == END) {
			beg = i-1;
		}
      else if (beg < 0) {
			beg = i+beg;
		}
		else if (beg == BEG) {
			beg = 0;
		}

#ifndef NDEBUG
		// bounds check
		if ( beg < 0 ) {
			throw BoundSliceException(beg,i);
		}
		if ( beg >= i ) {
			throw BoundSliceException(beg,i);
		}
#endif

		// compute end index
		if (end == END) {
			end = i;
		}
		else if (end < 0) {
			end = i+end;
		}
		else if (end == BEG) {
			end = -1;
		}

#ifndef NDEBUG
		// bounds check
		if ( end < -1 ) {
			throw BoundSliceException(end,i);
		}
		if ( end > i ) {
			throw BoundSliceException(end,i);
		}

		// zero slice check
		if ( (beg > end) && str > 0) {
			throw ZeroSliceException(beg,end,str);
		}
		if ( (end > beg) && str < 0) {
			throw ZeroSliceException(beg,end,str);
		}
#endif

		if (str < 0) {
			return 1+(end-beg+1)/str;
		}
		return 1+(end-beg-1)/str;
	}
};

struct FullSlice : public SliceBase {
	FullSlice() : SliceBase() {};
};

struct ContigSlice : public SliceBase {
	ContigSlice(int b) : SliceBase(b) {};
	ContigSlice(int b,int e) : SliceBase(b,e) {};
};

struct StridedSlice : public SliceBase {
	StridedSlice(int b,int e,int s) : SliceBase(b,e,s) {};
};

static inline
FullSlice Slice()
{
	return FullSlice();
}

static inline
ContigSlice Slice(int b)
{
	return ContigSlice(b);
}

static inline
ContigSlice Slice(int b,int e)
{
	return ContigSlice(b,e);
}

static inline
StridedSlice Slice(int b,int e,int s)
{
	return StridedSlice(b,e,s);
}

}; // end namespace FortCpp






#endif
