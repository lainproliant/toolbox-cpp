#ifndef __LAIN_SHARED_PTR_H
#define __LAIN_SHARED_PTR_H

#include <memory>

namespace lain {
   class polymorphic_shared_from_this_base : public std::enable_shared_from_this<polymorphic_shared_from_this_base> {
   public:
      virtual ~polymorphic_shared_from_this_base() { }
   };

   template <class T>
   class polymorphic_shared_from_this : virtual public polymorphic_shared_from_this_base {
   public:
      std::shared_ptr<T> shared_from_this() {
         return std::dynamic_pointer_cast<T>(polymorphic_shared_from_this_base::shared_from_this());
      }

      template <class D>
      std::shared_ptr<D> poly_shared_from_this() {
         return std::dynamic_pointer_cast<D>(polymorphic_shared_from_this_base::shared_from_this());
      }
   };
}

#endif
