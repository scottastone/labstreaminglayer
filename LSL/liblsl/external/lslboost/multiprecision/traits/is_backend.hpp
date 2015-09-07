///////////////////////////////////////////////////////////////////////////////
//  Copyright 2015 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.lslboost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_IS_BACKEND_HPP
#define BOOST_MP_IS_BACKEND_HPP

#include <lslboost/mpl/has_xxx.hpp>
#include <lslboost/type_traits/conditional.hpp>
#include <lslboost/type_traits/is_convertible.hpp>
#include <lslboost/multiprecision/detail/number_base.hpp>

namespace lslboost{ namespace multiprecision{  namespace detail{

   BOOST_MPL_HAS_XXX_TRAIT_DEF(signed_types);
   BOOST_MPL_HAS_XXX_TRAIT_DEF(unsigned_types);
   BOOST_MPL_HAS_XXX_TRAIT_DEF(float_types);

   template <class T>
   struct is_backend
   {
      static const bool value = has_signed_types<T>::value && has_unsigned_types<T>::value && has_float_types<T>::value;
   };

   template <class Backend>
   struct other_backend
   {
      typedef typename lslboost::conditional<
         lslboost::is_same<number<Backend>, number<Backend, et_on> >::value,
         number<Backend, et_off>, number<Backend, et_on> >::type type;
   };

   template <class B, class V>
   struct number_from_backend
   {
      typedef typename lslboost::conditional <
         lslboost::is_convertible<V, number<B> >::value,
         number<B>,
         typename other_backend<B>::type > ::type type;
   };

   template <bool b, class T, class U>
   struct is_first_backend_imp{ static const bool value = false; };
   template <class T, class U>
   struct is_first_backend_imp<true, T, U>{ static const bool value = is_convertible<U, number<T, et_on> >::value || is_convertible<U, number<T, et_off> >::value; };

   template <class T, class U>
   struct is_first_backend : is_first_backend_imp<is_backend<T>::value, T, U> {};

   template <bool b, class T, class U>
   struct is_second_backend_imp{ static const bool value = false; };
   template <class T, class U>
   struct is_second_backend_imp<true, T, U>{ static const bool value = (is_convertible<T, number<U, et_on> >::value || is_convertible<T, number<U, et_off> >::value) && !is_first_backend<T, U>::value; };

   template <class T, class U>
   struct is_second_backend : is_second_backend_imp<is_backend<U>::value, T, U> {};

}
}
}

#endif // BOOST_MP_IS_BACKEND_HPP