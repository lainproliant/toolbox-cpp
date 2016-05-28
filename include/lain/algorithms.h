#ifndef __LAIN_ALGORITHMS_H
#define __LAIN_ALGORITHMS_H

#include <algorithm>
#include <functional>
#include <numeric>
#include <set>
#include <vector>

namespace lain {
   namespace alg {
      template<class C1, class C2>
      void filter(const C1& src, C2& dest, std::function<bool(const typename C1::value_type&)> f) {
         std::copy_if(src.begin(), src.end(), std::back_inserter(dest), f);
      }

      template<class C1, class C2>
      void extend(C1& dest, const C2& src) {
         std::copy(src.begin(), src.end(), std::back_inserter(dest));
      }

      template<class C1>
      C1 filter(const C1& src, std::function<bool(typename C1::value_type)> f) {
         C1 result;
         std::copy_if(src.begin(), src.end(), std::back_inserter(result), f);
         return result;
      }

      template<class C1, class C2>
      void map(const C1& src, C2& dest, std::function<typename C2::value_type(const typename C1::value_type&)> f) {
         std::transform(src.begin(), src.end(), std::back_inserter(dest), f);
      }

      template<class C2, class C1>
      C2 map(const C1& src, std::function<typename C2::value_type(const typename C1::value_type&)> f) {
         C2 dest;
         std::transform(src.begin(), src.end(), back_inserter(dest), f);
         return dest;
      }

      template<class C2, class C1>
      C2 map(C1&& src, std::function<typename C2::value_type(const typename C1::value_type&)> f) {
         return map<C2, C1>(src, f);
      }

      template<class C1, class Compare = std::less<typename C1::value_type>>
      std::set<typename C1::value_type, Compare> to_set(const C1& src) {
         std::set<typename C1::value_type, Compare> result;
         for (auto iter = src.begin(); iter != src.end(); iter++) {
            result.insert(*iter);
         }
         return result;
      }

      template<class C1>
      C1 sorted(const C1& src) {
         C1 result;
         std::copy(src.begin(), src.end(), std::back_inserter(result));
         std::sort(result.begin(), result.end());
         return result;
      }

      template<class C1>
      C1 sorted(const C1& src, std::function<bool (const typename C1::value_type& a,
                                                   const typename C1::value_type& b)> comp) {
         C1 result;
         std::copy(src.begin(), src.end(), std::back_inserter(result));
         std::sort(result.begin(), result.end(), comp);
         return result;
      }

      template<class C1>
      typename C1::value_type sum(const C1& src, const typename C1::value_type& init) {
         return std::accumulate(src.begin(), src.end(), init);
      }
   }
}

#endif
