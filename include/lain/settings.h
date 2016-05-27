/*
 * Settings: A simplified wrapper class around picojson.
 *
 * Motivation: By not supporting hetergenous lists or
 * standalone non-object values, we can greatly simplify
 * the interface by which C++ code reads and writes
 * JSON data.  The original motivation for this module
 * was to allow picojson to be used for app settings.
 *
 * Author: Lain Supe (lainproliant)
 * Date: Wednesday, Jan 14 2015
 */
#pragma once
#include <fstream>
#include <memory>

#include "lain/maps.h"
#include "lain/exception.h"
#include "tinyformat/tinyformat.h"
#include "picojson/picojson.h"

namespace lain {
   using namespace std;
   namespace pj = picojson;

   class SettingsException : public Exception {
   public:
      using Exception::Exception;
   };

   namespace json_impl {
      template <class T>
      void set_value(pj::value& obj_value, const string& name, const T& value) {
         pj::object& obj = obj_value.get<pj::object>();
         pj::value json_value(value);
         obj[name] = json_value;
      }

      template <class T>
      T get_value(const pj::value& obj_value, const string& name) {
         if (! obj_value.contains(name)) {
            throw SettingsException(tfm::format(
               "Missing value for key '%s'.", name));
         }

         pj::value value = obj_value.get(name);
         if (! value.is<T>()) {
            throw SettingsException(tfm::format(
               "Unexpected value type for key '%s'.", name));
         }

         return value.get<T>();
      }

      template <class T>
      T get_value_or_default(const pj::value& obj_value,
                             const string& name,
                             const T& default_value) {
         try {
            return get_value<T>(obj_value, name);

         } catch (const SettingsException& e) {
            return default_value;
         }
      }

      template <class T>
      T get_value(pj::value& obj_value, const string& name, const T& default_value) {
         try {
            return get_value<T>(obj_value, name);

         } catch (const SettingsException& e) {
            set_value(obj_value, name, default_value);
            return default_value;
         }
      }

      template <class T>
      void set_array(pj::value& obj_value, const string& name, const vector<T>& vec) {
         pj::object& obj = obj_value.get<pj::object>();
         pj::array array;

         for (T val : vec) {
            array.push_back(pj::value(val));
         }

         obj[name] = pj::value(array);
      }

      template<class T>
      vector<T> get_array(const pj::value& obj_value, const string& name) {
         vector<T> vec;

         if (! obj_value.contains(name)) {
            throw SettingsException(tfm::format(
               "Missing array for key '%s'.", name));
         }

         const pj::value& array_value = obj_value.get(name);
         if (! array_value.is<pj::array>()) {
            throw SettingsException(tfm::format("Key '%s' does not refer to an array.", name));
         }

         const pj::array& array = array_value.get<pj::array>();
         for (pj::value val : array) {
            if (! val.is<T>()) {
               throw SettingsException(tfm::format(
                  "Unexpected heterogenous value type in array for key '%s'.", name));
            }

            vec.push_back(val.get<T>());
         }

         return vec;
      }

      template <class T>
      vector<T> get_array(pj::value& obj_value, const string& name, const vector<T>& default_array) {
         try {
            return get_array<T>(obj_value, name);

         } catch (const SettingsException& e) {
            cerr << e.get_message() << endl;
            set_array(name, default_array);
            return default_array;
         }
      }

      template <>
      inline int get_value<int>(const pj::value& obj_value, const string& name) {
         return (int)get_value<double>(obj_value, name);
      }

      template <>
      inline int get_value<int>(pj::value& obj_value, const string& name, const int& default_value) {
         return (int)(get_value<double>(obj_value, name,
                  (double)(default_value)));
      }

      template <>
      inline void set_array<int>(pj::value& obj_value, const string& name, const vector<int>& vec) {
         vector<double> db_vec;
         copy(vec.begin(), vec.end(), back_inserter(db_vec));
         set_array<double>(obj_value, name, db_vec);
      }

      template <>
      inline vector<int> get_array<int>(const pj::value& obj_value, const string& name) {
         vector<int> vec;
         vector<double> db_vec = get_array<double>(obj_value, name);
         copy(db_vec.begin(), db_vec.end(), back_inserter(vec));
         return vec;
      }

      template <>
      inline vector<int> get_array<int>(pj::value& obj_value, const string& name, const vector<int>& default_vec) {
         try {
            return get_array<int>(obj_value, name);

         } catch (const SettingsException& e) {
            set_array(obj_value, name, default_vec);
            return default_vec;
         }
      }
   }

   /**
    * A wrapper class around picojson with a simplified, object-focused interface.
    *
    * NOTE: This class cannot represent heterogenous lists, i.e. lists must
    * contain only objects, ints, floats, strings, etc. and may not mix these
    * data types.  This allows us to fetch lists directly into C++ data types.
    * Attempting to load data that does not fit this limitation will result in
    * SettingsException being thrown.
    */
   class Settings {
   public:
      Settings() : obj_value(make_shared<pj::value>(pj::object())) { }
      Settings(const shared_ptr<pj::value>& obj_value) :
         obj_value(obj_value) { }

      virtual ~Settings() { }

      static Settings load_from_file(const string& filename) {
         ifstream infile;
         shared_ptr<pj::value> obj_value = make_shared<pj::value>();
         infile.open(filename);
         infile >> (*obj_value);

         if (! obj_value->is<pj::object>()) {
            throw SettingsException(
               tfm::format("Settings file does not contain an object: '%s'", filename));
         }

         return Settings(obj_value);
      }

      void save_to_file(const string& filename, bool prettify = false) const {
         ofstream outfile;
         outfile.open(filename);
         print(outfile, prettify);
      }

      void print(ostream& outfile, bool prettify = false) const {
         outfile << to_string(prettify);
      }

      string to_string(bool prettify = false) const {
         return obj_value->serialize(prettify);
      }

      bool contains(const string& name) const {
         return obj_value->contains(name);
      }

      vector<string> get_keys() const {
         return lain::maps::keys(obj_value->get<pj::object>());
      }

      template <class T>
      T get(const string& name) const {
         return json_impl::get_value<T>(*const_pointer_cast<const pj::value>(obj_value), name);
      }

      template <class T>
      T get(const string& name, const T& default_value) {
         return json_impl::get_value<T>(*obj_value, name, default_value);
      }

      template <class T>
      T get_default(const string& name, const T& default_value) const {
         return json_impl::get_value_or_default(*const_pointer_cast<const pj::value>(obj_value), name, default_value);
      }

      template <class T>
      void set(const string& name, const T& value) {
         json_impl::set_value<T>(*obj_value, name, value);
      }

      template <class T>
      vector<T> get_array(const string& name) const {
         return json_impl::get_array<T>(*obj_value, name);
      }

      template <class T>
      vector<T> get_array(const string& name, const vector<T>& default_vec) {
         return json_impl::get_array<T>(*obj_value, name, default_vec);
      }

      template <class T>
      void set_array(const string& name, const vector<T>& vec) {
         json_impl::set_array<T>(*obj_value, name, vec);
      }

      vector<Settings> get_object_array(const string& name) const {
         pj::value obj_values_array_value = obj_value->get(name);

         if (! obj_values_array_value.is<pj::array>()) {
            throw SettingsException(tfm::format("Key '%s' does not refer to an object array.", name));
         }

         pj::array& obj_values_array = obj_values_array_value.get<pj::array>();
         vector<Settings> obj_array;

         for (pj::value val : obj_values_array) {
            if (! val.is<pj::object>()) {
               throw SettingsException(tfm::format("Object array contains non-object: '%s'", name));
            }

            obj_array.push_back(Settings(make_shared<pj::value>(val)));
         }

         return obj_array;
      }

      void set_object_array(const string& name, const vector<Settings>& obj_list) {
         pj::array array;
         pj::object& obj = obj_value->get<pj::object>();

         for (Settings obj : obj_list) {
            array.push_back(*(obj.obj_value.get()));
         }

         obj[name] = pj::value(array);
      }

      Settings get_object(const string& name, bool must_exist = false) const {
         if (! obj_value->contains(name)) {
            if (must_exist) {
               throw SettingsException(tfm::format("Missing object for key '%s'.", name));

            } else {
               return Settings();
            }
         }

         pj::value object_json = obj_value->get(name);
         if (! object_json.is<pj::object>()) {
            throw SettingsException(tfm::format("Key '%s' does not refer to a object.", name));
         }

         return Settings(make_shared<pj::value>(object_json));
      }

      void set_object(const string& name, const Settings& object) {
         pj::object& obj = obj_value->get<pj::object>();
         obj[name] = (*object.obj_value);
      }

   private:
      shared_ptr<pj::value> obj_value;
   };

   namespace json_impl {
      template <>
      inline vector<Settings> get_array<Settings>(const pj::value& obj_value, const string& name) {
         vector<pj::object> obj_array = get_array<pj::object>(obj_value, name);
         vector<Settings> json_vec(obj_array.size());

         for (auto obj : obj_array) {
            json_vec.push_back(Settings(make_shared<pj::value>(obj)));
         }

         return json_vec;
      }
   }

}
