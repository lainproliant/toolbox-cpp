#ifndef __LAIN_UTIL_H
#define __LAIN_UTIL_H

#include <string>
#include <memory>
#include "exception.h"

namespace lain {
   using namespace std;

   class ValueException : public Exception {
      using Exception::Exception;
   };
   
   namespace util {
      inline void assertTrue(bool expr, const string& message) {
         if (! expr) {
            throw Exception(message);
         }
      }

      template<class T>
      void require(const T* ptr, const string& msg) {
         if (ptr == nullptr) {
            throw ValueException(msg);
         }
      }

      template<class T>
      void require(const shared_ptr<T>& ptr, const string& msg) {
         if (ptr == nullptr) {
            throw ValueException(msg);
         }
      }
   }
}

#endif
