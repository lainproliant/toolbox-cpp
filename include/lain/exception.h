#ifndef __LAIN_EXCEPTION_H
#define __LAIN_EXCEPTION_H

#ifdef LAIN_ENABLE_STACKTRACE
#include <execinfo.h>
#endif

#include <exception>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <ostream>

#include "lain/string.h"
#include "tinyformat/tinyformat.h"

namespace lain {
   using namespace std;

   inline vector<string> generate_stacktrace(int max_frames = 256) {
#ifdef LAIN_ENABLE_STACKTRACE
      void* frames[max_frames];
      char** formatted_frames;
      size_t num_frames = backtrace(frames, max_frames);
      vector<string> btvec;
      formatted_frames = backtrace_symbols(frames, num_frames);
      for (size_t x = 0; x < num_frames; x++) {
         string formatted_frame = string(formatted_frames[x]);
         if (formatted_frame.size() > 0) {
            btvec.push_back(formatted_frame);
         }
      }
#else
      vector<string> btvec(0);
#endif

      return btvec;
   }

   inline string format_stacktrace(const vector<string>& stacktrace) {
      return tfm::format("Stack Trace -->\n\t%s",
            str::join(stacktrace, "\n\t"));
   }

   /**
    * A generic base exception class.
    */
   class Exception : public runtime_error {
   public:
      Exception(const string& message) : runtime_error(message.c_str()) {
         stacktrace = generate_stacktrace();
#ifdef LAIN_STACKTRACE_IN_DESCRIPTION
         this->message = tfm::format("%s\n%s\n",
            message, format_stacktrace());
#else
         this->message = message;
#endif
      }

      virtual const char* what() const throw()
      {
         return message.c_str();
      }

      virtual const string& get_message() const {
         return message;
      }

      string format_stacktrace() const {
         ostringstream sb;

         for (size_t x = 0; x < stacktrace.size(); x++) {
            sb << x << ": " << stacktrace[x] << endl;
         }

         return sb.str();
      }

      void print_stacktrace(ostream& out) const {
         out << format_stacktrace();
      }

      const vector<string>& get_stacktrace() const {
         return stacktrace;
      }

   private:
      string message;
      vector<string> stacktrace;
   };

   class ValueException : public Exception {
      using Exception::Exception;
   };

   class IndexException : public Exception {
      using Exception::Exception;
   };
}

#endif
