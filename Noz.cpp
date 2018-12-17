#include "Noz.hpp"
#include <cassert>
#include <typeinfo>

#define assert_equal(a, b) { assert(a == b); std::cout << #a << " == " << #b << std::endl; }

int main() {
    {
        std::ostringstream out;
        Noz::let s = "abc";
        out << s;
        assert(out.str() == "abc");
    }

    {
        std::ostringstream out;
        Noz::let i = 1;
        out << i;
        assert(out.str() == "1");
    }

    {
        Noz::Int i = 0;
        assert(!i);
    }

    {
        Noz::Int i = 1;
        assert(i);
    }

    {
        Noz::String s = "1.5";
        assert(Noz::generic_cast<double>(s) == 1.5);
    }

    {
        Noz::Int i = 1000;
        Noz::String s = i;
        assert(s == "1000");
    }

    #define $(arithmetic_type) \
        { \
            Noz::let value = static_cast<arithmetic_type>(0); \
            { \
                using is_same = std::is_same<decltype(value), Noz::let<arithmetic_type> >; \
                assert(is_same::value); \
                std::cout <<  "Noz::let<" << #arithmetic_type << "> is constuctible from static_cast<" << #arithmetic_type << ">(0)" << "." << std::endl; \
            } \
        } \
    //define $
        $(short)
        $(unsigned short)
        $(int)
        $(unsigned int)
        $(long)
        $(unsigned long)
        $(long long)
        $(unsigned long long)
        $(float)
        $(double)
    #undef $

    #define $(arithmetic_type, alias_type) \
        { \
            Noz::let<arithmetic_type> value{0};\
            { \
                constexpr auto is_same = std::is_same_v<decltype(value), Noz::alias_type>; \
                assert(is_same); \
                std::cout <<  "Noz::let<" << #arithmetic_type << "> is same to Noz::" << #alias_type << "." << std::endl; \
            } \
        } \
    //define $
        $(short, Short)
        $(unsigned short, UnsignedShort)
        $(int, Int)
        $(unsigned int, UnsignedInt)
        $(long, Long)
        $(unsigned long, UnsignedLong)
        $(long long, LongLong)
        $(unsigned long long, UnsignedLongLong)
        $(float, Float)
        $(double, Double)
    #undef $

    using arithmetic_types = std::tuple<
        char,
        signed char,
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
        double
    >;

    #define $(T, U) \
        { \
            U u{}; \
            Noz::let<T> value{u}; \
            std::cout << "Noz::let<" << #T << "> is constructible from " << #U << "." << std::endl; \
        } \

        $(char, bool)
        $(char, char)
        $(unsigned char, bool)
        $(unsigned char, unsigned char)
        $(short, bool)
        $(short, char)
        $(short, signed char)
        $(short, unsigned char)
        $(short, short)
        $(unsigned short, bool)
        $(unsigned short, unsigned char)
        $(unsigned short, unsigned short)
        $(int, bool)
        $(int, char)
        $(int, signed char)
        $(int, unsigned char)
        $(int, short)
        $(int, unsigned short)
        $(int, int)
        $(unsigned int, bool)
        $(unsigned int, unsigned char)
        $(unsigned int, unsigned short)
        $(unsigned int, unsigned int)
        $(long, bool)
        $(long, char)
        $(long, signed char)
        $(long, unsigned char)
        $(long, short)
        $(long, unsigned short)
        $(long, int)
        $(long, long)
        $(unsigned long, bool)
        $(unsigned long, unsigned char)
        $(unsigned long, unsigned short)
        $(unsigned long, unsigned int)
        $(unsigned long, unsigned long)
        $(long long, bool)
        $(long long, signed char)
        $(long long, char)
        $(long long, unsigned char)
        $(long long, short)
        $(long long, unsigned short)
        $(long long, int)
        $(long long, unsigned int)
        $(long long, long)
        $(long long, long long)
        $(unsigned long long, bool)
        $(unsigned long long, unsigned char)
        $(unsigned long long, unsigned short)
        $(unsigned long long, unsigned int)
        $(unsigned long long, unsigned long)
        $(unsigned long long, unsigned long long)
    #undef $

    #define $(T) \
        { \
            T instance{}; \
            Noz::let<T> value = "1"; \
            std::cout << "Noz::let<" << #T << "> is constructible from Noz::String." << std::endl; \
        } \
    //define $
    #undef $

    #define $(T, U) \
        { \
            U u{}; \
            auto t = Noz::generic_cast<T, U>(u); \
            constexpr auto is_same = std::is_same_v<decltype(t), T>; \
            assert(is_same); \
            std::cout << "Noz::generic_cast<" << #T << ", " << #U << ">(" << #U << ") is successed." << std::endl; \
        } \
    //define $
        $(std::string, bool)
        $(std::string, char)
        $(std::string, signed char)
        $(std::string, unsigned char)
        $(std::string, short)
        $(std::string, unsigned short)
        $(std::string, int)
        $(std::string, unsigned int)
        $(std::string, long)
        $(std::string, unsigned long)
        $(std::string, long long)
        $(std::string, unsigned long long)
        $(std::string, float)
        $(std::string, double)
    #undef $

    #define $(T, U) \
        { \
            T t{0}; \
            U u{1}; \
            Noz::swap(t, u); \
            assert(t == 1); \
            assert(u == 0); \
            std::cout << "Noz::swap(" << #T << ", " << #U << ") is successed." << std::endl; \
        \
            Noz::swap(u, t); \
            assert(t == 0); \
            assert(u == 1); \
            std::cout << "Noz::swap(" << #U << ", " << #T << ") is successed." << std::endl; \
        } \
    //define $
        $(bool, Noz::let<bool>)
        $(char, Noz::let<char>)
        $(signed char, Noz::let<signed char>)
        $(unsigned char, Noz::let<unsigned char>)
        $(int, Noz::let<int>)
        $(unsigned int, Noz::let<unsigned int>)
        $(long, Noz::let<long>)
        $(unsigned long, Noz::let<unsigned long>)
        $(long long, Noz::let<long long>)
        $(unsigned long long, Noz::let<unsigned long long>)
    #undef $

    {
        // using namespace std;
        // using namespace Noz;
        // let s = "0";
        // int i = s;
        // let d = 0.5;
        // std::string str = d;
    }

    {
        struct base {};
        struct derived : base { int value{1}; };
        assert(Noz::generic_cast<std::shared_ptr<derived>>(std::make_shared<derived>())->value == 1);
        std::cout << "Noz::generic_cast<std::shared_ptr<derived>>(std::make_shared<derived>()) is successed." << std::endl;
    }

    {
        assert_equal(Noz::String{"0123456789"}.left(3), "012");
        assert_equal(Noz::String{"0123456789"}.right(3), "789");
        assert_equal(Noz::String{"0123456789"}.substr(4, 3), "456");
    }

    {
        Noz::String s1{"foo"};
        std::string s2 = s1;
        assert_equal(s1, s2);
    }

    {
        std::string s1{"foo"};
        Noz::String s2 = s1;
        assert_equal(s1, s2);
    }

    {
        Noz::Input in{"./README.md"};
        auto c = in.read_char();
        std::ostringstream out;
        out << c;
        assert_equal(out.str(), "#");
    }

    {
        Noz::Input in{"./README.md"};
        auto line = in.read_line();
        std::ostringstream out;
        out << line;
        assert_equal(out.str(), "# Noz");
    }

    {
        // Noz::Output out{"./test.txt"};
        Noz::out < "foo", "bar", "buz";
    }
}
