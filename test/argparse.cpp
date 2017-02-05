#include "lain/argparse.h"
#include "lain/testing.h"

using namespace std;
using namespace lain;
using namespace lain::testing;

int main() {
   return TestSuite("toolbox argparse tests")
      .die_on_signal(SIGSEGV)
      .test("Argparse-001: Test basic arg parsing", [&]()->bool {
         vector<string> argv = {
            "test", "-abbbn", "12", "--input", "input.txt",
            "-q", "quest1.txt", "--quest", "quest2.txt", "--hardmode"};
         Arguments args = ArgumentBuilder()
            .arg('a')
            .arg('b')
            .arg('n').required()
            .arg('i', "input").required().option()
            .arg('q', "quest").required().option()
            .arg("hardmode")
            .arg("easymode")
            .parse(argv);

         assert_equal(args.count('a'), 1, "-a arg count");
         assert_equal(args.count('b'), 3, "-b arg count");
         assert_equal(args.count('n'), 1, "-n arg count");
         assert_equal(args.count('i'), 1, "-i arg count");
         assert_equal(args.count("input"), 1, "--input arg count");
         assert_equal(args.option('i'), string("input.txt"), "-i opt value");
         assert_equal(args.option("input"), string("input.txt"), "--input opt value");
         assert_true(args.check("hardmode"));
         assert_false(args.check("easymode"));

         return true;
      })
      .run();
}
