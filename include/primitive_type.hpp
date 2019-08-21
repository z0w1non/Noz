#ifndef DEFIEND_PRIMITIVE_TYPE_HPP
#define DEFIEND_PRIMITIVE_TYPE_HPP

#include "let.hpp"

namespace Noz {

template<typename type>
class let;

template<typename primitive_type>
struct primitive_type_impl {
    using type = primitive_type;
};
template<typename primitive_type>
struct primitive_type_impl<let<primitive_type>> {
    using type = typename primitive_type_impl<primitive_type>::type;
};
template<typename type>
using primitive_type_t = typename primitive_type_impl<type>::type;

}

#endif // DEFIEND_PRIMITIVE_TYPE_HPP
