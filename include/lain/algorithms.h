#ifndef __LAIN_ALGORITHMS_H
#define __LAIN_ALGORITHMS_H

#include <algorithm>
#include <functional>

namespace lain {
   using namespace std;

   template<class C1, class C2>
   void filter(const C1& src, C2& dest, function<bool(typename C1::value_type)> f) {
      copy_if(src.begin(), src.end(), back_inserter(dest), f); 
   }
}

#endif
