#ifndef __SIMPL_EXCEPTION_H
#define __SIMPL_EXCEPTION_H

#include <exception>
#include <stdexcept>

namespace lain {
   using namespace std;

   /**
    * A generic base exception class.
    */
   class Exception : public runtime_error {
   public:
      Exception(const string& message) :
         runtime_error(message.c_str()), message(message) {}

      virtual const char* what() const throw()
      {
         return get_message().c_str();
      }

      virtual const string& get_message() const {
         return message;
      }

   private:
      string message;
   };
}

#endif
