/*
 * getopt.hpp: Header-only Simplified Option Parser
 *
 * Author: Lain Lee Supe (lainproliant)
 * Date: Fri August 29, 2014
 */
#ifndef __LAIN_GETOPT_H
#define __LAIN_GETOPT_H

#include <set>
#include <map>
#include <string>
#include <iostream>

#include "Optional/optional.hpp"
#include "lain/exception.h"
#include "lain/string.h"

namespace lain {
   namespace getopt {
      using namespace std;
      using namespace lain;
      using namespace std::experimental;

      const string EMPTY_STRING = "";
      const list<string> EMPTY_STRING_LIST = {};

      /**
       * A base exception for invalid command line options.
       */
      class ArgvException : public Exception {
      public:
         using Exception::Exception;
      };

      class MissingParameterException : public ArgvException {
      public:
         MissingParameterException(const string& opt_str) :
            ArgvException(string("No parameter specified for option '--" +
                     opt_str + "'.")),
            option(string("--") + opt_str) { }

         MissingParameterException(const char& opt_c) :
            ArgvException(string("No parameter specified for option '-" +
                     str::char_to_str(opt_c) + "'.")),
            option(string("-") + str::char_to_str(opt_c)) { }

         const string& get_option() {
            return option;
         }

      private:
         string option;
      };

      class UndefinedOptionException : public ArgvException {
      public:
         UndefinedOptionException(const string& opt_str) :
            ArgvException(string("Option '--" +
                     opt_str + "' is undefined.")),
            option(string("--") + opt_str) { }

         UndefinedOptionException(const char& opt_c) :
            ArgvException(string("Option '-" +
                     str::char_to_str(opt_c) + "' is undefined.")),
            option(string("-") + str::char_to_str(opt_c)) { }

         const string& get_option() {
            return option;
         }

      private:
         string option;
      };

      struct Option {
         Option() : repr(""), has_param(false) {}

         string repr;
         bool has_param;
      };

      enum parser_mode_t {
         PARSE_INIT,
         PARSE_ARGUMENT,
         PARSE_SHORTOPT,
         PARSE_LONGOPT
      };

      inline ostream& operator<<(ostream& out, const Option& opt) {
         out << "Option(\"repr\"=>" << opt.repr << ", "
            << "\"has_param\"=>" << str::bool_repr(opt.has_param) << ")";
         return out;
      }

      class OptionParser {
      public:
         /**
          * Constructs an option parser.
          *
          * @param shortops A string containing definitions of short options.
          *    Short options are specified by a single character.  If the
          *    option requires a parameter, this may be specified by
          *    following the character with a colon (':').  Optional
          *    parameters are not supported by this class.
          * @param longopts A vector of strings specifying the long options
          *    which are available.  Long options are an alphanumeric string.
          *    If the option requires a parameter, this may be specified
          *    by ending the string with an equals sign ('=').  Optional
          *    parameters are not supported by this class.
          */
         OptionParser(const string& shortopts,
               const vector<string>& longopts,
               bool strict = false) {
            _parse_shortopts(shortopts);
            _parse_longopts(longopts);
            this->strict = strict;
         }

         /**
          * Copy constructor.  Creates a new copy of this option parser minus
          * any cached argument parsing which may have been performed.
          */
         OptionParser(const OptionParser& parser) {
            shortopts = parser.shortopts;
            longopts = parser.longopts;
         }

         /** Virtual destructor. */
         virtual ~OptionParser() {}

         /**
          * Create a new parser inheriting the short and long options of this one,
          * plus the additional ones specified below.  Option specifications in the
          * old OptionParser have precedence over the new ones and cannot be overridden
          * or removed.
          *
          * @param shortops A string containing definitions of short options.
          *    Short options are specified by a single character.  If the
          *    option requires a parameter, this may be specified by
          *    following the character with a colon (':').  Optional
          *    parameters are not supported by this class.
          * @param longopts A vector of strings specifying the long options
          *    which are available.  Long options are an alphanumeric string.
          *    If the option requires a parameter, this may be specified
          *    by ending the string with an equals sign ('=').  Optional
          *    parameters are not supported by this class.
          */
         OptionParser inherit(const string& shortopts, const vector<string>& longopts) const {
            OptionParser new_parser(*this);
            new_parser._parse_shortopts(shortopts);
            new_parser._parse_longopts(longopts);

            return new_parser;
         }

         /**
          * Determine if a longopt was provided.
          *
          * @param opt The option to check for.
          * @return True if the option was provided, false otherwise.
          */
         bool check_option(const string& opt) const {
            return longopts_specified.find(opt) != longopts_specified.end();
         }

         /**
          * Determine if a shortopt was provided.
          *
          * @param opt The option to check.
          * @return True if the option was provided, false otherwise.
          */
         bool check_option(const char& opt_c) const {
            return shortopts_specified.find(opt_c) != shortopts_specified.end();
         }

         /**
          * Determine if the given longopt or shortopt were specified.
          *
          * @param opt The longopt version of the option to check.
          * @param opt_c The shortopt version of the option to check.
          * @return True if the option was provided, false otherwise.
          */
         bool check_option(const string& opt, const char& opt_c) const {
            return check_option(opt) || check_option(opt_c);
         }

         /**
          * Determine the number of times the given longopt was specified.
          *
          * @param opt The option to check.
          * @return The number of times the option was provided, or 0
          *    if the option was not provided or is not defined.
          */
         int get_count(const string& opt) const {
            auto iter = longopts_specified.find(opt);
            if (iter != longopts_specified.end()) {
               return iter->second;
            } else {
               return 0;
            }
         }

         /**
          * Determine the number of times the given shortopt was specified.
          *
          * @param opt_c The option to check.
          * @return The number of times the option was provided, or 0
          *    if the option was not provided or is not defined.
          */
         int get_count(const char& opt_c) const {
            auto iter = shortopts_specified.find(opt_c);
            if (iter != shortopts_specified.end()) {
               return iter->second;
            } else {
               return 0;
            }
         }

         /**
          * Determine the number of times the given longopt and shortopt
          * were specified.
          *
          * @param opt The longopt version of the option to check.
          * @param opt_c The shortopt version of the option to check.
          * @return The number of times the option was provided, or 0
          *    if the option was not provided or is not defined.
          */
         int get_count(const string& opt, const char& opt_c) const {
            return get_count(opt) + get_count(opt_c);
         }

         /**
          * Return the program name as reported by the shell, this ends
          * up being argv[0] from the main() function.
          */
         const string& get_program_name() const {
            return program_name;
         }

         /**
          * Fetch the list of non-option arguments provided.
          *
          * @return The vector of non-option arguments provided, or an empty
          *    vector if no options were provided.
          */
         const vector<string>& get_args() const {
            return args;
         }

         /**
          * Fetch the parameter for the option provided.  Only one parameter
          * per option is recorded, that being the last one provided.
          *
          * If the option wasn't provided or doesn't accept parameters, an
          * empty string will be returned. Users should first call
          * check_option() to determine if the option was specified before
          * calling this function for reliable results.
          *
          * @param opt The option for which to fetch the parameter.
          * @return The parameter provided for the option, or an empty string
          *    if the parameter is not defined, was not provided, or does
          *    not accept a parameter.
          */
         template <class T>
         const string& get_param(const T& opt_token) const {
            const list<string>& params = get_param_list(opt_token);
            if (params.size() > 0) {
               return params.front();
            } else {
               return EMPTY_STRING;
            }
         }

         /**
          * Fetch the parameter for the option provided.  Only one parameter
          * per option is recorded, that being the last one provided.
          *
          * @param opt The option for which to fetch the parameter.
          * @return The parameter provided for the option.
          * @throws MissingParameterException if the option wasn't provided
          *    or doesn't accept parameters.
          */
         template <class T>
         const string& get_required_param(const T& opt_token) const {
            const string& param = get_param(opt_token);
            if (param == EMPTY_STRING) {
               throw MissingParameterException(opt_token);
            }

            return param;
         }

         /**
          * Fetch the parameter for the given long or shortopt provided.
          * Only one parameter per option is recorded, that being the last
          * one provided.  If a parameter is provided for the longopt and
          * the shortopt, the longopt parameter is preferred.
          *
          * If the option wasn't provided or doesn't accept parameters, an
          * empty string will be returned. Users should first call
          * check_option() to determine if the option was specified before
          * calling this function for reliable results.
          *
          * @param opt The longopt version of the option for which to fetch
          *    the parameter.
          * @param opt_c The shortopt version of the option for which to
          *    fetch the parameter.
          * @return The parameter provided for the option, or an empty string
          *    if the parameter is not defined, was not provided, or does
          *    not accept a parameter.
          */
         string get_param(const string& opt, const char& opt_c) const {
            if (check_option(opt)) {
               return get_param(opt);
            } else {
               return get_param(opt_c);
            }
         }

         /**
          * Fetch the parameter for the given long or shortopt provided.
          * Only one parameter per option is recorded, that being the last
          * one provided.  If a parameter is provided for the longopt and
          * the shortopt, the longopt parameter is preferred.
          *
          * @param opt The longopt version of the option for which to fetch
          *    the parameter.
          * @param opt_c The shortopt version of the option for which to
          *    fetch the parameter.
          * @return The parameter provided for the option, or an empty string
          *    if the parameter is not defined, was not provided, or does
          *    not accept a parameter.
          * @throws MissingParameterException if the option wasn't provided
          *    or doesn't accept parameters.
          */
         string get_required_param(const string& opt, const char& opt_c) const {
            string param = get_param(opt);

            if (param == EMPTY_STRING) {
               param = get_param(opt_c);
            }

            if (param == EMPTY_STRING) {
               throw MissingParameterException(opt + "/-" + opt_c);
            }

            return param;
         }

         /**
          * Fetch the list of parameters which were specified for
          * the given longopt.
          *
          * @param opt The longopt which was parameterized.
          * @return A list of strings, or an empty list if the
          *    longopt was not specified.
          */
         const list<string>& get_param_list(const string& opt) const {
            auto iter = longopts_values.find(opt);
            if (iter != longopts_values.end()) {
               return iter->second;
            } else {
               return EMPTY_STRING_LIST;
            }
         }

         /**
          * Fetch the list of parameters which were specified for
          * the given shortopt.
          *
          * @param opt_c The shortopt which was parameterized.
          * @return A list of strings, or an empty list if the
          *    shortopt was not specified.
          */
         const list<string>& get_param_list(const char& opt_c) const {
            auto iter = shortopts_values.find(opt_c);
            if (iter != shortopts_values.end()) {
               return iter->second;
            } else {
               return EMPTY_STRING_LIST;
            }
         }

         /**
          * Fetch the list of parameters which were specified for
          * the given longopt or shortopt.  Parameters specified for
          * the longopt and shortopt are combined, with longopt params
          * occurring before shortopt params in the result list.
          *
          * @param opt_c The shortopt which was parameterized.
          * @return A list of strings, or an empty list if the
          *    shortopt was not specified.
          */
         list<string> get_param_list(const string& opt, const char& opt_c) const {
            list<string> result_list;
            const list<string>& longopt_list = get_param_list(opt);
            const list<string>& shortopt_list = get_param_list(opt_c);

            result_list.insert(result_list.end(), longopt_list.begin(),
                               longopt_list.end());
            result_list.insert(result_list.end(), shortopt_list.begin(),
                               shortopt_list.end());

            return result_list;
         }

         /**
          * Parse the given arguments into the option parser.
          *
          * @param argc The number of elements in the argv array, as is
          *    provided to main().
          * @param argv An array of string pointers, as is provided by main().
          */
         OptionParser& parse(int argc, char** argv) {
            return parse(str::argv_to_vector(argc, argv));
         }

         /**
          * Parse the argument list provided.
          *
          * Any previously parsed options are cleared.
          */
         OptionParser& parse(const vector<string>& argv) {
            _clear();

            if (argv.size() < 1) {
               throw ArgvException("argv must have at least one entry (program name).");
            }

            _program_name_specified(argv[0]);

            for (size_t x = 1; x < argv.size(); x++) {
               parser_mode_t parser_mode = PARSE_INIT;
               string token = EMPTY_STRING;
               string optarg = argv[x];

               for (size_t y = 0; y < optarg.length(); y++) {
                  switch (parser_mode) {
                     case PARSE_INIT:
                        if (y == 0 && optarg[y] == '-') {
                           // optarg is likely a shortopt or longopt,
                           // start with shortopt.
                           parser_mode = PARSE_SHORTOPT;

                        } else {
                           // optarg is a free argument;
                           parser_mode = PARSE_ARGUMENT;
                           token.push_back(optarg[y]);
                        }
                        break;

                     case PARSE_ARGUMENT:
                        token.push_back(optarg[y]);
                        break;

                     case PARSE_SHORTOPT:
                        if (y == 1 && optarg[y] == '-') {
                           // We're actually parsing a longopt.
                           parser_mode = PARSE_LONGOPT;

                        } else {
                           optional<const Option&> opt = _get_opt(optarg[y]);
                           if (opt) {
                              if (opt->has_param) {
                                 if (y + 1 >= optarg.length() &&
                                       x + 1 < argv.size()) {
                                    _opt_specified(optarg[y], argv[++x]);

                                 } else {
                                    throw MissingParameterException(optarg[y]);
                                 }

                              } else {
                                 _opt_specified(optarg[y]);
                              }
                           }
                        }
                        break;

                     case PARSE_LONGOPT:
                        token.push_back(optarg[y]);
                        break;

                     default:
                        throw ArgvException("Invalid argv parser state.");
                  }
               }

               if (parser_mode == PARSE_LONGOPT) {
                  optional<const Option&> opt = _get_opt(token);
                  if (opt) {
                     if (opt->has_param) {
                        if (x + 1 < argv.size()) {
                           _opt_specified(token, argv[++x]);

                        } else {
                           throw MissingParameterException(token);
                        }
                     } else {
                        _opt_specified(token);
                     }
                  }

               } else if (parser_mode == PARSE_ARGUMENT) {
                  _arg_provided(token);
               }
            }

            return *this;
         }

      protected:
         void _parse_shortopts(const string& shortopts_str) {
            for (auto iter = shortopts_str.begin();
                  iter != shortopts_str.end();
                  iter++) {
               Option opt;
               string repr;
               auto next = iter + 1;

               repr.push_back(*iter);
               opt.repr = repr;

               if (next != shortopts_str.end() && *next == ':') {
                  opt.has_param = true;
               }

               // Only add the option to the list if there isn't
               // already one defined for this character,
               // regardless of whether or not this new one
               // specifies an option parameter.
               if (shortopts.find(*iter) == shortopts.end()) {
                  shortopts[*iter] = opt;
               }

               if (opt.has_param)
                  iter++;
            }
         }

         void _parse_longopts(const vector<string>& longoptsIn) {
            for (string opt_str : longoptsIn) {
               Option opt;
               string repr;

               if (opt_str.length() > 2 &&
                     opt_str.compare(opt_str.length() - 1, 1, "=") == 0) {
                  opt.has_param = true;
                  opt.repr = opt_str.substr(0, opt_str.length() - 1);

               } else {
                  opt.repr = opt_str;
               }

               // Only add the option to the list if there isn't
               // already one defined for this word, regardless
               // of whether or not this new one specifies an
               // option parameter.
               if (longopts.find(opt.repr) == longopts.end()) {
                  longopts[opt.repr] = opt;
               }
            }
         }

         bool _is_option_defined(const string& opt_str) const {
            return longopts.find(opt_str) != longopts.end();
         }

         bool _is_option_defined(const char& opt_c) const {
            return shortopts.find(opt_c) != shortopts.end();
         }

         optional<const Option&> _get_opt(const string& opt_str) const {
            auto iter = longopts.find(opt_str);
            if (iter != longopts.end()) {
               return iter->second;

            } else if (strict) {
               throw UndefinedOptionException(opt_str);
            }

            return nullopt;
         }

         optional<const Option&> _get_opt(const char& opt_c) const {
            auto iter = shortopts.find(opt_c);
            if (iter != shortopts.end()) {
               return iter->second;
            } else if (strict) {
               throw UndefinedOptionException(opt_c);
            }

            return nullopt;
         }

         void _opt_specified(const string& opt_str) {
            longopts_specified[opt_str] ++;
         }

         void _opt_specified(const char& opt_c) {
            shortopts_specified[opt_c] ++;
         }

         void _opt_specified(const string& opt_str, const string& value) {
            _opt_specified(opt_str);
            longopts_values[opt_str].push_back(value);
         }

         void _opt_specified(const char& opt_c, const string& value) {
            _opt_specified(opt_c);
            shortopts_values[opt_c].push_back(value);
         }

         void _program_name_specified(const string& program_name) {
            this->program_name = program_name;
         }

         void _arg_provided(const string& arg) {
            args.push_back(arg);
         }

         void _clear() {
            shortopts_specified.clear();
            longopts_specified.clear();
            shortopts_values.clear();
            longopts_values.clear();
            args.clear();
         }

      private:
         map<char, Option> shortopts;
         map<string, Option> longopts;

         map<char, int> shortopts_specified;
         map<string, int> longopts_specified;
         map<char, list<string>> shortopts_values;
         map<string, list<string>> longopts_values;

         vector<string> args;

         string program_name;

         bool strict;
      };
   }
}

#endif
