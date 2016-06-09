#ifndef __LAIN_ANSI_H
#define __LAIN_ANSI_H

#include <string>
#include "lain/string.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace lain {
   using namespace std;

   namespace ansi {
      inline string seq(const string& code) {
         return "\033[" + code;
      }
      
      inline string attr(initializer_list<int> attrs) {
         return seq(str::join(attrs, ";") + "m");
      }
      
      const string reset = attr({0});
      const string bright = attr({1});
      const string dim = attr({2});
      const string underscore = attr({4});
      const string blink = attr({5});
      const string reverse = attr({7});
      const string hidden = attr({8});
      const string fraktur = attr({20});

      const string clear = seq("2J") + seq("0;0H");
      const string clear_eol = seq("K");

      namespace fg {
         const string black = attr({30});
         const string red = attr({31});
         const string green = attr({32});
         const string yellow = attr({33});
         const string blue = attr({34});
         const string magenta = attr({35});
         const string cyan = attr({36});
         const string white = attr({37});

         const string bright_black = attr({1, 30});
         const string bright_red = attr({1, 31});
         const string bright_green = attr({1, 32});
         const string bright_yellow = attr({1, 33});
         const string bright_blue = attr({1, 34});
         const string bright_magenta = attr({1, 35});
         const string bright_cyan = attr({1, 36});
         const string bright_white = attr({1, 37});
      }

      namespace bg {
         const string black = attr({40});
         const string red = attr({41});
         const string green = attr({42});
         const string yellow = attr({43});
         const string blue = attr({44});
         const string magenta = attr({45});
         const string cyan = attr({46});
         const string white = attr({47});
      }
      
      namespace cur {
         const string hide = seq("?25l");
         const string show = seq("?25h");
         const string save = seq("s");
         const string restore = seq("u");

         inline string move(int x, int y) {
            return seq(to_string(y) + ";" + to_string(x) + "H");
         }

         inline string up(int n) {
            return seq(to_string(n) + "A");
         }

         inline string down(int n) {
            return seq(to_string(n) + "B");
         }

         inline string right(int n) {
            return seq(to_string(n) + "C");
         }

         inline string left(int n) {
            return seq(to_string(n) + "D");
         }
      }

      namespace screen {
         struct Size {
            int width;
            int height;
         };

         inline Size get_size() {
            Size size;

#ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            int columns, rows;

            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            size.width = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            size.height = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
            struct winsize winsz;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);
            size.width = winsz.ws_col;
            size.height = winsz.ws_row;
#endif
            return size;
         }
      }
   }
}

#endif
