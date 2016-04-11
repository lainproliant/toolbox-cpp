#include "lain/algorithms.h"
#include "lain/testing.h"

#include <list>
#include <vector>
#include <tinyformat/tinyformat.h>
#include <lain/string.h>

using namespace std;
using namespace lain;
using namespace lain::testing;
namespace tfm = tinyformat;

int main() {
   return TestSuite("Algorithms (algorithms.h) tests")
      .die_on_signal(SIGSEGV)
      .test("Test lain::filter", [&]() {
         list<int> first = {1, 2, 3, 4, 5, 6};
         vector<int> second;

         filter(first, second, [&](int x) {
            return !(x % 2); 
         });

         tfm::printf("first = [%s]\n", str::join(first, ", "));
         tfm::printf("second = [%s]\n", str::join(second, ", "));

         assert_true(lists_equal(second, {2, 4, 6}));
         return true;
      })
      .test("Test lain::filter implicit form", [&]() {
         list<int> first = {1, 2, 3, 4, 5, 6};

         assert_true(lists_equal(filter(first, [&](int x) {
            return !(x % 2);
         }), {2, 4, 6}));

         return true;
      })
      .test("Test lain::sum", [&]() {
         vector<int> vec = {1, 2, 3};
         assert_equal(lain::sum(vec, 0), 6);

         return true;
      })
      .run();
}

