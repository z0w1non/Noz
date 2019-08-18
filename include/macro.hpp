#ifndef MACRO_HPP
#define MACRO_HPP

#define forward_member_function(memfun) \
    /* Override non-const member function that return lreference */ \
        template<typename ... Args> \
        auto memfun(Args && ... args) \
            -> std::enable_if_t< \
                std::is_same_v<decltype(std::declval<std::basic_string<CharT, Traits, Allocator>>().memfun(std::forward<Args>(args) ...)), std::basic_string<CharT, Traits, Allocator> &>, \
                let<string_tag<CharT, Traits, Allocator>> & \
            > \
        { \
            using namespace std; \
            auto temp_ptr = std::make_shared<std::basic_string<CharT, Traits, Allocator>>(*ptr); \
            temp_ptr->memfun(std::forward<Args>(args) ...); \
            auto temp_const_ptr = std::static_pointer_cast<const std::basic_string<CharT, Traits, Allocator>>(temp_ptr); \
            swap(ptr, temp_const_ptr); \
            return *this; \
        } \
    \
    /* Override non-const member function that return value */ \
    template<typename ... Args> \
    auto memfun(Args && ... args) \
        -> std::enable_if_t< \
            std::is_same_v<decltype(std::declval<std::basic_string<CharT, Traits, Allocator>>().memfun(std::forward<Args>(args) ...)), std::basic_string<CharT, Traits, Allocator>>, \
            let<string_tag<CharT, Traits, Allocator>> \
        > \
    { \
        return ptr->memfun(std::forward<Args>(args) ...); \
    } \
    \
    /* Override const member function that return value type */ \
    template<typename ... Args> \
    auto memfun(Args && ... args) const \
        ->std::enable_if_t< \
            std::is_same_v<std::decay_t<decltype(std::declval<const std::basic_string<CharT, Traits, Allocator>>().memfun(std::forward<Args>(args) ...))>, std::basic_string<CharT, Traits, Allocator>>, \
            let<string_tag<CharT, Traits, Allocator>> \
        > \
    { \
        return ptr->memfun(std::forward<Args>(args) ...); \
    } \
    \
    /* Override const member function that return other type */ \
    template<typename ... Args> \
    auto memfun(Args && ... args) const \
        ->std::enable_if_t< \
            !std::is_same_v<std::decay_t<decltype(std::declval<const std::basic_string<CharT, Traits, Allocator>>().memfun(std::forward<Args>(args) ...))>, std::basic_string<CharT, Traits, Allocator>>, \
            decltype(std::declval<const std::basic_string<CharT, Traits, Allocator>>().memfun(std::forward<Args>(args) ...)) \
        > \
    { \
        return ptr->memfun(std::forward<Args>(args) ...); \
    } \
//define forward_member_function

#endif // MACRO_HPP
