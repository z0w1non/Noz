#ifndef GENERIC_CAST_HPP
#define GENERIC_CAST_HPP

namespace Noz {

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

template<typename destination_type, typename source_type>
auto lexical_cast(const source_type & source)
    -> destination_type
{
    destination_type destination;
    try {
        std::ostringstream os{};
        os << source;
        std::istringstream is{os.str()};
        is >> destination;
        if (!is) {
            throw std::exception();
        }
    } catch (std::exception &) {
        throw std::bad_cast();
    }
    return destination;
}

#define $begin(function_name) \
    template<typename T1_, typename T2_> \
    class function_name { \
    private: \
        template<typename source_type, typename destination_type> \
        static auto test(void *) \
            -> decltype( \
//define $begin

#define $end(function_name) \
                , std::true_type{} \
            ); \
        template<typename, typename> \
        static auto test(...) \
            -> std::false_type; \
    public: \
        static constexpr bool value = decltype(test<T1_, T2_>(nullptr))::value; \
    }; \
    template<typename T1, typename T2> \
    constexpr bool function_name ## _v = function_name<T1, T2>::value; \
//define $end

    $begin(is_static_castable)
        static_cast<destination_type>(std::declval<source_type>())
    $end(is_static_castable)

    $begin(is_static_pointer_castable)
        std::static_pointer_cast<typename pointer_traits<destination_type>::element_type>(std::declval<source_type>())
    $end(is_static_pointer_castable)

    $begin(is_dynamic_castable)
        dynamic_cast<destination_type>(std::declval<source_type>())
    $end(is_dynamic_castable)

    $begin(is_dynamic_pointer_castable)
        std::dynamic_pointer_cast<typename pointer_traits<destination_type>::element_type>(std::declval<source_type>())
    $end(is_dynamic_pointer_castable)

    $begin(is_const_castable)
        const_cast<destination_type>(std::declval<source_type>())
    $end(is_const_castable)

    $begin(is_constructor_castable)
        destination_type(std::declval<source_type>())
    $end(is_constructor_castable)

    $begin(is_lexical_castable)
        lexical_cast<destination_type>(std::declval<source_type>())
    $end(is_lexical_castable)

    $begin(is_generic_castable)
        generic_cast<destination_type>(std::declval<source_type>())
    $end(is_generic_castable)

#undef $end
#undef $begin

template<typename destination_type, typename source_type>
auto test_generic_cast(const source_type &)
	-> std::string
{
	if (is_static_castable_v<source_type, destination_type>)
		return "static_cast";
	else if (is_static_pointer_castable_v<source_type, destination_type>)
		return "static_pointer_cast";
	else if (is_dynamic_castable_v<source_type, destination_type>)
		return "dynamic_pointer_cast";
	else if (is_dynamic_pointer_castable_v<source_type, destination_type>)
		return "dynamic_pointer_cast";
	else if (is_constructor_castable_v<source_type, destination_type>)
		return "constructor_cast";
	else if (is_lexical_castable_v<source_type, destination_type>)
		return "lexical_cast";
	else
		return "uncanstable";
}

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

#define $if(pred, name) \
    template<typename source_type, typename destination_type> \
    auto name(const source_type & source) \
        -> std::enable_if_t< \
            pred ## _v<source_type, destination_type>, \
            destination_type \
        > \
    { \
        return \
//define $if

#define $else(pred, name) \
    } \
    template<typename source_type, typename destination_type> \
    auto name(const source_type & source) \
        -> std::enable_if_t< \
            !pred ## _v<source_type, destination_type>, \
            destination_type \
         > \
    { \
        return \
//define $else

#define $endif \
    } \
//define  endif

    $if (is_lexical_castable, try_lexical_cast)
        lexical_cast<destination_type>(source);
    $else (is_lexical_castable, try_lexical_cast)
        std::enable_if_t<false>;
    $endif

    $if (is_constructor_castable, try_constructor_cast)
        To(from);
    $else (is_constructor_castable, try_constructor_cast)
        try_lexical_cast<source_type, destination_type>(source);
    $endif

    $if(is_dynamic_pointer_castable, try_dynamic_pointer_cast)
        std::dynamic_pointer_cast<typename pointer_traits<destination_type>::element_type>(source);
    $else (is_dynamic_pointer_castable, try_dynamic_pointer_cast)
        try_constructor_cast<source_type, destination_type>(source);
    $endif

    $if (is_dynamic_castable, try_dynamic_cast)
        dynamic_cast<destination_type>(source);
    $else (is_dynamic_castable, try_dynamic_cast)
        try_dynamic_pointer_cast<source_type, destination_type>(source);
    $endif

    $if (is_static_pointer_castable, try_static_pointer_cast)
        std::static_pointer_cast<typename pointer_traits<destination_type>::element_type>(source);
    $else (is_static_pointer_castable, try_static_pointer_cast)
        try_dynamic_cast<source_type, destination_type>(source);
    $endif

    $if (is_static_castable, try_static_cast)
        static_cast<destination_type>(source);
    $else (is_static_castable, try_static_cast)
        try_static_pointer_cast<source_type, destination_type>(source);
    $endif

#undef $if
#undef $else
#undef $endif

template<typename destination_type, typename source_type>
auto generic_cast(const source_type & source)
    -> destination_type
{
    return try_static_cast<source_type, destination_type>(source);
}

} // namespace Noz

#endif // GENERIC_CAST_HPP
