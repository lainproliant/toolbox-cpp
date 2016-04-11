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
      .test("Test lain::filter", [&]()->bool {
         list<int> first = {1, 2, 3, 4, 5, 6};
         vector<int> second;

         filter(first, second, [&](int x)->bool {
            return !(x % 2); 
         });

         tfm::printf("first = [%s]\n", str::join(first, ", "));
         tfm::printf("second = [%s]\n", str::join(second, ", "));

         assert_true(lists_equal(second, {2, 4, 6}));
         return true;
      })
      .run();
}

