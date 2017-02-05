#include "lain/mmap.h"
#include "lain/testing.h"

using namespace std;
using namespace lain;
using namespace lain::testing;

int main(int argc, char** argv) {
   return TestSuite("mmap (mmap.h) tests")
      .test("mmap::Builder static initialization", [&]()->bool {
         auto mmap = mmap::Builder<string, string>({
            {"fruit", {"apple", "orange", "banana", "pear"}},
            {"meat", {"beef", "chicken", "pork", "fish", "lamb"}},
            {"drink", {"coffee", "tea", "ice water"}}})
            .build();

         assert_true(! lists_equal(mmap::collect(mmap, "fruit"),
                  {"apple", "banana", "pear"}));

         assert_true(lists_equal(mmap::collect(mmap, "fruit"),
                  {"apple", "orange", "banana", "pear"}));
         return true;
      })
      .run();
}
