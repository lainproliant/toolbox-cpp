#include "lain/string.h"
#include "lain/testing.h"

using namespace std;
using namespace lain;
using namespace lain::testing;

int main() {
   return TestSuite("toolbox string.h tests")
      .die_on_signal(SIGSEGV)
      .test("String-001: str::startsWith", []() {
         assert_true(str::startsWith("oranges", "ora"));
         assert_false(str::startsWith("oranges", "oraz"));
         return true;
      })
      .test("String-002: str::endsWith", []() {
         assert_true(str::endsWith("oranges", "ges"));
         assert_false(str::endsWith("oranges", "gesz"));
         return true;
      })
      .test("String-003: str::join", []() {
         vector<int> v = {1, 2, 3, 4};
         assert_equal(str::join(v, ","), string("1,2,3,4"));
         return true;
      })
      .test("String-004: str::split", []() {
         vector<string> tokens;
         str::split(tokens, "1,2,3,4", ",");
         assert_true(lists_equal(tokens, {"1", "2", "3", "4"}));
         assert_true(lists_equal(str::split("1:2:3:4", ":"), {"1", "2", "3", "4"}));
         return true;
      })
      .test("String-005: str::argv_to_vector", []() {
         const char* argv[] = {"a", "b", "c", "d"};
         assert_true(lists_equal(str::argv_to_vector(4, (char**)argv), {"a", "b", "c", "d"}));
         return true;
      })
      .test("String-006: str::char_to_str", []() {
         assert_equal(str::char_to_str('c'), string("c"));
         return true;
      })
      .test("String-007: str::trim, str::trim_left, str::trim_right", []() {
         string s = "   abc   ";
         cout << "trim_left: \"" << str::trim_left(s) << "\"" << endl;
         cout << "trim_right: \"" << str::trim_right(s) << "\"" << endl;
         cout << "trim: \"" << str::trim(s) << "\"" << endl;
         assert_equal(str::trim_left(s), string("abc   "));
         assert_equal(str::trim_right(s), string("   abc"));
         assert_equal(str::trim(s), string("abc"));
         return true;
      })
      .run();
}
