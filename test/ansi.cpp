#include <iostream>
#include <ctime>
#include "lain/ansi.h"

namespace ansi = lain::ansi;
using namespace std;

int main() {
   ansi::screen::Size sz = ansi::screen::get_size();
   string message = "This should print at the bottom right of the screen.";

   cout << ansi::cur::save << ansi::cur::move(sz.width - message.length() + 1, sz.height)
             << ansi::bg::red << ansi::fg::bright_white << message << ansi::reset << ansi::cur::restore;
   
   cout.flush();

   sleep(5);

   cout << ansi::clear << "Test is complete!" << endl;

   return 0;
}


