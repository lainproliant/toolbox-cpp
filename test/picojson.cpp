#include <iostream>
#include <fstream>

#include "lain/testing.h"
#include "picojson/picojson.h"

using namespace std;
using namespace lain::testing;

namespace pj = picojson;

pj::value load_json(const string& filename);
   
int main(int argc, char** argv) {

   return TestSuite("PicoJson Tests")
      .test("Loading a file and reading values.", [&]()->bool {
         pj::value json;
         pj::object json_obj;
         pj::value project;
         pj::object project_obj;
         pj::value version;
         pj::value awesome_ratio;
         pj::value tasks;
         pj::array tasks_array;
         
         json = load_json("picojson-test-001.json");
         
         assert_true(json.is<pj::object>(), "json is an object");
         json_obj = json.get<pj::object>();
         project = json_obj["project"];
         assert_true(json.is<pj::object>(), "project is an object");
         project_obj = project.get<pj::object>();
         version = project_obj["version"];
         assert_true(version.is<double>(), "version is a double");
         awesome_ratio = project_obj["awesome_ratio"];
         assert_true(awesome_ratio.is<double>(), "awesome_ratio is a double");
         tasks = project_obj["tasks"];
         assert_true(tasks.is<pj::array>(), "tasks is an array");
         tasks_array = tasks.get<pj::array>();
         
         cerr << "OBJ DUMP: " << json << endl;
      })
      .test("Converting from non-object to object throws an exception.", [&]()->bool {
         pj::value json;
         pj::value obj_value;
         pj::value nonobj_value;
         pj::object obj;
         pj::object nonobj;

         json = load_json("picojson-test-002.json");

         obj = json.get<pj::object>();
         obj = obj["object"].get<pj::object>();

         try {
            nonobj = obj["non_object"].get<pj::object>();

         } catch (const runtime_error& exc) {
            cerr << "Received expected runtime_exception (" 
                 << typeid(exc).name()
                 << ") " << exc.what()
                 << endl;
            return true;
         }

         return false;
      })
      .run();
}

pj::value load_json(const string& filename) {
   ifstream infile;
   string error;
   pj::value json;

   infile.open(filename);
   infile >> json;

   error = pj::get_last_error();
   if (! error.empty()) {
      throw TestException(string("Error while loading json: ") + error);
   }

   return json;
}

