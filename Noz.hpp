#ifndef NOZ_HPP
#define NOZ_HPP

#include <iostream>
#include <sstream>
#include <utility>
#include <type_traits>
#include <string>
#include <limits>
#include <memory>
#include <typeinfo>
#include <iterator>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace Noz {

template<typename T>
class is_shared_ptr {
private:
    template<typename U>
    struct impl {
        static auto test() -> std::false_type;
    };
    template<typename U>
    struct impl<std::shared_ptr<U>> {
        static auto test() -> std::true_type;
    };
public:
    static constexpr bool value = decltype(impl<T>::test())::value;
};
template<typename T>
constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

template<typename T>
class pointer_traits {};

template<typename T>
class pointer_traits<std::shared_ptr<T>> {
public:
    using element_type = T;
};

#define $begin(function_name) \
    template<typename T_> \
    class function_name { \
    private: \
        template<typename T> \
        static auto test(void *) \
            -> decltype( \
//define $begin

#define $end(function_name) \
                , std::true_type{} \
            ); \
        template<typename T> \
        static auto test(...) \
            -> std::false_type; \
    public: \
        static constexpr bool value = decltype(test<T_>(nullptr))::value; \
    }; \
    template<typename T> \
    constexpr bool function_name ## _v = function_name<T>::value; \
//define $end

    $begin(is_outputtable)
        std::declval<std::ostream &>() << std::declval<const T &>()
    $end(is_outputtable)

    $begin(is_inputtable)
        std::declval<std::istream &>() >> std::declval<T &>()
    $end(is_inputtable)

#undef $end
#undef $begin

template<typename T1, typename T2>
auto lexical_cast(const T2 & t2)
    -> T1
{
    T1 t1;
    try {
        std::ostringstream os{};
        os << t2;
        std::istringstream is{os.str()};
        is >> t1;
        if (!is) {
            throw std::exception();
        }
    } catch (std::exception & e) {
        throw std::bad_cast();
    }
    return t1;
}

template<typename T1, typename T2>
auto generic_cast(const T2 &)
    -> T1;

#define $begin(function_name) \
    template<typename T1_, typename T2_> \
    class function_name { \
    private: \
        template<typename T1, typename T2> \
        static auto test(void *) \
            -> decltype( \
//define $begin

#define $end(function_name) \
                , std::true_type{} \
            ); \
        template<typename T1, typename T2> \
        static auto test(...) \
            -> std::false_type; \
    public: \
        static constexpr bool value = decltype(test<T1_, T2_>(nullptr))::value; \
    }; \
    template<typename T1, typename T2> \
    constexpr bool function_name ## _v = function_name<T1, T2>::value; \
//define $end

    $begin(is_static_castable)
        static_cast<T2>(std::declval<T1>())
    $end(is_static_castable)

    $begin(is_static_pointer_castable)
        std::static_pointer_cast<typename pointer_traits<T2>::element_type>(std::declval<T1>())
    $end(is_static_pointer_castable)

    $begin(is_dynamic_castable)
        dynamic_cast<T2>(std::declval<T1>())
    $end(is_dynamic_castable)

    $begin(is_dynamic_pointer_castable)
        std::dynamic_pointer_cast<typename pointer_traits<T2>::element_type>(std::declval<T1>())
    $end(is_dynamic_pointer_castable)

    $begin(is_const_castable)
        const_cast<T2>(std::declval<T1>())
    $end(is_const_castable)

    $begin(is_constructor_castable)
        T2(std::declval<T1>())
    $end(is_constructor_castable)

    $begin(is_lexical_castable)
        lexical_cast<T2>(std::declval<T1>())
    $end(is_lexical_castable)

    $begin(is_generic_castable)
        generic_cast<T2>(std::declval<T1>())
    $end(is_generic_castable)

#undef $end
#undef $begin

#define DEFINE_META_FUNCTION_HAS_MEMBER_TYPE(member_type, function_name) \
    template<typename T> \
    class function_name { \
    private: \
        template<typename U> \
        static auto test(typename U::member_type *) \
            -> std::true_type; \
        template<typename U> \
        static auto test(...) \
            -> std::false_type; \
    public: \
        static constexpr bool value = decltype(test<T>(nullptr))::value; \
    }; \
    template<typename T> \
    static constexpr bool function_name ## _v = function_name<T>::value;

#define DEFINE_META_FUNCTION_HAS_MEMBER_VRIANT(member_variant, function_name) \
    tempalte<typename T> \
    class function_name { \
    private: \
        template<typename U> \
        static auto test(decltype(U::member_variant) *) \
            -> std::true_type; \
        template<typename U> \
        static auto test(...) \
            -> std::false_type; \
    public: \
        static constexpr bool value = decltype()::value; \
    }; \
    tempalte<typename T> \
    static constexpr bool function_name##_v = function_name<T>::value;

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
class string_tag {};

#define $if(cond, name) \
    template<typename From, typename To> \
    auto name(const From & from) \
        -> std::enable_if_t< \
            cond ## _v<From, To>, \
            To \
        > \
    { \
        return \
//define $if

#define $else(cond, name) \
    } \
    template<typename From, typename To> \
    auto name(const From & from) \
        -> std::enable_if_t< \
            !cond ## _v<From, To>, \
            To \
         > \
    { \
        return \
//define $else

#define $endif \
    } \
//define  endif

    $if(is_lexical_castable, try_lexical_cast)
        lexical_cast<To, From>(from);
    $else(is_lexical_castable, try_lexical_cast)
        std::enable_if_t<false>;
    $endif

    $if(is_constructor_castable, try_constructor_cast)
        To(from);
    $else(is_constructor_castable, try_constructor_cast)
        try_lexical_cast<From, To>(from);
    $endif

    $if(is_dynamic_pointer_castable, try_dynamic_pointer_cast)
        std::dynamic_pointer_cast<typename pointer_traits<To>::element_type>(from);
    $else(is_dynamic_pointer_castable, try_dynamic_pointer_cast)
        try_constructor_cast<From, To>(from);
    $endif

    $if(is_dynamic_castable, try_dynamic_cast)
        dynamic_cast<To>(from);
    $else(is_dynamic_castable, try_dynamic_cast)
        try_dynamic_pointer_cast<From, To>(from);
    $endif

    $if(is_static_pointer_castable, try_static_pointer_cast)
        std::static_pointer_cast<typename pointer_traits<To>::element_type>(from);
    $else(is_static_pointer_castable, try_static_pointer_cast)
        try_dynamic_cast<From, To>(from);
    $endif

    $if(is_static_castable, try_static_cast)
        static_cast<To>(from);
    $else(is_static_castable, try_static_cast)
        try_static_pointer_cast<From, To>(from);
    $endif

#undef $if
#undef $else
#undef $endif

template<typename To, typename From>
auto generic_cast(const From & from)
    -> To
{
    return try_static_cast<From, To>(from);
}

template<typename T>
class let;

template<typename CharT, typename Traits, typename Allocator>
class let<string_tag<CharT, Traits, Allocator>>;

template<typename T>
class let {
    friend std::hash<let<T>>;

    template<typename U>
    friend auto swap(let<U> &, U &) noexcept -> void;

    template<typename U>
    friend auto swap(U &, let<U> &) noexcept -> void;

public:
    template<typename U>
    constexpr let(U && instance)
        : value{generic_cast<std::decay_t<T>>(instance)}
    {}

    constexpr let()
        : value{}
    {}


#define $(name) \
    static constexpr auto name() { return std::numeric_limits<T>::name(); } \
//define $
    $(min)
    $(max)
    $(lowest)
    $(epsilon)
    $(round_error)
    $(infinity)
    $(quiet_NaN)
    $(signaling_NaN)
    $(denorm_NaN)
#undef $

#define $(name) \
    static constexpr auto name = std::numeric_limits<T>::name; \
//define $
    $(digits)
    $(digits10)
    $(max_digits10)
    $(is_signed)
    $(is_integer)
    $(is_exact)
    $(radix)
    $(min_exponent)
    $(min_exponent10)
    $(max_exponent)
    $(max_exponent10)
    $(has_infinity)
    $(has_quiet_NaN)
    $(has_signaling_NaN)
    $(has_denorm)
    $(has_denorm_loss)
    $(is_iec559)
    $(is_bounded)
    $(is_modulo)
    $(traps)
    $(tinyness_before)
    $(round_style)
#undef $

#define $(op) \
    auto operator op(T value) const \
        -> let<T> \
    { \
        return this->value op value; \
    } \
    auto operator op ## =(T value) \
        -> let<T> & \
    { \
        this->value op ## = value; \
        return *this; \
    } \
//define $
    $(+)
    $(-)
    $(*)
    $(/)
    $(%)
    $(|)
    $(&)
    $(^)
#undef $

auto operator ~() const
    -> let<T>
{
    return ~value;
}

#define $(op) \
auto operator op() \
    -> let<T> & \
{ \
    op value; \
    return *this; \
} \
auto operator op(int) \
    -> let<T> \
{ \
    let<T> temp = value; \
    op value; \
    return temp; \
} \
//define $
$(++)
$(--)
#undef $

#define $(op) \
    bool operator op(T value) const { \
        return this->value op value; \
    } \
//define $
    $(<)
    $(<=)
    $(>)
    $(>=)
    $(==)
    $(!=)
#undef $

    auto swap (let<T> & instance) noexcept
        -> void
    {
        std::swap(value, instance.value);
    }

    template<typename U>
    operator U() const {
        return generic_cast<U>(value);
    }

private:
    T value;
};

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

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT> >
class let<string_tag<CharT, Traits, Allocator>> {

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
    -> std::basic_ostream<CharT_, Traits_> &;

template<typename CharT_, typename Traits_, typename Allocator_>
friend auto operator >>(std::basic_istream<CharT_, Traits_> & istream, let<string_tag<CharT_, Traits_, Allocator_> > & string)
    -> std::basic_istream<CharT_, Traits_> &;

friend class std::hash<let<string_tag<CharT, Traits, Allocator> > >;

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
    /* Override const member function that return other type */ \
    template<typename ... Args> \
    auto memfun(Args && ... args) \
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
        return ptr->substr(length() - length);
    }

private:
    std::shared_ptr<const std::basic_string<CharT, Traits, Allocator> > ptr;
};

#define $(op) \
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
    } \
//define $
    $(==)
    $(!=)
    $(< )
    $(<=)
    $(> )
    $(>=)
#undef $

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

template<typename T>
class var_traits {
public:
    using primitive_type = T;
    using type = let<T>;
    static       type & get_premitive(      type & instance) { return instance; }
    static const type & get_premitive(const type & instance) { return instance; }
};

template<typename T>
class var_traits<let<T> > {
public:
    using primitive_type = typename var_traits<T>::primitive_type;
    using type = let<primitive_type>;
    static       type & get_premitive(      type & instance) { return instance.value; }
    static const type & get_premitive(const type & instance) { return instance.value; }
};

template<typename CharT, typename Traits, typename Allocator>
class var_traits<let<string_tag<CharT, Traits, Allocator> > > {
public:
    using primitive_type = std::basic_string<CharT, Traits, Allocator>;
    using type = let<string_tag<CharT, Traits, Allocator>>;
    static       type & get_premitive(      type & instance) { return *instance.ptr; }
    static const type & get_premitive(const type & instance) { return *instance.ptr; }
};

template<typename CharT>
let(const CharT *) -> let<string_tag<CharT>>;

template<typename CharT, typename Size, Size N>
let(const CharT [N]) -> let<string_tag<CharT>>;

template<typename T>
let(T &&)->let<typename var_traits<typename std::decay_t<T&&> >::primitive_type>;

using Bool = let<bool>;
using Char = let<char>;
using UnsignedChar = let<unsigned char>;
using SignedChar = let<signed char>;
using Short = let<short>;
using UnsignedShort = let<unsigned short>;
using Int = let<int>;
using UnsignedInt = let<unsigned int>;
using Long = let<long>;
using UnsignedLong = let<unsigned long>;
using LongLong = let<long long>;
using UnsignedLongLong = let<unsigned long long>;
using Float = let<float>;
using Double = let<double>;
using String = let<string_tag<char>>;
using WString = let<string_tag<wchar_t>>;

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

template<typename CharT, typename Traits = std::char_traits<CharT>, typename T>
auto operator <<(std::basic_ostream<CharT, Traits> & os, const Noz::let<T> & v)
    -> std::basic_ostream<CharT, Traits> &
{
    return os << static_cast<std::basic_string<CharT, Traits> >(v);
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

namespace std {

#define $(type) \
    template<> \
    class hash<Noz::let<type>> { \
    public: \
        std::size_t operator ()(const Noz::let<type> & instance) const { \
            return std::hash<type>{}(instance.value); \
        } \
        using result_type = std::size_t; \
        using argument_type = Noz::let<type>; \
    }; \
//define $
    $(char)
    $(unsigned char)
    $(signed char)
    $(wchar_t)
    $(short)
    $(unsigned short)
    $(int)
    $(long)
    $(unsigned long)
    $(long long)
    $(unsigned long long)
    $(float)
    $(double)
#undef $

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

#endif //NOZ_HPP
