#include <iostream>
#include <fstream>

#include "lain/testing.h"
#include "picojson/picojson.h"

using namespace std;
using namespace lain::testing;

namespace pj = picojson;

int main(int argc, char** argv) {
   return TestSuite("PicoJson Tests")
      .test("Loading a file and reading values.", [&]()->bool {
         ifstream infile;
         pj::value json;
         pj::object json_obj;
         pj::value project;
         pj::object project_obj;
         pj::value version;
         pj::value awesome_ratio;
         pj::value tasks;
         pj::array tasks_array;

         string error;

         infile.open("picojson-test.json");
         infile >> json;

         error = pj::get_last_error();
         if (! error.empty()) {
            cerr << "ERROR: " << error << endl;
            return false;
         }
         
         assert(json.is<pj::object>(), "json is an object");
         json_obj = json.get<pj::object>();
         project = json_obj["project"];
         assert(json.is<pj::object>(), "project is an object");
         project_obj = project.get<pj::object>();
         version = project_obj["version"];
         assert(version.is<double>(), "version is a double");
         awesome_ratio = project_obj["awesome_ratio"];
         assert(awesome_ratio.is<double>(), "awesome_ratio is a double");
         tasks = project_obj["tasks"];
         assert(tasks.is<pj::array>(), "tasks is an array");
         tasks_array = tasks.get<pj::array>();
         
         cerr << "OBJ DUMP: " << json << endl;
      })
      .run();
}
