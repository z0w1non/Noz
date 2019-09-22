#define BOOST_TEST_MAIN
#include <boost//test/included/unit_test.hpp>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include "Noz.hpp"

BOOST_AUTO_TEST_CASE(trim)
{
    BOOST_CHECK_EQUAL(Noz::string(" a ").trim_copy(), "a");
    BOOST_CHECK_EQUAL(Noz::string("  a").trim_copy(), "a");
    BOOST_CHECK_EQUAL(Noz::string("a  ").trim_copy(), "a");
    BOOST_CHECK_EQUAL(Noz::string("  a").trim_left_copy(), "a");
    BOOST_CHECK_EQUAL(Noz::string("a  ").trim_right_copy(), "a");
}

BOOST_AUTO_TEST_CASE(string_predicate)
{
    BOOST_CHECK(Noz::wstring(L"abc").to_upper_copy() == L"ABC");
    BOOST_CHECK(Noz::wstring(L"ABC").to_lower_copy() == L"abc");
    BOOST_CHECK(Noz::string{"abcdef"}.starts_with("abc"));
    BOOST_CHECK(Noz::string{"abcdef"}.ends_with("def"));
    BOOST_CHECK(Noz::string{"abcdef"}.contains("cd"));
    BOOST_CHECK(Noz::string{"abcdef"}.equals("abcdef"));
    BOOST_CHECK(Noz::string{"abcdef"}.istarts_with("ABC"));
    BOOST_CHECK(Noz::string{"abcdef"}.iends_with("DEF"));
    BOOST_CHECK(Noz::string{"abcdef"}.icontains("CD"));
    BOOST_CHECK(Noz::string{"abcdef"}.iequals("ABCDEF"));
    BOOST_CHECK(Noz::string::is_alpha{}('a'));
    BOOST_CHECK(Noz::wstring::is_alpha{}(L'a'));
    BOOST_CHECK(Noz::wstring{L"abcdef"}.contains(L"cd"));
    BOOST_CHECK(Noz::wstring{L"abcdef"}.icontains(L"CD"));
    BOOST_CHECK(Noz::string::is_alnum{}('a'));
    BOOST_CHECK(Noz::string::is_alpha{}('a'));
    BOOST_CHECK(Noz::string::is_blank{}(' '));
    BOOST_CHECK(Noz::string::is_cntrl{}('\r'));
    BOOST_CHECK(Noz::string::is_digit{}('0'));
    BOOST_CHECK(Noz::string::is_graph{}('a'));
    BOOST_CHECK(Noz::string::is_lower{}('a'));
    BOOST_CHECK(Noz::string::is_print{}('a'));
    BOOST_CHECK(Noz::string::is_punct{}(','));
    BOOST_CHECK(Noz::string::is_space{}(' '));
    BOOST_CHECK(Noz::string::is_upper{}('A'));
    BOOST_CHECK(Noz::string::is_xdigit{}('a'));
}

BOOST_AUTO_TEST_CASE(nil_assignable)
{
    int * ptr = nil;
    BOOST_CHECK_EQUAL(ptr, nullptr);
	std::optional<int> opt = nil;
    BOOST_CHECK(!opt.has_value());
    BOOST_CHECK(opt == std::nullopt);
}

BOOST_AUTO_TEST_CASE(primitive_type_t)
{
    constexpr bool test1 = std::is_same_v<Noz::primitive_type_t<Noz::let<int>>, int>;                     BOOST_CHECK(test1);
    constexpr bool test2 = std::is_same_v<Noz::primitive_type_t<Noz::let<Noz::let<int>>>, int>;           BOOST_CHECK(test2);
    constexpr bool test3 = std::is_same_v<Noz::primitive_type_t<Noz::let<Noz::let<Noz::let<int>>>>, int>; BOOST_CHECK(test3);
}

using basic_types = std::tuple<
    bool,
    char,
    unsigned char,
    short,
    unsigned short,
    int,
    unsigned int,
    long,
    unsigned long,
    long long,
    unsigned long long,
    float,
    double,
    std::string,
    std::wstring
>;
BOOST_AUTO_TEST_CASE_TEMPLATE(class_size, type, basic_types)
{
    Noz::let instance = type{};
    BOOST_CHECK_EQUAL(sizeof(instance), sizeof(type));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(constructible_from_primitive_type, type, basic_types)
{
    Noz::let instance = type{};
}

BOOST_AUTO_TEST_CASE(test_generic_cast_numeric_to_string)
{
	Noz::let i = 1;
	BOOST_CHECK(Noz::test_generic_cast<bool       >(i) == "static_cast" );
	BOOST_CHECK(Noz::test_generic_cast<char       >(i) == "static_cast" );
	BOOST_CHECK(Noz::test_generic_cast<short      >(i) == "static_cast" );
	BOOST_CHECK(Noz::test_generic_cast<float      >(i) == "static_cast" );
	BOOST_CHECK(Noz::test_generic_cast<double     >(i) == "static_cast" );
	BOOST_CHECK(Noz::test_generic_cast<std::string>(i) == "lexical_cast");
}

BOOST_AUTO_TEST_CASE(test_generic_cast_string_to_numeric)
{
    Noz::let s = "1";
    BOOST_CHECK(Noz::test_generic_cast<bool       >(s) == "static_cast");
    BOOST_CHECK(Noz::test_generic_cast<char       >(s) == "static_cast");
    BOOST_CHECK(Noz::test_generic_cast<short      >(s) == "static_cast");
    BOOST_CHECK(Noz::test_generic_cast<float      >(s) == "static_cast");
    BOOST_CHECK(Noz::test_generic_cast<double     >(s) == "static_cast");
    BOOST_CHECK(Noz::test_generic_cast<std::string>(s) == "static_cast");
}

BOOST_AUTO_TEST_CASE(outut_to_ostringstream)
{
    std::ostringstream out;
    Noz::let i = 1;
    out << i;
    BOOST_CHECK_EQUAL(out.str(), "1");
}

BOOST_AUTO_TEST_CASE(generic_cast_as_static_pointer_cast)
{
    struct base {};
    struct derived : base { int value{1}; };
    BOOST_CHECK(Noz::generic_cast<std::shared_ptr<derived>>(std::make_shared<derived>())->value == 1);
}

BOOST_AUTO_TEST_CASE(left_right_substr)
{
    BOOST_CHECK_EQUAL(Noz::string{"0123456789"}.left(3), "012");
    BOOST_CHECK_EQUAL(Noz::string{"0123456789"}.right(3), "789");
    BOOST_CHECK_EQUAL(Noz::string{"0123456789"}.substr(4, 3), "456");
}

BOOST_AUTO_TEST_CASE(fixed_string_default_construct)
{
	Noz::fixed_string<10> s{};
}

BOOST_AUTO_TEST_CASE(fixed_string_construct_from_std_string)
{
	BOOST_CHECK((Noz::fixed_string<4>{std::string{"abc"}} == "abc"));
	BOOST_CHECK((Noz::fixed_string<4>{std::string{"abc"}, 1, 1} == "b"));
	BOOST_CHECK((Noz::fixed_string<4>{std::string_view{"abc"}, 1, 1} == "b"));
	BOOST_CHECK((Noz::fixed_string<4>{std::string_view{"abc"}, 1, 1} == "b"));
}

BOOST_AUTO_TEST_CASE(fixed_string_compare)
{
	BOOST_CHECK(Noz::fixed_string<4>{"abc"} == "abc");
	BOOST_CHECK(Noz::fixed_string<4>{"abcdef"} == "abc");
	BOOST_CHECK(Noz::fixed_string<1>{""} == "");
	BOOST_CHECK(Noz::fixed_string<1>{""}.empty());
}
