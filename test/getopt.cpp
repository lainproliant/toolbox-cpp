#include <iostream>
#include "lain/getopt.h"
#include "lain/testing.h"
#include "lain/string.h"

using namespace std;
using namespace lain::getopt;
using namespace lain::testing;

int main() {

   return TestSuite("OptionParser (getopt.h) tests")
      .test("Test Reading Command Line Args (Required Mode)", [&]()->bool {
         OptionParser options("abX:", {"alpha", "beta", "xtras="});
         options.parse({"./testapp", "-a", "-b", "-a", "-X", "1"});
         assert_true(options.get_count('a') == 2, "-a count should be 2");
         assert_true(options.get_count('b') == 1, "-b count should be 1");
         assert_true(options.get_count('X') == 1, "-X count should be 1");
         assert_true(options.get_param('X') == "1", "-X single param should equal \"1\"");
         return true;
      })
      .test("Test shortopt/longopt combined param functions.", [&]()->bool {
         OptionParser options("i:o:vc:x", {"input=", "output=", "verbose", "config=", "xtra"});
         options.parse({"/.testapp", "-i", "inputA.txt", "--input", "inputB.txt",
                        "-o", "outputA.txt", "-o", "outputB.txt", "--output", "outputC.txt",
                        "-v", "--verbose", "-v", "--config", "master.ini", "-c", "other.ini",
                        "-x"});
         assert_true(options.get_count("verbose", 'v') == 3, "-v/--verbose count should be 3");
         cout << "input = " << str::list_repr(options.get_param_list("input", 'i')) << endl;
         assert_true(lists_equal(options.get_param_list("input", 'i'),
                                 {"inputB.txt", "inputA.txt"}),
                                 "(-i) longopts should occur first in the list");
         cout << "output = " << str::list_repr(options.get_param_list("output", 'o')) << endl;
         assert_true(lists_equal(options.get_param_list("output", 'o'),
                                 {"outputC.txt", "outputA.txt", "outputB.txt"}),
                                 "(-o) longopts should occur first in the list.");
         cout << "config = " << options.get_param("config", 'c') << endl;
         assert_true(options.get_param("config", 'c') == "master.ini", "(-c) should be master.ini");
         cout << "xtra = " << str::bool_repr(options.check_option("xtra", 'x')) << endl;
         assert_true(options.check_option("xtra", 'x'), "xtra should be specified by shortopt");
         return true;
      })
      .run();
}
