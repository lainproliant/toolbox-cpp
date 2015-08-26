#include "lain/json.h"
#include "lain/testing.h"

using namespace std;
using namespace lain;
using namespace lain::testing;

int main() {
   return TestSuite("toolbox json tests")
      .die_on_signal(SIGSEGV)
      .test("JSON-001: Loading settings from a file", [&]()->bool {
         JSON settings = JSON::load_from_file("json/test001.json");
         JSON graphics_settings = settings.get_object("graphics", true);
         assert_true(graphics_settings.get<int>("width") == 1920);
         assert_true(graphics_settings.get<int>("height") == 1080);
         return true;
      })
      .test("JSON-002: Using default values for settings", [&]()->bool {
         JSON settings;
         JSON graphics_settings = settings.get_object("graphics");
         int width = graphics_settings.get<int>("width", 1920);
         int height = graphics_settings.get<int>("height", 1080);
         bool fullscreen = graphics_settings.get<bool>("fullscreen", false);

         assert_true(width == 1920);
         assert_true(height == 1080);
         assert_true(fullscreen == false);
         assert_true(graphics_settings.get<int>("width") == 1920);
         assert_true(graphics_settings.get<int>("height") == 1080);

         settings.set_object("graphics", graphics_settings);
         graphics_settings = settings.get_object("graphics");
         assert_true(graphics_settings.get<int>("width") == 1920);
         assert_true(graphics_settings.get<int>("height") == 1080);
         assert_true(graphics_settings.get<bool>("fullscreen") == false);

         return true;
      })
      .test("JSON-003: Write a new settings value based on defaults", [&]()->bool {
         JSON settings;
         JSON graphics_settings = settings.get_object("graphics");

         int width = graphics_settings.get<int>("width", 1920);
         int height = graphics_settings.get<int>("height", 1080);

         assert_true(width == 1920);
         assert_true(height == 1080);
         assert_true(graphics_settings.get<int>("width") == 1920);
         assert_true(graphics_settings.get<int>("height") == 1080);

         settings.set_object("graphics", graphics_settings);
         settings.save_to_file("JSON-003.json.output");
         settings = JSON::load_from_file("JSON-003.json.output");

         graphics_settings = settings.get_object("graphics");
         assert_true(graphics_settings.get<int>("width") == 1920);
         assert_true(graphics_settings.get<int>("height") == 1080);

         return true;
      })
      .test("JSON-004: Load arrays from settings keys", [&]()->bool {
         JSON settings = JSON::load_from_file("json/test004.json");

         vector<int> integers = settings.get_array<int>("numbers");
         vector<string> strings = settings.get_array<string>("strings");

         assert_true(lists_equal(integers, {1, 2, 3, 4, 5}));
         assert_true(lists_equal(strings, {"alpha", "bravo", "charlie", "delta", "eagle"}));
         assert_false(lists_equal(integers, {1, 2, 3, 4}));
         assert_false(lists_equal(strings, {"alpha", "bravo", "charlie", "delta"}));

         return true;
      })
      .test("JSON-005: Load and save arrays with defaults", [&]()->bool {
         JSON settings;

         vector<int> integers = settings.get_array<int>("numbers", {1, 2, 3, 4, 5});
         assert_true(lists_equal(integers, {1, 2, 3, 4, 5}));
         settings.save_to_file("JSON-005.json.output");
         settings = JSON::load_from_file("JSON-005.json.output");
         integers = settings.get_array<int>("numbers");
         assert_true(lists_equal(integers, {1, 2, 3, 4, 5}));

         return true;
      })
      .test("JSON-006: Heterogenous lists throw JSONException", [&]()->bool {
         JSON settings = JSON::load_from_file("json/test006.json");
         try {
            vector<int> integers = settings.get_array<int>("numbers");

         } catch (const JSONException& e) {
            cerr << "Received expected JSONException: "
                 << e.get_message()
                 << endl;
            return true;
         }

         return false;
      })
      .test("JSON-007: Creation of settings objects with float values", [&]()->bool {
         JSON settings;
         settings.set<float>("pi", 3.14159);
         cout << settings.to_string() << endl;
         return true;
      })
      .run();
}
