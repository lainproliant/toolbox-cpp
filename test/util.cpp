#include "lain/matrix.h"
#include "lain/testing.h"
#include "lain/util.h"
#include "lain/macros.h"

using namespace std;
using namespace lain;
using namespace lain::testing;

int main(int argc, char** argv) {
   UNUSED(argc); UNUSED(argv);

   return TestSuite("util (util.h) tests")
      .test("Finalizers", [&]()->bool {
         string message = "incorrect";

         function<void()> f = [&]() {
            Finalizer finally([&]() {
               message = "correct";
            });
            
            throw Exception("oops");
            message = "also incorrect";
         };
         
         try {
            f();

         } catch (Exception e) {}

         assert_equal(message, string("correct"));

         return true;
      })
      .run();
}
