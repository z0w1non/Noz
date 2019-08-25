#include "Noz.hpp"

#define assert_equal(a, b) { assert(a == b); std::cout << #a << " == " << #b << std::endl; }

void generate_macro_hpp() {
    {
        // include guard
        auto out = std::ofstream{"./include/macro.hpp"};
        auto argnum = 64;
        out << "#ifndef MACRO_HPP" << std::endl;
        out << "#define MACRO_HPP" << std::endl;
        out << std::endl;

        out << "#define $string(a) $string_i(a)" << std::endl;
        out << "#define $string_i(a) #a" << std::endl;
        out << "#define $wstring(a) $wstring_i(a)" << std::endl;
        out << "#define $wstring_i(a) $wstring_ii(#a)" << std::endl;
        out << "#define $wstring_ii(a) L ## a" << std::endl;
        out << std::endl;

        out << "#define $forward(a) $forward_i(a)" << std::endl;
        out << "#define $forward_i(a) a" << std::endl;
        out << std::endl;

        out << "#define $cat(a, b) $cat_impl(a, b)" << std::endl;
        out << "#define $cat_impl(a, b) a ## b" << std::endl;
        out << "#define $variadic_size(...) $cat($variadic_size_impl(__VA_ARGS__, ";
        for (int i = argnum - 1; i >= 0; --i) {
            out << i;
            //if (i)
            out << ", ";
        }
        out << "),)" << std::endl;
        out << "#define $variadic_size_impl(";
        for (int i = 0; i < argnum - 1; ++i) {
            out << "_" << i << ", ";
        }
        out << "size, ...) ";
        out << "size" << std::endl;
        out << std::endl;

        //variadic_to_seq
        out << "#define $variadic_to_seq(...) $cat($variadic_to_seq_, $variadic_size(__VA_ARGS__)(__VA_ARGS__),)" << std::endl;
        //#define $variadic_to_seq_3(arg0, arg1, arg2) (arg0) (arg1) (arg2)
        for (int i = 0; i < argnum; ++i) {
            out << "#define $variadic_to_seq_" << i << "(";
            for (int j = 0; j < i; ++j) {
                out << "_" << j;
                if (j < i - 1) {
                    out << ", ";
                }
            }
            out << ") ";
            for (int j = 0; j < i; ++j) {
                out << "(_" << j << ")";
                if (j < i - 1) {
                    out << " ";
                }
            }
            out << std::endl;
        }
        out << std::endl;

        // seq_size
        out << "#define $seq_size(seq) $seq_size_i(seq)" << std::endl;
        out << "#define $seq_size_i(seq) $cat($seq_size_, $seq_size_0) seq" << std::endl;
        for (int i = 0; i < argnum; ++i) {
            out << "#define $seq_size_" << i << "(seq) $seq_size_" << (i + 1) << std::endl;
        }
        for (int i = 0; i < argnum; ++i) {
            out << "#define $seq_size_$seq_size_" << i << " " << i << std::endl;
        }
        out << std::endl;

        // lambda
        out << "#define $lambda(...) [=]($lambda_i($variadic_to_seq(__VA_ARGS__)))" << std::endl;
        out << "#define $lambda_i(seq) $lambda_ii($seq_size(seq),seq)" << std::endl;
        out << "#define $lambda_ii(size, seq) $cat($lambda_, size)seq" << std::endl;
        for (int i = 0; i < argnum; ++i) {
            out << "#define $lambda_" << i << "(a)";
            if (i > 0) {
                out << " auto a";
            }
            if (i > 1) {
                out << ", $lambda_" << (i - 1);
            }
            out << std::endl;
        }
        out << std::endl;

        out << "#endif" << std::endl;
    }
}

int main() {
    using namespace Noz;

    generate_macro_hpp();

    {
        assert(string(" a ").trim_copy()       == "a");
        assert(string("  a").trim_copy()       == "a");
        assert(string("a  ").trim_copy()       == "a");
        assert(string("  a").trim_left_copy()  == "a");
        assert(string("a  ").trim_right_copy() == "a");
        assert(string{"_a_"}.trim_if_copy([](auto c) { return c == '_'; }) == "a");
    }

    {
        let s = "abcdef";
        assert(s.starts_with("abc"));
        assert(s.ends_with("def"));
        assert(s.contains("cd"));
        assert(s.equals("abcdef"));
        assert(s.istarts_with("ABC"));
        assert(s.iends_with("DEF"));
        assert(s.icontains("CD"));
        assert(s.iequals("ABCDEF"));
        assert(string::is_alpha{}('a'));
        assert(wstring::is_alpha{}(L'a'));
        assert(wstring{L"abcdef"}.contains(L"cd"));
        assert(wstring{L"abcdef"}.icontains(L"CD"));
        assert(string::is_alnum {}('a'));
        assert(string::is_alpha {}('a'));
        assert(string::is_blank {}(' '));
        assert(string::is_cntrl {}('\r'));
        assert(string::is_digit {}('0'));
        assert(string::is_graph {}('a'));
        assert(string::is_lower {}('a'));
        assert(string::is_print {}('a'));
        assert(string::is_punct {}(','));
        assert(string::is_space {}(' '));
        assert(string::is_upper {}('A'));
        assert(string::is_xdigit{}('a'));
    }

	{
		int* ptr = nil;
		assert(ptr == nullptr);

		std::optional<int> opt = nil;
		assert(!opt.has_value());
		assert(opt == std::nullopt);
	}

	{
		int a = 0, b = 1;
		assert(a + b);
	}

	{
        constexpr auto i = 1;
		assert(i); // runtime assertion
	}

	{
		assert(std::is_same_v<primitive_type_t<let<int>>, int>, "");
		assert(std::is_same_v<primitive_type_t<let<let<int>>>, int>, "");
		assert(std::is_same_v<primitive_type_t<let<let<let<int>>>>, int>, "");
	}

	{
		assert(true);
	}

	{
		#define assert_size_equivalence(type) assert(sizeof(type) == sizeof(let<type>), "");
			assert_size_equivalence(bool);
			assert_size_equivalence(char);
			assert_size_equivalence(short);
			assert_size_equivalence(int);
			assert_size_equivalence(long);
			assert_size_equivalence(long long);
			assert_size_equivalence(float);
			assert_size_equivalence(double);
			assert_size_equivalence(std::string);
			assert_size_equivalence(std::wstring);
		#undef assert_size_equivalence
	}

    {
        std::ostringstream out;
        let s = "abc";
        out << s;
        assert(out.str() == "abc");
    }

	{
		let i = 1;
		assert(test_generic_cast<bool       >(i) == "static_cast" );
		assert(test_generic_cast<char       >(i) == "static_cast" );
		assert(test_generic_cast<short      >(i) == "static_cast" );
		assert(test_generic_cast<float      >(i) == "static_cast" );
		assert(test_generic_cast<double     >(i) == "static_cast" );
		assert(test_generic_cast<std::string>(i) == "lexical_cast");
	}

    {
        std::ostringstream out;
        let i = 1;
        out << i;
        assert(out.str() == "1");
    }

    {
        Int i = 0;
        assert(!i);
    }

    {
        Int i = 1;
        assert(i);
    }

    {
        Noz::string s = "1.5";
        assert(generic_cast<double>(s) == 1.5);
    }

	{
		//let i = 1000;
		//let s = i;
		//assert(s == "1000");
	}

    {
        //Int i = 1000;
        //Noz::string s = i;
        //assert(s == "1000");
    }

    #define $(arithmetic_type) \
        { \
            let value = static_cast<arithmetic_type>(0); \
            { \
                using is_same = std::is_same<decltype(value), let<arithmetic_type> >; \
                assert(is_same::value); \
                std::cout <<  "let<" << #arithmetic_type << "> is constuctible from static_cast<" << #arithmetic_type << ">(0)" << "." << std::endl; \
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
            let<arithmetic_type> value{0};\
            { \
                constexpr auto is_same = std::is_same_v<decltype(value), alias_type>; \
                assert(is_same); \
                std::cout <<  "let<" << #arithmetic_type << "> is same to " << #alias_type << "." << std::endl; \
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
            let<T> value{u}; \
            std::cout << "let<" << #T << "> is constructible from " << #U << "." << std::endl; \
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
            let<T> value = "1"; \
            std::cout << "let<" << #T << "> is constructible from Noz::string." << std::endl; \
        } \
    //define $
    #undef $

    #define $(T, U) \
        { \
            U u{}; \
            auto t = generic_cast<T, U>(u); \
            constexpr auto is_same = std::is_same_v<decltype(t), T>; \
            assert(is_same); \
            std::cout << "generic_cast<" << #T << ", " << #U << ">(" << #U << ") is successed." << std::endl; \
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
            swap(t, u); \
            assert(t == 1); \
            assert(u == 0); \
            std::cout << "swap(" << #T << ", " << #U << ") is successed." << std::endl; \
        \
            swap(u, t); \
            assert(t == 0); \
            assert(u == 1); \
            std::cout << "swap(" << #U << ", " << #T << ") is successed." << std::endl; \
        } \
    //define $
        //$(bool, let<bool>)
        //$(char, let<char>)
        //$(signed char, let<signed char>)
        //$(unsigned char, let<unsigned char>)
        //$(int, let<int>)
        //$(unsigned int, let<unsigned int>)
        //$(long, let<long>)
        //$(unsigned long, let<unsigned long>)
        //$(long long, let<long long>)
        //$(unsigned long long, let<unsigned long long>)
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
        assert(generic_cast<std::shared_ptr<derived>>(std::make_shared<derived>())->value == 1);
        std::cout << "generic_cast<std::shared_ptr<derived>>(std::make_shared<derived>()) is successed." << std::endl;
    }

    {
        assert_equal(Noz::string{"0123456789"}.left(3), "012");
        assert_equal(Noz::string{"0123456789"}.right(3), "789");
        assert_equal(Noz::string{"0123456789"}.substr(4, 3), "456");
    }

    {
        Noz::string s1{"foo"};
        std::string s2 = s1;
        assert_equal(s1, s2);
    }

    {
        std::string s1{"foo"};
        Noz::string s2 = s1;
        assert_equal(s1, s2);
    }

    {
        Input in{"./README.md"};
        auto c = in.read_char();
        std::ostringstream out;
        out << c;
        assert_equal(out.str(), "#");
    }

    {
        Input in{"./README.md"};
        auto line = in.read_line();
        std::ostringstream out;
        out << line;
        assert_equal(out.str(), "# Noz");
    }

    {
        //Output out{"test.txt"};
		cout < "foo", "bar", "buz";
    }

    {
        cout.write("foo", "bar", "buz");
        cout.puts();
        cout.puts("foo", "bar", "buz");

        cout.printf("printf called at %d\n", __LINE__);
    }

    {
        wcout.printf(L"printf called at %d\n", __LINE__);
    }

    {
        let f = std::ofstream{"out.txt"};
        f.puts("f");
        let f2 = f;
        f2.puts("f2");
    }

    {
        const char * buffer = "abcdef";
        cout.write_binary(buffer, 3);
    }
}
