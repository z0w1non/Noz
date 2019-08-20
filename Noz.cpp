#include "Noz.hpp"

#define assert_equal(a, b) { assert(a == b); std::cout << #a << " == " << #b << std::endl; }

int main() {
	use Noz;
    {
        let s = "   a   ";
        assert(s.trim_copy() == "a");
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
