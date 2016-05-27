#ifndef __LAIN_MAPS_H
#define __LAIN_MAPS_H

#include <vector>

namespace lain {
   namespace maps {
      using namespace std;

      template<class T>
      vector<typename T::key_type> keys(const T& map) {
         vector<typename T::key_type> kvec;

         for (auto iter = map.cbegin(); iter != map.cend(); iter++) {
            kvec.push_back(iter->first);
         }

         return kvec;
      }

      template<class T>
      vector<typename T::mapped_type> values(const T& map) {
         vector<typename T::mapped_type> vvec;

         for (auto iter = map.cbegin(); iter != map.cend(); iter++) {
            vvec.push_back(iter->second);
         }

         return vvec;
      }
   }
}


#endif
