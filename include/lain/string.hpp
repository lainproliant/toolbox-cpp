#ifndef __LAIN_STRING_H
#define __LAIN_STRING_H

#include <sstream>
#include <list>
#include <vector>

namespace lain {
   namespace str {
      /**
       * Determine if one string starts with another's characters.
       *
       * @param str The string to check for a prefix.
       * @param prefix The prefix to check for.
       * @return true if the string starts with prefix, false otherwise.
       */
      inline bool startsWith(const string& str, const string& prefix) {
         return ! str.compare(0, prefix.size(), prefix);
      }

      /**
       * Determine if one string ends with another's characters.
       *
       * @param str The string to check for a suffix.
       * @param suffix The suffix to check for.
       * @return true if the string starts with prefix, false otherwise.
       */
      inline bool endsWith(const string& str, const string& suffix) {
         return suffix.length() <= str.length() &&
                ! str.compare(suffix.length() - str.length(),
                              suffix.length(), suffix);
      }

      /**
       * Join the given iterable collection into a token delimited string.
       *
       * @param coll The collection to be joined.
       * @param token The token used to join the elements.
       * @return A new string.
       */
      template<class T>
      string join(const T& coll, const string& token) {
         ostringstream sb;

         for (typename T::const_iterator i = coll.begin(); i != coll.end();
              i++) {
            if (i != coll.begin()) sb << string(token);
            sb << *i;
         }

         return sb.str();
      }
      
      /**
       * Split the given string into a list of strings based on
       * the delimiter provided, and insert them into the given
       * collection.  Collection must support push_back().
       *
       * @param tokens The collection into which the split string
       *    elements will be appended.
       * @param s The string to be split.
       * @param delimiter The delimiter used to split the string.
       */
      template <class T>
      void split(T& tokens, const string& s, const string& delimiter) {
         string::size_type from = 0;
         string::size_type to = 0;

         while (to != string::npos) {
            to = s.find(delimiter, from);
            if (from < s.size() && from != to) {
               tokens.push_back(s.substr(from, to - from));
            }

            from = to + delimiter.size();
         }
      }
      

      /**
       * Split the given string into a list of strings based on
       * the delimiter provided.
       *
       * @param s The string to be split.
       * @param delimiter The delimiter used to split the string.
       * @return The contents of the string, minus the delimiters,
       *    split along the delimiter boundaries in a linked list.
       */
      inline list<string> split(const string& s, const string& delimiter) {
         list<string> tokens;
         split(tokens, s, delimiter);
         return tokens;
      }
      
      /**
       * Convert argc and argv from main() into a vector of strings.
       *
       * @param argc The number of strings in the pointer array.
       * @param argv The pointer array, as provided to main().
       * @return A vector of STL strings containing the strings
       *    in the argv vector array.
       */
      inline vector<string> argv_to_vector(int argc, char** argv) {
         vector<string> vec;

         for (int x = 0; x < argc; x++) {
            vec.push_back(string(argv[x]));   
         }

         return vec;
      }
      
      template <class T, class V = typename T::value_type>
      string repr(const T& coll) {
         ostringstream sb;

         sb << "[";
         auto iter = coll.begin();
         while (iter != coll.end()) {
            const V& value = *iter;
            sb << value;
            iter++;
            if (iter != coll.end())
               sb << ", ";
         }
         sb << "]";

         return sb.str();
      }

      template <class M, class K = typename M::key_type,
                class V = typename M::mapped_type>
      string repr(M& map, bool pretty = false) {
         ostringstream sb;

         sb << "{";
         auto iter = map.begin();
         while (iter != map.end()) {
            if (pretty)
               sb << "\n\t";
            K key = iter->first;
            V val = iter->second;
            sb << "\"" << key << "\"=>" << val;
            iter++;
            if (iter != map.end())
               sb << ", ";
         }
         if (pretty)
            sb << "\n";
         sb << "}";

         return sb.str();
      }

      template <class T>
      string bool_repr(const T& val) {
         return val ? "true" : "false";
      }
   }
}

#endif
