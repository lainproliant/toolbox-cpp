#ifndef __LAIN_MMAP_H
#define __LAIN_MMAP_H

#include <map>
#include <vector>

namespace lain {
   namespace mmap {
      using namespace std;

      template<class K, class T>
      struct Mapping {
         K key;
         vector<T> values;
      };

      template<class K, class T>
      class Builder {
      public:
         Builder(const vector<Mapping<K, T>>& mappings) :
            mappings(mappings)
         { }

         multimap<K, T> build() {
            multimap<K, T> mmap;

            for (auto mapping : mappings) {
               for (auto value : mapping.values) {
                  mmap.insert(make_pair(mapping.key, value));
               }
            }

            return mmap;
         }

      private:
         vector<Mapping<K, T>> mappings;
      };
      
      template<class M>
      inline vector<typename M::mapped_type> collect(const M& mmap, const typename M::key_type& key) {
         vector<typename M::mapped_type> values;
         auto range = mmap.equal_range(key);

         for (auto iter = range.first; iter != range.second; iter++) {
            values.push_back(iter->second);
         }
         
         return values;
      }
   }
}

#endif
