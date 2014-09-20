#include <iostream>
#include "lain/getopt.h"

using namespace std;
using namespace lain::getopt;

int main(int argc, char** argv) {
   OptionParser options = OptionParser::parse(argc, argv,
         "abX:", {"alpha", "beta", "xtras="});
 
   options.debug(cout);
   return 0; 
}
