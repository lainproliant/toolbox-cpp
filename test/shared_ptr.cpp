#include "lain/shared_ptr.h"
#include "lain/testing.h"

using namespace std;
using namespace lain;
using namespace lain::testing;

class Base : public polymorphic_shared_from_this<Base> {
public:
   virtual string get_type () const {
      return "Base";
   }
};

class Implementation : public Base {
public:
   virtual string get_type() const {
      return "Implementation";
   }

   shared_ptr<Implementation> self() {
      return poly_shared_from_this<Implementation>();
   }
};

int main(int argc, char** argv) {
   return TestSuite("shared_ptr utils (shared_ptr.h) tests")
      .test("shared_ptr::polymorphic_shared_from_this", [&]()->bool {

         auto impl = make_shared<Implementation>();
         shared_ptr<Implementation> self = impl->self();
         
         // The only real test is whether or not the code above compiles,
         // but let's put this here anyway.
         assert_equal(impl->get_type(), self->get_type());
         return true;
      })
      .run();
}
