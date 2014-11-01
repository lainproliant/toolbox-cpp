/*
 * testing.h: A very simple to use unit testing framework
 *    built around lambda expressions.
 *
 * Author: Lain Supe (lainproliant)
 * Date: Thu October 9, 2014
 */
#ifndef __LAIN_TESTING_H
#define __LAIN_TESTING_H

#include <iostream>
#include <functional>
#include <vector>

#include "lain/exception.h"

namespace lain {
   namespace testing {
      using namespace std;

      class AssertionFailed : public Exception {
      public:
         AssertionFailed(const string& message) :
            Exception(message) {}
      };

      class UnitTest {
      public:
         UnitTest(const string& name, function<bool()> test_fn) :
            name(name), test_fn(test_fn) {}
         virtual ~UnitTest() {}

         bool run() const {
            return test_fn();
         }

         string get_name() const {
            return name;
         }

      private:
         function<bool()> test_fn;
         string name;
      };

      class TestSuite {
      public:
         TestSuite(const string& name) : name(name) {}
         virtual ~TestSuite() {}

         TestSuite& test(string name, std::function<bool()> test_fn) {
            return test(UnitTest(name, test_fn));
         }

         TestSuite& test(const UnitTest& test) {
            tests.push_back(test);
            return *this;
         }

         int run(ostream& out = cout) const {
            int tests_failed = 0;

            out << "===== " << name << " =====" << endl;

            for (const UnitTest& test : tests) {
               try {
                  out << "Running test: '"
                      << test.get_name()
                      << "'..." << endl;

                  if (test.run()) {
                     out << "    PASSED" << endl;

                  } else {
                     out << "    FAILED" << endl;
                     tests_failed ++;
                  }

               } catch (...) {
                  std::exception_ptr eptr = std::current_exception();

                  try {
                     std::rethrow_exception(eptr);
                  } catch (const std::exception& e) {
                     out << "    FAILED: " << e.what() << endl;
                  }

                  tests_failed ++;
               }
            }

            out << endl;

            return tests_failed;
         }

         int size() const {
            return tests.size();
         }

      private:
         vector<UnitTest> tests;
         string name;
      };

      inline void assert(bool expr, const string& message = "Assertion failed.") {
         if (! expr) {
            throw AssertionFailed(message);
         }
      }
   }
}

#endif
