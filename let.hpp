#ifndef LET_HPP
#define LET_HPP

#include "primitive_type.hpp"

namespace Noz {

template<typename type>
class let;

template<typename T>
auto swap(let<T> & a, T & b) noexcept
-> void
{
    std::swap(a.value, b);
}

template<typename T>
auto swap(T & a, let<T> & b) noexcept
-> void
{
    std::swap(a, b.value);
}

template<typename T>
auto swap(T & a, T & b) noexcept
-> void
{
    std::swap(a, b);
}

template<typename type>
let(type &&)->let<primitive_type_t<std::decay_t<type &&>>>;

} // namespace Noz

#endif // LET_HPP
