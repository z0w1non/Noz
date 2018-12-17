#ifndef NOZ_OUTPUT_HPP
#define NOZ_OUTPUT_HPP

namespace Noz {

template<
    typename T,
    typename CharT,
    typename Traits = std::char_traits<CharT>
>
class BaseOutput {
public:
    auto write_char(CharT c)
        -> void
    {
        static_cast<T*>(this)->get_impl() << c;
    }

    template<typename String>
    auto write_line(const String & line)
        -> void
    {
        if (line.length() > 0 && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }
        if (line.length() > 0 && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }
        static_cast<T *>(this)->get_impl() << line << std::endl;
    }

    template<typename String>
    auto write_all(const String & all)
        -> void
    {
        static_cast<T *>(this)->get_impl() << all;
    }

    template<typename U>
    auto write(const U & obj)
        -> void
    {
        static_cast<T*>(this)->get_impl() << obj;
    }

    explicit operator bool() const {
        return static_cast<bool>(static_cast<T*>(this)->get_impl());
    }

    class TemporaryBaseOutput {
    public:
        TemporaryBaseOutput(T & instance) : instance(instance) {}
        TemporaryBaseOutput(const TemporaryBaseOutput &) = delete;
        TemporaryBaseOutput(TemporaryBaseOutput && obj) :
            instance{obj.instance},
            has_ownership{obj.has_ownership}
        { obj.has_ownership = false; }

        ~TemporaryBaseOutput() {
            if (has_ownership) {
                auto & impl = static_cast<T&>(instance).get_impl();
                impl << std::endl;
            }
        }

        template<typename U>
        auto operator ,(const U & obj)
            -> TemporaryBaseOutput
        {
            auto & impl = static_cast<T&>(instance).get_impl();
            impl << obj;
            return std::move(*this);
        }

    private:
        T & instance;
        bool has_ownership{true};
    };

    template<typename U>
    auto operator <(U & instance)
        -> TemporaryBaseOutput
    {
        auto & impl = static_cast<T*>(this)->get_impl();
        impl << instance;
        return *static_cast<T*>(this);
    }
};

template<typename ... T>
class BasicOutput;

template<
    typename CharT,
    typename Traits
>
class BasicOutput<std::basic_ofstream<CharT, Traits>>
    : public BaseOutput<
        BasicOutput<std::basic_ofstream<CharT, Traits>>,
        CharT,
        Traits
    >
{
    friend class BaseOutput<
        BasicOutput<std::basic_ofstream<CharT, Traits>>,
        CharT,
        Traits
    >;

public:
    template<typename ... Args>
    BasicOutput(Args && ... args)
        : out{std::forward<Args>(args) ...}
    {}

private:
    auto get_impl()
        -> std::basic_ofstream<CharT, Traits> &
    {
        return out;
    }

    auto get_impl() const
        -> const std::basic_ofstream<CharT, Traits> &
    {
        return out;
    }

    std::basic_ofstream<CharT, Traits> out;
};

template<
    typename CharT,
    typename Traits
>
class BasicOutput<std::basic_ostream<CharT, Traits>>
    : public BaseOutput<
        BasicOutput<std::basic_ostream<CharT, Traits>>,
        CharT,
        Traits
    >
{
    friend class BaseOutput<
        BasicOutput<std::basic_ostream<CharT, Traits>>,
        CharT,
        Traits
    >;
public:
    BasicOutput(std::basic_ostream<CharT, Traits> & out)
        : out{out}
    {}

private:
    auto get_impl()
        -> std::basic_ostream<CharT, Traits> &
    {
        return out;
    }

    auto get_impl() const
        -> const std::basic_ostream<CharT, Traits> &
    {
        return out;
    }

    std::basic_ostream<CharT, Traits> & out;
};

using Output = BasicOutput<std::basic_ofstream<char>>;
using WOutput = BasicOutput<std::basic_ofstream<wchar_t>>;

template<
    typename CharT,
    typename Traits = std::char_traits<CharT>,
    typename Allocator = std::allocator<CharT>
>
BasicOutput(BasicString<CharT, Traits, Allocator>)
    -> BasicOutput<std::basic_ofstream<CharT, Traits> >;

template<typename CharT>
BasicOutput(const CharT *)
    -> BasicOutput<std::basic_ofstream<CharT>>;

template<
    typename CharT,
    typename Traits = std::char_traits<CharT>
>
BasicOutput(std::basic_ostream<CharT, Traits> &)
    -> BasicOutput<std::basic_ostream<CharT, Traits>>;

BasicOutput out{std::cout};
BasicOutput wout(std::wcout);
BasicOutput err{std::cerr};
BasicOutput werr(std::wcerr);

} // namespace Noz

#endif // NOZ_OUTPUT_HPP
