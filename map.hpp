#ifndef MAP_HPP
#define MAP_HPP

#include <unordered_map>
#include <memory>

namespace Noz {

template<
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Allocator = std::allocator<std::pair<const Key, T>>
>
using Map = let<std::shared_ptr<std::unordered_map<Key, T, Hash, Pred, Allocator>>>;

template<
    typename T,
    typename Hash = std::hash<T>,
    typename Pred = std::equal_to<T>,
    typename Allocator = std::allocator<const T>
>
using Set = let<std::shared_ptr<std::unordered_set<T, Hash, Pred, Allocator>>>;

} //namespace Noz

#endif // MAP_HPP
