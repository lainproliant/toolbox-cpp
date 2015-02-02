#ifndef __SIMPL_EXCEPTION_H
#define __SIMPL_EXCEPTION_H

#ifdef LAIN_ENABLE_STACKTRACE
#include <execinfo.h>
#endif

#include <exception>
#include <stdexcept>
#include <vector>
#include <ostream>

namespace lain {
   using namespace std;

   vector<string> generate_stacktrace(int max_frames = 256) {
#ifdef LAIN_ENABLE_STACKTRACE
      void* frames[max_frames];
      char** formatted_frames;
      size_t num_frames = backtrace(frames, max_frames);
      vector<string> btvec(num_frames);
      formatted_frames = backtrace_symbols(frames, num_frames);
      for (size_t x = 0; x < num_frames; x++) {
         btvec.push_back(string(formatted_frames[x]));
      }
#else
      vector<string> btvec(0);
#endif

      return btvec;
   }
   /**
    * A generic base exception class.
    */
   class Exception : public runtime_error {
   public:
      Exception(const string& message) :
         runtime_error(message.c_str()), message(message) {
         stacktrace = generate_stacktrace();
      }

      virtual const char* what() const throw()
      {
         return get_message().c_str();
      }

      virtual const string& get_message() const {
         return message;
      }

      void print_stacktrace(ostream& out) const {
         for (size_t x = 0; x < stacktrace.size(); x++) {
            out << x << ": " << stacktrace[x] << endl;
         }
      }

      const vector<string>& get_stacktrace() const {
         return stacktrace;
      }

   private:
      string message;
      vector<string> stacktrace;
   };
}

#endif
