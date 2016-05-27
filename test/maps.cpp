#include "lain/maps.h"
#include "lain/algorithms.h"
#include "lain/testing.h"
#include <map>

using namespace std;
using namespace lain;
using namespace lain::testing;

int main() {
   return TestSuite("map functions (maps.h) tests")
      .test("map::keys test", [&]()->bool {
         map<string, int> M = {
            {"apple", 1},
            {"banana", 2},
            {"orange", 3}};

         assert_true(lists_equal(
            alg::sorted(maps::keys(M)),
            {"apple", "banana", "orange"}));
         assert_true(lists_equal(
            alg::sorted(maps::values(M)),
            {1, 2, 3}));
         return true;
      })
      .run();
}
