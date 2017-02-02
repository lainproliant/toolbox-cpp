#ifndef __LAIN_ARGPARSE_H
#define __LAIN_ARGPARSE_H

#include <string>
#include <memory>
#include <map>
#include <iterator>

#include "exception.h"
#include "maps.h"
#include "settings.h"

namespace lain {
   using namespace std;

   class ArgumentParser;

   /*------------------------------------------------------------------------*/
   class ArgumentException : public Exception {
      using Exception::Exception;
   };

   /*------------------------------------------------------------------------*/
   struct ArgSpec {
      int id;
      char short_form;
      string long_form;
      bool optional;
      bool has_option;

      string to_string() const {
         vector<string> reprs;

         if (short_form != 0) {
            string repr = "-";
            repr.push_back(short_form);
            reprs.push_back(repr);
         }

         if (long_form != "") {
            reprs.push_back("--" + long_form);
         }

         return str::join(reprs, "/");
      }
   };

   /*------------------------------------------------------------------------*/
   class ArgumentSpecCollection {
   public:
      const ArgSpec& get(const string& arg) const {
         auto iter = long_form_map.find(arg);
         if (iter == long_form_map.end()) {
            throw Exception(tfm::format("Argument '%s' not defined.", arg)); 
         }
         return arg_specs[iter->second];
      }

      const ArgSpec& get(char arg) const {
         auto iter = short_form_map.find(arg);
         if (iter == short_form_map.end()) {
            throw Exception(tfm::format("Argument '%c' not defined.", arg));
         }
         return arg_specs[iter->second];
      }

      const vector<ArgSpec>& args() const {
         return arg_specs;
      }

      ArgumentSpecCollection& add(const ArgSpec& spec_in) {
         ArgSpec spec = spec_in;
         spec.id = arg_specs.size();
         arg_specs.push_back(spec);

         if (spec.short_form != 0) {
            short_form_map.insert({spec.short_form, spec.id});
         }

         if (spec.long_form.size() > 0) {
            long_form_map.insert({spec.long_form, spec.id});
         }

         return *this;
      }

   private:
      vector<ArgSpec> arg_specs;
      map<char, int> short_form_map;
      map<string, int> long_form_map;
   }; 

   /*------------------------------------------------------------------------*/
   class Arguments {
   public:
      Arguments(const ArgumentSpecCollection& specs) : specs(specs) { }
      virtual ~Arguments() { }

      const vector<string>& free_args() const {
         return free_args_list;
      }

      int count(char arg) const {
         return count(specs.get(arg));
      }

      int count(const string& arg) const {
         return count(specs.get(arg));
      }

      bool check(char arg) const {
         return count(arg) > 0;
      }

      bool check(const string& arg) const {
         return count(arg) > 0;
      }

      string option(char arg) const {
         return option(specs.get(arg));
      }

      string option(const string& arg) const {
         return option(specs.get(arg));
      }

      void validate() const {
         for (auto argspec : specs.args()) {
            if (! argspec.optional && count(argspec) < 1) {
               throw ArgumentException("Required argument not provided: " + argspec.to_string());
            }
         }
      }

      void set_program_name(const string& program_name) {
         this->program_name = program_name;
      }

      void add_argument(const ArgSpec& argspec, const string& opt = "") {
         arg_count_map[argspec.id] ++;
         if (argspec.has_option) {
            arg_option_map.insert({argspec.id, opt});
         }
      }

      void add_free_argument(const string& arg) {
         free_args_list.push_back(arg);
      }
   
   protected:
      int count(const ArgSpec& argspec) const {
         auto iter = arg_count_map.find(argspec.id);
         if (iter != arg_count_map.end()) {
            return iter->second;
         } else {
            return 0;
         }
      }
      
      vector<string> options(const ArgSpec& argspec) const {
         if (! argspec.has_option) {
            throw ArgumentException("Argument does not accept options: " + argspec.to_string());
         }

         vector<string> options;
         auto range = arg_option_map.equal_range(argspec.id);
         for (auto iter = range.first; iter != range.second; iter++) {
            options.push_back(iter->second);
         }
         return options;
      }

      string option(const ArgSpec& argspec) const {
         vector<string> options_list = options(argspec);
         if (options_list.size() < 1) {
            throw ArgumentException("No argument specified for argspec: " + argspec.to_string());
         }
         return options_list.back();
      }

   private:
      const ArgumentSpecCollection specs;
      string program_name;
      vector<string> free_args_list;
      map<int, int> arg_count_map;
      multimap<int, string> arg_option_map;
   };

   /*------------------------------------------------------------------------*/
   class ArgumentParser {
   public:
      ArgumentParser(const ArgumentSpecCollection& specs) : specs(specs) { }
      virtual ~ArgumentParser() { }

      Arguments parse(int argc, char** argv) const {
         return parse(str::argv_to_vector(argc, argv));
      }

      Arguments parse(const vector<string>& argv) const {
         Arguments result(specs);
         ArgIter begin = argv.begin();
         result.set_program_name(*begin);

         for (auto iter = ++begin; iter != argv.end();
              iter = parse_arg(result, iter, argv.end())) { } 

         return result;
      }

   protected:
      typedef vector<string>::const_iterator ArgIter;

      ArgIter parse_arg(Arguments& result, ArgIter cursor, ArgIter end) const {
         if (*cursor == "--") {
            // As per GNU, parse all remaining arguments as free arguments.
            while (++cursor != end) {
               result.add_free_argument(*cursor);
            }

            return end;

         } else if (str::startsWith(*cursor, "--")) {
            // This is a long opt specification.
            ArgSpec arg = specs.get((*cursor).substr(2));
            if (arg.has_option) {
               if (next(cursor) == end) {
                  throw ArgumentException("Missing option for parameter: " +
                     arg.to_string());
               }

               result.add_argument(arg, *(++cursor));

            } else {
               result.add_argument(arg);
            }
            
         } else if (str::startsWith(*cursor, "-")) {
            // This is a list of one or more short opts.
            if ((*cursor).size() > 1) {
               const string& shortopts = *cursor;
               for (size_t x = 1; x < shortopts.size(); x++) {
                  ArgSpec arg = specs.get(shortopts[x]);
                  if (arg.has_option) {
                     if (x + 1 < shortopts.size() || next(cursor) == end) {
                        throw ArgumentException("Missing option for parameter: " +
                           arg.to_string());
                     }

                     result.add_argument(arg, *(++cursor));

                  } else {
                     result.add_argument(arg);                     
                  }
               }

            } else {
               result.add_free_argument(*cursor);
            }

         } else {
            result.add_free_argument(*cursor);
         }

         return ++cursor;
      }

   private:
      const ArgumentSpecCollection specs;
   };

   class ArgumentBuilder {
   public:
      ArgumentBuilder arg(char short_form, const string& long_form,
                           bool optional = true, bool has_option = false) {
         ArgSpec spec;
         spec.short_form = short_form;
         spec.long_form = long_form;
         spec.optional = optional;
         spec.has_option = has_option;
         argspecs.add(spec);
         return *this;
      }

      ArgumentSpecCollection specs() const {
         return argspecs;
      }

      ArgumentParser parser() const {
         return ArgumentParser(argspecs);
      }

      Arguments parse(int argc, char** argv) const {
         return parser().parse(argc, argv);
      }

      Arguments parse(const vector<string>& argv) const {
         return parser().parse(argv);
      }

   private:
      ArgumentSpecCollection argspecs;
   };
}

#endif
