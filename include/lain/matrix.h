#ifndef __LAIN_MATRIX_H
#define __LAIN_MATRIX_H

#include "exception.h"
#include <cstdlib>
#include <cassert>
#include <functional>

namespace lain {
   using namespace std;
   
   /**
    * A wrapper template class for a plain vector providing
    * 2d access semantics and preallocation of a static sized
    * matrix.
    */
   template<class T>
   class Matrix {
   public:
      /**
       * Construct a matrix with the given width and height.
       */
      Matrix(int width_in, int height_in,
             const T& default_val) :
         Matrix(width_in, height_in, [=](int x, int y) { return default_val; }) {}

      /**
       * Construct a matrix with the given width and height.
       */
      Matrix(int width_in, int height_in,
             function<T(int, int)> default_f = [](int x, int y) {return T();}) :
         _width(width_in), _height(height_in), default_f(default_f) {
         vec.resize(_width * _height);
         clear();
      }

      virtual ~Matrix() { }
   
      /**
       * Get a mutable reference to the element at (x, y).
       */
      T& at(int x, int y)  {
         assert(y * _width + x < size());
         return vec[y * _width + x];
      }
      
      /**
       * Get a const reference to the element at (x, y).
       */
      const T& at(int x, int y) const {
         assert(y * _width + x < size());
         return vec[y * _width + x];
      }

      void scan(function<bool(int, int, T&)> f) {
         for (int y = 0; y < _height; y++) {
            for (int x = 0; x < _width; x++) {
               if (! f(x, y, vec[y * _width + x])) {
                  return;
               }
            }
         }
      }
      
      /**
       * Resize the matrix so that it is the given width and height.
       */
      Matrix<T> resize(int width_new, int height_new) const {
         Matrix<T> m_new(width_new, height_new, default_f);

         for (int y = 0; y < _height && y < height_new; y++) {
            for (int x = 0; x < _width && x < width_new; x++) {
               m_new.at(x, y) = at(x, y);
            }
         }

         return m_new;
      }

      void clear() {
         for (int y = 0; y < _height; y++) {
            for (int x = 0; x < _width; x++) {
               vec[y * _width + x] = default_f(x, y);
            }
         }
      }

      int width() const {
         return _width;
      }

      int height() const {
         return _height;
      }

      int size() const {
         return vec.size();
      }

   private:
      int _width, _height;
      function<T(int, int)> default_f;
      vector<T> vec;
   };
}

#endif
