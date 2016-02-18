#include "../FortCpp.h"

namespace FC = FortCpp;
using namespace std;

class dummy
{

   public:
      FC::Alloc<int,2> a;
      dummy()
      {
         a.allocate(10,10);
      }

      FC::Alloc<int,2> get_array_by_value()
      {
         return a;
      }

      FC::Alloc<int,2>& get_array_by_reference()
      {
         return a;
      }

      const FC::Alloc<int,2>& get_array_by_const_reference()
      {
         return a;
      }
      FC::Alloc<int,2>&& get_array_by_rvalue()
      {
         return std::move(a);
      }
};

FC::Alloc<int,2> return_by_value()
{
   FC::Alloc<int,2> a(10,10);
   a = 1;
   return a;
}

void pass_by_value(FC::Alloc<int,1> b)
{
   b = 2;
}

void pass_by_reference(FC::Alloc<int,1>& b)
{
   b = 3;
}

void pass_by_const_ref(const FC::Alloc<int,1>& b)
{
}

void pass_by_rvalue(FC::Alloc<int,1>&& b)
{
   b = 5;
}

int main()
{
   FC::Alloc<int,2> a(10,10);

   a = 0;

   a = return_by_value(); // move + assignment -- move elided, assignment NOT elided

   a.swap(return_by_value()); // move -- elided

   FC::Alloc<int,2> b = return_by_value(); // move -- elided

   auto s = FC::Slice();

   auto c = b.view(s,0); // move -- elided

   pass_by_value(c); // copy -- NOT elided
   pass_by_value(b.view(s,0)); // move -- elided -- destructor

   // pass_by_reference(b.view(s,0)); // not possible

   pass_by_reference(c); // nothing

   pass_by_const_ref(b.view(s,0)); // move -- elided -- destructor
   pass_by_const_ref(c); // nothing

   pass_by_rvalue(b.view(s,0)); // move -- elided -- destructor

   // pass_by_rvalue(c); // not possible
   pass_by_rvalue(std::move(c)); // nothing

   dummy d;

   auto e = d.get_array_by_value(); // copy + move -- copy NOT elided, move elided
   auto f = d.get_array_by_reference(); // copy -- NOT elided

   auto& g = d.get_array_by_reference(); // nothing
   auto& h = d.get_array_by_const_reference(); // nothing

   auto i = d.get_array_by_const_reference(); // copy -- NOT elided

   auto j = d.get_array_by_rvalue(); // move -- this deallocates d.a! -- NOT elided

   // auto& k = d.get_array_by_rvalue(); // not possible

    auto&& k = d.get_array_by_rvalue(); // nothing







}





