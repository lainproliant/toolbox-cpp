/*
 * log.h: A logger with a familiar interface.
 *
 * Powered by tinyformat.
 *
 * Author: Lain Supe (lainproliant)
 * Date: Thu November 27, 2014
 */
#ifndef __LAIN_LOGGER_H
#define __LAIN_LOGGER_H

#include <mutex>
#include <map>
#include "picojson/picojson.h"

namespace lain {
   namespace log {
      using namespace std;
      
      const int FATAL = 0;
      const int ERROR = 10;
      const int WARNING = 20;
      const int INFO = 30;
      const int DEBUG = 40;
      const int TRACE = 50;

      const map<string, int> LEVEL_MAP = {
         {"FATAL",      FATAL},
         {"ERROR",      ERROR},
         {"WARNING",    WARNING},
         {"INFO",       INFO},
         {"DEBUG",      DEBUG},
         {"TRACE",      TRACE}
      };

      const string DEFAULT_PREFIX = "[%date]<%name> ";

      class LineWriter {
      public:
         LineWriter(ostream& output) : output(output) { }
         virtual ~LineWriter() { }

         void writeLine(const string& line) {
            lock_guard<mutex> lock(mtx);
            output << line << endl;
         }

      private:
         mutex mtx;
      };

      class Config {
      public:
         static Config from_json(const pj::value& value) {
         
         }
         
         Config() {
            log_level = DEBUG;
            prefix = DEFAULT_PREFIX;
         }

         virtual ~Config () { }

         const string& get_prefix() const {
            return prefix;
         }

         const int get_log_level() const {
            return log_level;
         }

      private:
         int log_level;
         string prefix;
      };

      class Logger {
      public:

      private:

      };
   }
}

#endif
