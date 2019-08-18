#ifndef IS_SHARED_PTR_HPP
#define IS_SHARED_PTR_HPP

#include <memory>

namespace Noz {

template<typename>
struct is_shared_ptr_impl : std::false_type {};
template<typename type>
struct is_shared_ptr_impl<std::shared_ptr<type>> : std::true_type {};
template<typename T>
constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

}

#endif // IS_SHARED_PTR_HPP
