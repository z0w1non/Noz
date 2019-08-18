#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include "let.hpp"

namespace Noz {

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
struct string_tag;

template<typename CharT, typename Traits, typename Allocator>
class let<string_tag<CharT, Traits, Allocator>> {
    friend get_primitive_value_impl<let<string_tag<CharT, Traits, Allocator>>>;

#define $(op) \
    template<typename CharT_, typename Traits_, typename Allocator_> \
    friend auto operator op (const let<string_tag<CharT_, Traits_, Allocator_> > & left, const let<string_tag<CharT_, Traits_, Allocator_> > & right) -> bool; \
    template<typename CharT_, typename Traits_, typename Allocator_> \
    friend auto operator op (const CharT_ * left, const let<string_tag<CharT_, Traits_, Allocator_> &> & right) -> bool; \
    template<typename CharT_, typename Traits_, typename Allocator_> \
    friend auto operator op (const let<string_tag<CharT_, Traits_, Allocator_> > & left, const CharT_ * right) -> bool; \
//define $
    $(==)
    $(!=)
    $(< )
    $(<=)
    $(> )
    $(>=)
#undef $

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
        : ptr{std::make_shared<std::basic_string<CharT, Traits, Allocator>>(std::forward<Args>(args) ...)}
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

#define $(memfun, ...) \
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
//define $
    $(begin)
    $(end)
    $(cbegin)
    $(cend)
    $(rbegin)
    $(rend)
    $(crbegin)
    $(crend)
    $(size)
    $(length)
    $(max_size)
    $(resize)
    $(capacity)
    $(reserve)
    $(shrink_to_fit)
    $(clear)
    $(empty)
    $(operator [])
    $(at)
    $(front)
    $(back)
    $(append)
    $(push_back)
    $(assign)
    $(insert)
    $(erase)
    $(pop_back)
    $(replace)
    $(c_str)
    $(data)
    $(get_allocator)
    $(copy)
    $(find)
    $(rfind)
    $(find_first_of)
    $(find_last_of)
    $(find_first_not_of)
    $(find_last_not_of)
    $(substr)
    $(compare)
#undef $

    template<typename U>
    operator U() const {
        return generic_cast<U>(*ptr);
    }

    operator const std::basic_string<CharT, Traits, Allocator> & () const {
        return *ptr;
    }

    operator std::basic_string_view<CharT, Traits>() const {
        return std::basic_string_view<CharT, Traits>{*ptr};
    }

#define $(type) \
    using type = typename std::basic_string<CharT, Traits, Allocator>::type; \
//define $
    $(traits_type)
    $(value_type)
    $(allocator_type)
    $(size_type)
    $(difference_type)
    $(reference)
    $(const_reference)
    $(pointer)
    $(const_pointer)
    $(iterator)
    $(const_iterator)
    $(reverse_iterator)
    $(const_reverse_iterator)
#undef $

        static const size_type npos = std::basic_string<CharT, Traits, Allocator>::npos;

    auto operator +=(std::basic_string_view<CharT, Traits> view)
        -> let<string_tag<CharT, Traits, Allocator>> &
    {
        using namespace std;
        auto temp_ptr = std::make_shared<std::basic_string<CharT, Traits, Allocator>>(*this);
        temp_ptr->append(view);
        auto temp_const_ptr = std::static_pointer_cast<const std::basic_string<CharT, Traits, Allocator>>(temp_ptr);
        swap(ptr, temp_const_ptr);
        return *this;
    }

    auto operator +(std::basic_string_view<CharT, Traits> view)
        -> let<string_tag<CharT, Traits, Allocator> >
    {
        return let<string_tag<CharT, Traits, Allocator>>{*ptr} += view;
    }

    auto left(size_type length) const
        -> let<string_tag<CharT, Traits, Allocator> >
    {
        return ptr->substr(0, length);
    }

    auto right(size_type length) const
        -> let<string_tag<CharT, Traits, Allocator> >
    {
        return ptr->substr(this->length() - length);
    }

private:
    std::shared_ptr<const std::basic_string<CharT, Traits, Allocator> > ptr;
};

#define define_comparison_operator(op) \
    template<typename CharT, typename Traits, typename Allocator> \
    auto operator op (const let<string_tag<CharT, Traits, Allocator>> & left, const let<string_tag<CharT, Traits, Allocator> > & right) \
        -> bool \
    { \
        return *left.ptr op *right.ptr; \
    } \
    template<typename CharT, typename Traits, typename Allocator> \
    auto operator op (const CharT * left, const let<string_tag<CharT, Traits, Allocator> > & right) \
        -> bool \
    { \
        return left op *right.ptr; \
    } \
    template<typename CharT, typename Traits, typename Allocator> \
    auto operator op (const let<string_tag<CharT, Traits, Allocator>> & left, const CharT * right) \
        -> bool \
    { \
        return *left.ptr op right; \
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
using BasicString = let<string_tag<T ...>>;
using String  = BasicString<char>;
using WString = BasicString<wchar_t>;

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
auto operator ==(const Noz::BasicString<CharT, Traits, Allocator> & a, const std::basic_string<CharT, Traits, Allocator> & b)
    -> bool
{
    return static_cast<const std::basic_string<CharT, Traits, Allocator> &>(a) == b;
}

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
auto operator ==(const std::basic_string<CharT, Traits, Allocator> & a, const Noz::BasicString<CharT, Traits, Allocator> & b)
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
