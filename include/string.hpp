#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <algorithm>
#include <functional>
#include <cstring>
#include <cwctype>
#include "let.hpp"

namespace Noz {

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
struct string_tag;

#define define_character_predicate(name)  \
    template<typename>                    \
    struct is_##name;                     \
    template<>                            \
    struct is_##name<char> {              \
        auto operator ()(char c) const    \
            -> bool                       \
        {                                 \
            return std::is##name(c);      \
        }                                 \
    };                                    \
    template<>                            \
    struct is_##name<wchar_t> {           \
        auto operator ()(wchar_t c) const \
            -> bool                       \
        {                                 \
            return std::isw##name(c);     \
        }                                 \
    };                                    \
// define_character_predicate
    define_character_predicate(alnum )
    define_character_predicate(alpha )
    define_character_predicate(blank )
    define_character_predicate(cntrl )
    define_character_predicate(digit )
    define_character_predicate(graph )
    define_character_predicate(lower )
    define_character_predicate(print )
    define_character_predicate(punct )
    define_character_predicate(space )
    define_character_predicate(upper )
    define_character_predicate(xdigit)
#undef define_character_predicate

template<typename CharT, typename Traits, typename Allocator>
class let<string_tag<CharT, Traits, Allocator>> {
    friend get_primitive_value_impl<let<string_tag<CharT, Traits, Allocator>>>;

#define declare_frined_function(op)                                  \
    template<typename CharT_, typename Traits_, typename Allocator_> \
    friend auto operator op (                                        \
        const let<string_tag<CharT_, Traits_, Allocator_> > & left,  \
        const let<string_tag<CharT_, Traits_, Allocator_> > & right  \
    ) -> bool;                                                       \
    template<typename CharT_, typename Traits_, typename Allocator_> \
    friend auto operator op (                                        \
        const CharT_ * left,                                         \
        const let<string_tag<CharT_, Traits_, Allocator_> &> & right \
    ) -> bool;                                                       \
    template<typename CharT_, typename Traits_, typename Allocator_> \
    friend auto operator op (                                        \
        const let<string_tag<CharT_, Traits_, Allocator_> > & left,  \
        const CharT_ * right                                         \
    ) -> bool;                                                       \
//define declare_frined_function
    declare_frined_function(==)
    declare_frined_function(!=)
    declare_frined_function(< )
    declare_frined_function(<=)
    declare_frined_function(> )
    declare_frined_function(>=)
#undef declare_frined_function

    template<typename CharT_, typename Traits_, typename Allocator_>
    friend auto operator <<(std::basic_ostream<CharT_, Traits_> & ostream, const let<string_tag<CharT_, Traits_, Allocator_>> & string)
        ->std::basic_ostream<CharT_, Traits_> &;

    template<typename CharT_, typename Traits_, typename Allocator_>
    friend auto operator >>(std::basic_istream<CharT_, Traits_> & istream, let<string_tag<CharT_, Traits_, Allocator_> > & string)
        ->std::basic_istream<CharT_, Traits_> &;

    friend struct std::hash<let<string_tag<CharT, Traits, Allocator> > >;

public:
    template<typename ... Args>
    constexpr let<string_tag<CharT, Traits, Allocator>>(Args && ... args)
        : ptr{std::make_shared<primitive_type>(std::forward<Args>(args) ...)}
    {}

    constexpr let<string_tag<CharT, Traits, Allocator>>(const let<string_tag<CharT, Traits, Allocator>> & str)
        : ptr{str.ptr}
    {}

    auto operator =(const let<string_tag<CharT, Traits, Allocator>> & str)
        -> let<string_tag<CharT, Traits, Allocator>> &
    {
        ptr = str.ptr;
        return *this;
    }

private:
    using primitive_type = std::basic_string<CharT, Traits, Allocator>;

public:
#define import_std_basic_string_function(memfun, ...)                                                              \
    /* import non-const member function that returns lreference */                                                 \
        template<typename ... Args>                                                                                \
        auto memfun(Args && ... args)                                                                              \
            -> std::enable_if_t<                                                                                   \
                std::is_same_v<                                                                                    \
                    decltype(std::declval<primitive_type>().memfun(std::forward<Args>(args) ...)),                 \
                    primitive_type &>,                                                                             \
                let<string_tag<CharT, Traits, Allocator>> &>                                                       \
        {                                                                                                          \
            using namespace std;                                                                                   \
            auto temp_ptr = std::make_shared<primitive_type>(*ptr);                                                \
            temp_ptr->memfun(std::forward<Args>(args) ...);                                                        \
            auto temp_const_ptr = std::static_pointer_cast<const primitive_type>(temp_ptr);                        \
            swap(ptr, temp_const_ptr);                                                                             \
            return *this;                                                                                          \
        }                                                                                                          \
                                                                                                                   \
    /* import non-const member function that returns value */                                                      \
    template<typename ... Args>                                                                                    \
    auto memfun(Args && ... args)                                                                                  \
        -> std::enable_if_t<                                                                                       \
            std::is_same_v<                                                                                        \
                decltype(std::declval<primitive_type>().memfun(std::forward<Args>(args) ...)),                     \
                primitive_type>,                                                                                   \
            let<string_tag<CharT, Traits, Allocator>>>                                                             \
    {                                                                                                              \
        return ptr->memfun(std::forward<Args>(args) ...);                                                          \
    }                                                                                                              \
                                                                                                                   \
    /* import const member function that returns value type */                                                     \
    template<typename ... Args>                                                                                    \
    auto memfun(Args && ... args) const                                                                            \
        ->std::enable_if_t<                                                                                        \
            std::is_same_v<                                                                                        \
                std::decay_t<decltype(std::declval<const primitive_type>().memfun(std::forward<Args>(args) ...))>, \
                primitive_type>,                                                                                   \
            let<string_tag<CharT, Traits, Allocator>>>                                                             \
    {                                                                                                              \
        return ptr->memfun(std::forward<Args>(args) ...);                                                          \
    }                                                                                                              \
                                                                                                                   \
    /* import const member function that returns other type */                                                     \
    template<typename ... Args>                                                                                    \
    auto memfun(Args && ... args) const                                                                            \
        ->std::enable_if_t<                                                                                        \
            !std::is_same_v<                                                                                       \
                std::decay_t<decltype(std::declval<const primitive_type>().memfun(std::forward<Args>(args) ...))>, \
                primitive_type>,                                                                                   \
            decltype(std::declval<const primitive_type>().memfun(std::forward<Args>(args) ...))                    \
        >                                                                                                          \
    {                                                                                                              \
        return ptr->memfun(std::forward<Args>(args) ...);                                                          \
    }                                                                                                              \
//define import_std_basic_string_function
    import_std_basic_string_function(begin            )
    import_std_basic_string_function(end              )
    import_std_basic_string_function(cbegin           )
    import_std_basic_string_function(cend             )
    import_std_basic_string_function(rbegin           )
    import_std_basic_string_function(rend             )
    import_std_basic_string_function(crbegin          )
    import_std_basic_string_function(crend            )
    import_std_basic_string_function(size             )
    import_std_basic_string_function(length           )
    import_std_basic_string_function(max_size         )
    import_std_basic_string_function(resize           )
    import_std_basic_string_function(capacity         )
    import_std_basic_string_function(reserve          )
    import_std_basic_string_function(shrink_to_fit    )
    import_std_basic_string_function(clear            )
    import_std_basic_string_function(empty            )
    import_std_basic_string_function(operator []      )
    import_std_basic_string_function(at               )
    import_std_basic_string_function(front            )
    import_std_basic_string_function(back             )
    import_std_basic_string_function(append           )
    import_std_basic_string_function(push_back        )
    import_std_basic_string_function(assign           )
    import_std_basic_string_function(insert           )
    import_std_basic_string_function(erase            )
    import_std_basic_string_function(pop_back         )
    import_std_basic_string_function(replace          )
    import_std_basic_string_function(c_str            )
    import_std_basic_string_function(data             )
    import_std_basic_string_function(get_allocator    )
    import_std_basic_string_function(copy             )
    import_std_basic_string_function(find             )
    import_std_basic_string_function(rfind            )
    import_std_basic_string_function(find_first_of    )
    import_std_basic_string_function(find_last_of     )
    import_std_basic_string_function(find_first_not_of)
    import_std_basic_string_function(find_last_not_of )
    import_std_basic_string_function(substr           )
    import_std_basic_string_function(compare          )
#undef import_std_basic_string_function

    template<typename U>
    operator U() const {
        return generic_cast<U>(*ptr);
    }

    operator const primitive_type & () const {
        return *ptr;
    }

    operator std::basic_string_view<CharT, Traits>() const {
        return std::basic_string_view<CharT, Traits>{*ptr};
    }

#define import_std_basic_string_type_alias(type) \
    using type = typename primitive_type::type;  \
//define import_std_basic_string_type_alias
    import_std_basic_string_type_alias(traits_type           )
    import_std_basic_string_type_alias(value_type            )
    import_std_basic_string_type_alias(allocator_type        )
    import_std_basic_string_type_alias(size_type             )
    import_std_basic_string_type_alias(difference_type       )
    import_std_basic_string_type_alias(reference             )
    import_std_basic_string_type_alias(const_reference       )
    import_std_basic_string_type_alias(pointer               )
    import_std_basic_string_type_alias(const_pointer         )
    import_std_basic_string_type_alias(iterator              )
    import_std_basic_string_type_alias(const_iterator        )
    import_std_basic_string_type_alias(reverse_iterator      )
    import_std_basic_string_type_alias(const_reverse_iterator)
#undef import_std_basic_string_type_alias

    static const size_type npos = primitive_type::npos;

    auto operator +=(std::basic_string_view<CharT, Traits> view)
        -> let<string_tag<CharT, Traits, Allocator>> &
    {
        using namespace std;
        auto temp_ptr = std::make_shared<primitive_type>(*this);
        temp_ptr->append(view);
        auto temp_const_ptr = std::static_pointer_cast<const primitive_type>(temp_ptr);
        swap(ptr, temp_const_ptr);
        return *this;
    }

    auto operator +(std::basic_string_view<CharT, Traits> view)
        -> let<string_tag<CharT, Traits, Allocator> >
    {
        return let<string_tag<CharT, Traits, Allocator>>{*ptr} += view;
    }

    auto left(size_type length) const
        -> let
    {
        return ptr->substr(0, length);
    }

    auto right(size_type length) const
        -> let
    {
        return ptr->substr(this->length() - length);
    }

    auto to_upper_copy() const
        -> let
    {
        primitive_type temp = *this;
        std::transform(temp.begin(), temp.end(), temp.begin(), std::toupper);
        return temp;
    }

    auto to_lower_copy() const
        -> let
    {
        primitive_type temp = *this;
        std::transform(temp.begin(), temp.end(), temp.begin(), std::tolower);
        return temp;
    }

    template<typename pred_t>
    auto trim_left_if_copy(pred_t && pred) const
        -> let
    {
        auto temp = *ptr;
        auto trim_end = std::find_if(temp.begin(), temp.end(), [&](auto && expr) { return !pred(expr); });
        if (trim_end != temp.end()) {
            return {trim_end, temp.end()};
        }
        return *this;
    }

    auto trim_left_copy() const
        -> let
    {
        return trim_left_if_copy(is_space{});
    }

    template<typename pred_t>
    auto trim_right_if_copy(pred_t && pred) const
        -> let
    {
        auto temp = *ptr;
        auto trim_rend = std::find_if(temp.rbegin(), temp.rend(), [&](auto && expr) { return !pred(expr); });
        if (trim_rend != temp.rend()) {
            return {std::addressof(*temp.begin()), std::addressof(*trim_rend) + 1};
        }
        return *this;
    }

    auto trim_right_copy() const
        -> let
    {
        return trim_right_if_copy(is_space{});
    }

    template<typename pred_t>
    auto trim_if_copy(pred_t && pred) const
        -> let
    {
        auto temp = *ptr;
        auto trim_end  = std::find_if(temp.begin(),  temp.end(),  [&](auto && expr) { return !pred(expr); });
        auto trim_rend = std::find_if(temp.rbegin(), temp.rend(), [&](auto && expr) { return !pred(expr); });
        if (trim_end != temp.end() || trim_rend != temp.rend()) {
            auto begin = std::addressof(trim_end  != temp.end()  ? *trim_end  : *temp.begin());
            auto end   = std::addressof(trim_rend != temp.rend() ? *trim_rend : *temp.rbegin()) + 1;
            return {begin, end};
        }
        return *this;
    }

    auto trim_copy() const
        -> let
    {
        return trim_if_copy(is_space{});
    }

    auto starts_with(std::string_view string) const
        -> bool
    {
        return left(string.length()) == string;
    }

    auto ends_with(std::string_view string) const
        -> bool
    {
        return right(string.length()) == string;
    }

    auto contains(std::string_view string) const
        -> bool
    {
        return find(string) != -1;
    }

    auto equals(std::string_view string) const
        -> bool
    {
        return *ptr == string;
    }

    template<typename pred_t>
    auto all(pred_t && pred) const
        -> bool
    {
        return std::all(ptr->begin(), ptr->end(), std::forward<pred_t>(pred));
    }

#define define_nonconst_function(name)                       \
    template<typename ... args_t>                            \
    auto name(args_t ... args)                               \
        -> void                                              \
    {                                                        \
        *this = name##_copy(std::forward<args_t>(args) ...); \
    }                                                        \
// define_nonconst_function
    define_nonconst_function(to_upper     )
    define_nonconst_function(to_lower     )
    define_nonconst_function(trim_left_if )
    define_nonconst_function(trim_left    )
    define_nonconst_function(trim_right_if)
    define_nonconst_function(trim_right   )
    define_nonconst_function(trim_if      )
    define_nonconst_function(trim         )
#undef define_nonconst_function

#define define_icase_predicate_function(name)              \
    template<typename arg_t>                               \
    auto i##name(arg_t && arg) const                       \
        -> bool                                            \
    {                                                      \
        let temp = std::forward<arg_t>(arg);               \
        return to_lower_copy().name(temp.to_lower_copy()); \
    }                                                      \
// define_icase_predicate_function
    define_icase_predicate_function(starts_with)
    define_icase_predicate_function(ends_with  )
    define_icase_predicate_function(contains   )
    define_icase_predicate_function(equals     )
#undef define_icase_predicate_function

#define define_character_predicate(name)     \
    using is_##name = Noz::is_##name<CharT>; \
// define_character_predicate
    define_character_predicate(alnum )
    define_character_predicate(alpha )
    define_character_predicate(blank )
    define_character_predicate(cntrl )
    define_character_predicate(digit )
    define_character_predicate(graph )
    define_character_predicate(lower )
    define_character_predicate(print )
    define_character_predicate(punct )
    define_character_predicate(space )
    define_character_predicate(upper )
    define_character_predicate(xdigit)
#undef define_character_predicate

private:
    std::shared_ptr<const std::basic_string<CharT, Traits, Allocator> > ptr;
};

#define define_comparison_operator(op) \
    template<typename CharT, typename Traits, typename Allocator> \
    auto operator op (                                            \
        const let<string_tag<CharT, Traits, Allocator>> & left,   \
        const let<string_tag<CharT, Traits, Allocator> > & right  \
    )                                                             \
        -> bool                                                   \
    {                                                             \
        return *left.ptr op *right.ptr;                           \
    }                                                             \
    template<typename CharT, typename Traits, typename Allocator> \
    auto operator op (                                            \
        const CharT * left,                                       \
        const let<string_tag<CharT, Traits, Allocator> > & right  \
    )                                                             \
        -> bool                                                   \
    {                                                             \
        return left op *right.ptr;                                \
    }                                                             \
    template<typename CharT, typename Traits, typename Allocator> \
    auto operator op (                                            \
        const let<string_tag<CharT, Traits, Allocator>> & left,   \
        const CharT * right                                       \
    )                                                             \
        -> bool                                                   \
    {                                                             \
        return *left.ptr op right;                                \
    }
// define define_comparison_operator
    define_comparison_operator(==)
    define_comparison_operator(!=)
    define_comparison_operator(< )
    define_comparison_operator(<=)
    define_comparison_operator(> )
    define_comparison_operator(>=)
#undef define_comparison_operator

template<typename CharT, typename Traits, typename Allocator>
void swap(let<string_tag<CharT, Traits, Allocator> > & a,
          let<string_tag<CharT, Traits, Allocator> > & b) {
    std::swap(a.ptr, b.ptr);
}

template<typename CharT, typename Traits, typename Allocator>
std::basic_ostream<CharT, Traits> & operator <<(std::basic_ostream<CharT, Traits> & ostream, const let<string_tag<CharT, Traits, Allocator>> & string) {
    return ostream << *string.ptr;
}

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits> & operator >>(std::basic_istream<CharT, Traits> & istream, let<string_tag<CharT, Traits, Allocator> > & string) {
    return istream >> *string.ptr;
}

template<typename CharT>
let(const CharT *) -> let<string_tag<CharT>>;

template<typename CharT, std::size_t n>
let(const CharT [n]) -> let<string_tag<CharT>>;

template<typename CharT, typename Traits = std::char_traits<CharT>>
let(std::basic_string_view<CharT, Traits>)->let<string_tag<CharT>>;

template<typename CharT, typename Traits, typename Allocator>
struct primitive_type_impl<let<string_tag<CharT, Traits, Allocator>>> {
    using type = std::basic_string<CharT, Traits, Allocator>;
};

template<typename CharT, typename Traits, typename Allocator>
struct get_primitive_value_impl<let<string_tag<CharT, Traits, Allocator>>> {
    static const std::basic_string<CharT, Traits, Allocator> & get(const let<string_tag<CharT, Traits, Allocator>> & instance) { return instance.value; }
    static       std::basic_string<CharT, Traits, Allocator> & get(      let<string_tag<CharT, Traits, Allocator>> & instance) { return instance.value; }
};

template<typename ... T>
using basic_string = let<string_tag<T ...>>;
using string  = basic_string<char>;
using wstring = basic_string<wchar_t>;

} // namespace Noz

template<typename CharT, typename Traits = std::char_traits<CharT>, typename T>
auto operator <<(std::basic_ostream<CharT, Traits> & os, const Noz::let<T> & v)
    -> std::basic_ostream<CharT, Traits> &
{
	auto string = Noz::generic_cast<std::basic_string<CharT, Traits>>(Noz::get_primitive_value(v));
    return os << string;
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT> >
auto operator +(std::basic_string_view<CharT, Traits> a, std::basic_string_view<CharT, Traits> b)
    -> Noz::let<Noz::string_tag<CharT, Traits, Allocator> >
{
    return Noz::let<std::basic_string<CharT, Traits, Allocator> >{a}.append(b);
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT> >
auto operator +(const Noz::let<Noz::string_tag<CharT, Traits, Allocator> > & a,
                const Noz::let<Noz::string_tag<CharT, Traits, Allocator> > & b)
    -> Noz::let<Noz::string_tag<CharT, Traits, Allocator> >
{
    return Noz::let<std::basic_string<CharT, Traits, Allocator> >{a}.append(b);
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>, typename T>
auto operator +(const Noz::let<Noz::string_tag<CharT, Traits, Allocator> > & a,
                const T & b)
    -> Noz::let<Noz::string_tag<CharT, Traits, Allocator> >
{
    return Noz::generic_cast<std::basic_string<CharT, Traits, Allocator> >(a).append(
           Noz::generic_cast<std::basic_string<CharT, Traits, Allocator> >(b)
    );
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>, typename T>
auto operator +(const T & a,
                const Noz::let<Noz::string_tag<CharT, Traits, Allocator> > & b)
    -> Noz::let<Noz::string_tag<CharT, Traits, Allocator> >
{
    return Noz::generic_cast<std::basic_string<CharT, Traits, Allocator> >(a).append(
           Noz::generic_cast<std::basic_string<CharT, Traits, Allocator> >(b)
    );
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
auto operator ==(const Noz::basic_string<CharT, Traits, Allocator> & a, const std::basic_string<CharT, Traits, Allocator> & b)
    -> bool
{
    return static_cast<const std::basic_string<CharT, Traits, Allocator> &>(a) == b;
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
auto operator ==(const std::basic_string<CharT, Traits, Allocator> & a, const Noz::basic_string<CharT, Traits, Allocator> & b)
    -> bool
{
    return a == static_cast<const std::basic_string<CharT, Traits, Allocator> &>(b);
}

namespace std {

template<typename CharT, typename Traits, typename Allocator>
class hash<Noz::let<Noz::string_tag<CharT, Traits, Allocator> > > {
public:
    std::size_t operator ()(const Noz::let<Noz::string_tag<CharT, Traits, Allocator>> & instance) const {
        return std::hash<std::basic_string<CharT, Traits, Allocator> >{}(*instance.ptr);
    }
    using result_type = std::size_t;
    using argument_type = Noz::let<Noz::string_tag<CharT, Traits, Allocator> >;
};

} // namespace std

#endif // STRING_HPP
