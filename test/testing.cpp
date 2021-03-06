#include <iostream>
#include "lain/testing.h"

using namespace std;
using namespace lain::testing;

int main(int argc, char** argv) {
   return TestSuite("TestSuite (testing.h)")
      .test("Forced failure by return type", []()->bool {
         ostream cnull(0);
         return TestSuite("internal test suite")
            .test("doomed test", []()->bool {
               return false;
            })
            .run(cnull) == 1;
      })
      .test("Forced failure by assertion", []()->bool {
         ostream cnull(0);
         return TestSuite("internal test suite")
            .test("doomed test", []()->bool {
               assert_true(false);
            })
            .run(cnull) == 1;
      })
      .test("Forced failure by runtime exception", []()->bool {
         ostream cnull(0);
         return TestSuite("internal test suite")
            .test("doomed test", []()->bool {
               throw runtime_error("oh noes!");
            })
            .run(cnull) == 1;
      })
      .run();

}

