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
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <cfloat>
#include <cmath>

#include "lain/exception.h"

namespace lain {
   namespace testing {
      using namespace std;

      class TestException : public Exception {
      public:
         using Exception::Exception;
      };

      class AssertionFailed : public TestException {
      public:
         using TestException::TestException;
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

         TestSuite& die_on_signal(int signalId) {
            signal(signalId, signal_callback);
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
                     out << "    FAILED (" << typeid(e).name() << "): " << e.what() << endl;
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
         static void signal_callback(int signal) {
            cerr << endl << "FATAL: Caught signal " << signal
               << " (" << strsignal(signal) << ")"
               << endl;
            exit(1);
         }

         vector<UnitTest> tests;
         string name;
      };

      inline void assert_true(bool expr, const string& message = "Assertion failed.") {
         if (! expr) {
            throw AssertionFailed(message);
         }
      }

      inline void assert_false(bool expr, const string& message = "Negative assertion failed.") {
         if (expr) {
            throw AssertionFailed(message);
         }
      }

      inline void epsilon_assert(double a, double b,
            double epsilon = DBL_EPSILON,
            const string& message = "Value equivalence assertion failed. (double epsilon)") {

         if (abs(a - b) > epsilon) {
            throw AssertionFailed(message);
         }
      }

      inline void epsilon_assert(float a, float b,
            float epsilon = FLT_EPSILON,
            const string& message = "Value equivalence assertion failed. (float epsilon)") {
         if (fabs(a - b) > epsilon) {
            throw AssertionFailed(message);
         }
      }

      template<class T>
      inline void assert_equal(const T& a, const T& b,
            const string& message = "Value equivalence assertion failed.") {
         if (a != b) {
            throw AssertionFailed(message);
         }
      }

      template<>
      inline void assert_equal<double>(const double& a, const double& b, const string& message) {
         try {
            epsilon_assert(a, b);

         } catch (const AssertionFailed& e) {
            throw AssertionFailed(message);
         }
      }

      template<>
      inline void assert_equal<float>(const float& a, const float& b, const string& message) {
         try {
            epsilon_assert(a, b);

         } catch (const AssertionFailed& e) {
            throw AssertionFailed(message);
         }
      }

      template<class T>
      inline size_t generic_list_size(const T& listA) {
         size_t sz = 0;
         for (auto iter = listA.begin(); iter != listA.end(); iter++) {
            sz++;
         }
         return sz;
      }

      template<class T>
      inline bool lists_equal(const T& listA, const T& listB) {

         return generic_list_size(listA) == generic_list_size(listB) &&
            equal(listA.begin(), listA.end(), listB.begin());
      }
   }
}

#endif
