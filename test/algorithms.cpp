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
      .test("Test alg::filter", [&]() {
         list<int> first = {1, 2, 3, 4, 5, 6};
         vector<int> second;

         alg::filter(first, second, [&](int x) {
            return !(x % 2);
         });

         tfm::printf("first = [%s]\n", str::join(first, ", "));
         tfm::printf("second = [%s]\n", str::join(second, ", "));

         assert_true(lists_equal(second, {2, 4, 6}));
         return true;
      })
      .test("Test alg::filter implicit form", [&]() {
         list<int> first = {1, 2, 3, 4, 5, 6};

         assert_true(lists_equal(alg::filter(first, [&](int x) {
            return !(x % 2);
         }), {2, 4, 6}));

         return true;
      })
      .test("Test alg::sum", [&]() {
         vector<int> vec = {1, 2, 3};
         assert_equal(alg::sum(vec, 0), 6);

         return true;
      })
      .test("Test alg::map", [&]() {
         vector<int> vec = {1, 2, 3};

         assert_true(lists_equal({2, 4, 6},
            alg::map<vector<int>>(vec, [&](auto& x) { return x * 2; })));

         return true;
      })
      .test("Test alg::sorted without custom sorter", [&]() {
         vector<int> vec = {8, 6, 7, 5, 3, 0, 9};
         vector<int> sorted = alg::sorted(vec);

         tfm::printf("vec = [%s]\n", str::join(vec, ", "));
         tfm::printf("sorted = [%s]\n", str::join(sorted, ", "));

         assert_true(lists_equal(vec, {8, 6, 7, 5, 3, 0, 9}));
         assert_true(lists_equal(sorted, {0, 3, 5, 6, 7, 8, 9}));

         return true;
      })
      .run();
}

