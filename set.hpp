#ifndef SET_HPP
#define SET_HPP

#include <unordered_set>
#include <memory>

namespace Noz {

template<
    typename T,
    typename Hash = std::hash<T>,
    typename Pred = std::equal_to<T>,
    typename Allocator = std::allocator<const T>
>
using Set = let<std::shared_ptr<std::unordered_set<T, Hash, Pred, Allocator>>>;

} //namespace Noz

#endif // SET_HPP
