#include "lain/matrix.h"
#include "lain/testing.h"
#include "lain/macros.h"

#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace lain;
using namespace lain::testing;

const int BIG_MATRIX_WIDTH = 40000;
const int BIG_MATRIX_HEIGHT = 1000;

void print_matrix(const Matrix<char>& m) {
   for (int y = 0; y < m.height(); y++) {
      for (int x = 0; x < m.width(); x++) {
         cout << m.at(x, y) << ' ';
      }
      cout << endl;
   }
}

int main(int argc, char** argv) {
   UNUSED(argc); UNUSED(argv);

   return TestSuite("matrix (matrix.h) tests")
      .test("Matrix initialization", [&]()->bool {
         Matrix<char> m(5, 5, '#');

         for (int x = 0; x < m.width(); x++) {
            for (int y = 0; y < m.height(); y++) {
               m.at(x, y) = 'a' + y * m.height() + x;
            }
         }
         
         cout << "----------" << endl;
         print_matrix(m);
         assert_equal(m.at(0, 0), 'a');
         assert_equal(m.at(2, 2), 'm');
         assert_equal(m.width(), 5);
         assert_equal(m.height(), 5);

         cout << "----------" << endl;
         m = m.resize(4, 2);
         print_matrix(m);
         assert_equal(m.at(0, 0), 'a');
         assert_equal(m.width(), 4);
         assert_equal(m.height(), 2);

         cout << "----------" << endl;
         m = m.resize(5, 3);
         print_matrix(m);
         assert_equal(m.width(), 5);
         assert_equal(m.height(), 3);
         assert_equal(m.at(0, 0), 'a');
         assert_equal(m.at(4, 0), '#');
         assert_equal(m.at(0, 2), '#');
         assert_equal(m.at(3, 1), 'i');

         cout << "----------" << endl;
         m = m.resize(6, 1);
         print_matrix(m);
         assert_equal(m.width(), 6);
         assert_equal(m.height(), 1);
         assert_equal(m.at(0, 0), 'a');
         assert_equal(m.at(3, 0), 'd');
         assert_equal(m.at(4, 0), '#');
         assert_equal(m.at(5, 0), '#');

         cout << "----------" << endl;
         cout << "Resizing to " << BIG_MATRIX_WIDTH << "x" << BIG_MATRIX_HEIGHT
              << "..." << endl;
         auto start_time = steady_clock::now();
         m = m.resize(BIG_MATRIX_WIDTH, BIG_MATRIX_HEIGHT);
         int millis = duration_cast<milliseconds>(
               steady_clock::now() - start_time).count();
         cout << "Done. (" << millis << "ms)" << endl;
         assert_equal(m.at(0, 0), 'a');
         assert_equal(m.at(3, 0), 'd');
         assert_equal(m.at(4, 0), '#');
         assert_equal(m.at(5, 0), '#');
         assert_equal(m.at(BIG_MATRIX_WIDTH - 1, BIG_MATRIX_HEIGHT - 1), '#');

         return true;
      })
      .run();
}
