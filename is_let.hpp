#ifndef IS_LET_HPP
#define IS_LET_HPP

#include "let.hpp"

namespace Noz {

template<typename type>
class let;

template<typename>
struct is_let_impl;
template<typename type>
struct is_let_impl : std::false_type {};
template<typename ... types>
struct is_let_impl<let<types ...>> : std::true_type {};
template<typename type>
using is_let_type = typename is_let_impl<type>::type;
template<typename type>
constexpr bool is_let_v = is_let_type<type>::value;

}

#endif // IS_LET_HPP
