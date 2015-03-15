#include <iostream>
#include <shared_mutex>
#include "lain/testing.h"

using namespace std;
using namespace lain::testing;

int main(int argc, char** argv) {
   return TestSuite("C++14 shared_mutex tests")
      .test("Test unique_lock on shared_timed_mutex with live shared_locks", [&]()->bool {
         shared_lock<shared_timed_mutex> sharedLock(mutex);
         unique_lock<shared_timed_mutex> uniqueLock(mutex);
         cout << "Unique lock acquired while shared lock active." << endl;
         return true;
      })
      .run();
}
