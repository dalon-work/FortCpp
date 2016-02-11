#ifndef FortCpp_sLICE_h
#define FortCpp_sLICE_h
#include "ForwardDeclarations.h"
#include <assert.h>

namespace FortCpp
{

static const int BEG = -1;
static const int END = -156700;

struct SliceBase {
	int beg;
	int end;
	int str;
	bool index;

	SliceBase() : beg(0),end(END),str(1),index(0) {}
	SliceBase(int b) : beg(b),end(END),str(1),index(0) {}
	SliceBase(int b,int e) : beg(b),end(e),str(1),index(0) {}
	SliceBase(int b,int e,int s) : beg(b),end(e),str(s),index(0) {}

	unsigned size(unsigned i) {
		assert(str!=0);

		if (beg < 0) {
			beg = i+beg;
		}
		else if (beg == END) {
			beg = i-1;
		}
		else if (beg == BEG) {
			beg = 0;
		}

		if (end == END) {
			end = i;
		}
		else if (end < 0) {
			end = i+end;
		}

		if (beg > end) {
			assert(str<0);
		}

		if (str < 0) {
			return 1+(end-beg+1)/str;
		}
		return 1+(end-beg-1)/str;
	}
};

struct FullSlice : public SliceBase
{
   FullSlice() : SliceBase() {};
};

struct ContigSlice : public SliceBase
{
   ContigSlice(int b) : SliceBase(b) {};
   ContigSlice(int b,int e) : SliceBase(b,e) {};
};

struct StridedSlice : public SliceBase
{
   StridedSlice(int b,int e,int s) : SliceBase(b,e,s) {};
};

FullSlice Slice()
{
   return FullSlice();
}

ContigSlice Slice(int b)
{
   return ContigSlice(b);
}

ContigSlice Slice(int b,int e)
{
   return ContigSlice(b,e);
}

StridedSlice Slice(int b,int e,int s)
{
   return StridedSlice(b,e,s);
}

}; // end namespace FortCpp






#endif
