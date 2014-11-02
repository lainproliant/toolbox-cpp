#include <iostream>
#include "lain/getopt.h"
#include "lain/testing.h"

using namespace std;
using namespace lain::getopt;
using namespace lain::testing;

int main(int argc, char** argv) {
   OptionParser options("abX:", {"alpha", "beta", "xtras="});

   return TestSuite("OptionParser (getopt.h) tests")
      .test("Test Reading Command Line Args (Required Mode)", [&]()->bool {
         options.parse({"./testapp", "-a", "-b", "-a", "-X", "1"});
         assert(options.get_count('a') == 2, "-a count should be 2");
         assert(options.get_count('b') == 1, "-b count should be 1");
         assert(options.get_count('X') == 1, "-X count should be 1");
         assert(options.get_param('X') == "1", "-X single param should equal \"1\"");
         return true;
      })
      .run();
}
