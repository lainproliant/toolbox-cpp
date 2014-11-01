#include <iostream>
#include <fstream>

#include "lain/testing.h"
#include "picojson/picojson.h"

using namespace std;
using namespace lain::testing;

int main(int argc, char** argv) {
   return TestSuite("PicoJson Tests")
      .test("Loading a file and reading values.", [&]()->bool {
         ifstream infile;
         picojson::value json;
         picojson::object json_obj;
         picojson::value project;
         picojson::object project_obj;
         picojson::value version;
         picojson::value awesome_ratio;
         picojson::value tasks;
         picojson::array tasks_array;

         string error;

         infile.open("picojson-test.json");
         infile >> json;

         error = picojson::get_last_error();
         if (! error.empty()) {
            cerr << "ERROR: " << error << endl;
            return false;
         }
         
         assert(json.is<picojson::object>(), "json is an object");
         json_obj = json.get<picojson::object>();
         project = json_obj["project"];
         assert(json.is<picojson::object>(), "project is an object");
         project_obj = project.get<picojson::object>();
         version = project_obj["version"];
         assert(version.is<double>(), "version is a double");
         awesome_ratio = project_obj["awesome_ratio"];
         assert(awesome_ratio.is<double>(), "awesome_ratio is a double");
         tasks = project_obj["tasks"];
         assert(tasks.is<picojson::array>(), "tasks is an array");
         tasks_array = tasks.get<picojson::array>();
         
         cerr << "OBJ DUMP: " << json << endl;
      })
      .run();
}
