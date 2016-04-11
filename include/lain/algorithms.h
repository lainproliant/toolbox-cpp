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

   template<class C1>
   C1 filter(const C1& src, function<bool(typename C1::value_type)> f) {
      C1 result;
      copy_if(src.begin(), src.end(), back_inserter(result), f);
      return result;
   }

   template<class C1>
   typename C1::value_type sum(const C1& src, const typename C1::value_type& init) {
      return accumulate(src.begin(), src.end(), init);
   }
}
  
#endif
