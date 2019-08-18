#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "let.hpp"
#include "generic_cast.hpp"

namespace Noz {

template<typename T>
class let {
    friend std::hash<let<T>>;
    friend get_primitive_value_impl<let<T>>;

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


#define import_constexpr_function(name) \
    static constexpr auto name() { return std::numeric_limits<T>::name(); } \
//define import_constexpr_function
    import_constexpr_function(min          )
    import_constexpr_function(max          )
    import_constexpr_function(lowest       )
    import_constexpr_function(epsilon      )
    import_constexpr_function(round_error  )
    import_constexpr_function(infinity     )
    import_constexpr_function(quiet_NaN    )
    import_constexpr_function(signaling_NaN)
    import_constexpr_function(denorm_NaN   )
#undef import_constexpr_function

#define import_constexpr_value(name) \
    static constexpr auto name = std::numeric_limits<T>::name; \
//define $
    import_constexpr_value(digits           )
    import_constexpr_value(digits10         )
    import_constexpr_value(max_digits10     )
    import_constexpr_value(is_signed        )
    import_constexpr_value(is_integer       )
    import_constexpr_value(is_exact         )
    import_constexpr_value(radix            )
    import_constexpr_value(min_exponent     )
    import_constexpr_value(min_exponent10   )
    import_constexpr_value(max_exponent     )
    import_constexpr_value(max_exponent10   )
    import_constexpr_value(has_infinity     )
    import_constexpr_value(has_quiet_NaN    )
    import_constexpr_value(has_signaling_NaN)
    import_constexpr_value(has_denorm       )
    import_constexpr_value(has_denorm_loss  )
    import_constexpr_value(is_iec559        )
    import_constexpr_value(is_bounded       )
    import_constexpr_value(is_modulo        )
    import_constexpr_value(traps            )
    import_constexpr_value(tinyness_before  )
    import_constexpr_value(round_style      )
#undef import_constexpr_value

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
    $(< )
    $(<=)
    $(> )
    $(>=)
    $(==)
    $(!=)
#undef $

        auto swap(let<T> & instance) noexcept
        -> void
    {
        std::swap(value, instance.value);
    }

    template<typename U>
    operator U() const {
        return generic_cast<primitive_type_t<U>>(value);
    }

private:
    T value;
};

using Bool             = let<bool              >;
using Char             = let<char              >;
using UnsignedChar     = let<unsigned char     >;
using SignedChar       = let<signed char       >;
using Short            = let<short             >;
using UnsignedShort    = let<unsigned short    >;
using Int              = let<int               >;
using UnsignedInt      = let<unsigned int      >;
using Long             = let<long              >;
using UnsignedLong     = let<unsigned long     >;
using LongLong         = let<long long         >;
using UnsignedLongLong = let<unsigned long long>;
using Float            = let<float             >;
using Double           = let<double            >;

} // namespace Noz

namespace std {

#define define_hash_function_object(type) \
    template<> \
    class hash<Noz::let<type>> { \
    public: \
        std::size_t operator ()(const Noz::let<type> & instance) const { \
            return std::hash<type>{}(instance.value); \
        } \
        using result_type = std::size_t; \
        using argument_type = Noz::let<type>; \
    }; \
// define define_hash_function_object
    define_hash_function_object(char              )
    define_hash_function_object(unsigned char     )
    define_hash_function_object(signed char       )
    define_hash_function_object(wchar_t           )
    define_hash_function_object(short             )
    define_hash_function_object(unsigned short    )
    define_hash_function_object(int               )
    define_hash_function_object(long              )
    define_hash_function_object(unsigned long     )
    define_hash_function_object(long long         )
    define_hash_function_object(unsigned long long)
    define_hash_function_object(float             )
    define_hash_function_object(double            )
#undef define_hash_function_object

} // namespace std

#endif // NUMERIC_HPP
