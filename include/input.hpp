#ifndef NOZ_INPUT_HPP
#define NOZ_INPUT_HPP

namespace Noz {

template<
    typename T,
    typename CharT,
    typename Traits = std::char_traits<CharT>
>
class BaseInput {
public:
    auto read_char()
        -> CharT
    {
        CharT c;
        auto & impl = static_cast<T*>(this)->get_impl();
        impl >> c;
        return c;
    }

    auto read_line()
        -> Noz::BasicString<CharT, Traits>
    {
        std::basic_string<CharT, Traits> line;
        auto & impl = static_cast<T*>(this)->get_impl();
        std::getline(impl, line);
        if (line.length() > 0 && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }
        if (line.length() > 0 && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }
        return line;
    }

    auto read_all()
        -> Noz::BasicString<CharT, Traits>
    {
        using iterator = std::istreambuf_iterator<CharT>;
        auto & impl = static_cast<T*>(this)->get_impl();
        return Noz::BasicString<CharT, Traits>{
            iterator(impl),
            iterator()
        };
    }

    template<typename U>
    auto read(U & instance)
        -> bool
    {
        auto & impl = static_cast<T*>(this)->get_impl();
        impl >> instance;
        return static_cast<bool>(impl);
    }

    explicit operator bool() const {
        auto & impl = static_cast<T*>(this)->get_impl();
        return static_cast<bool>(impl);
    }

    class TemporaryBaseInput {
    public:
        TemporaryBaseInput(BaseInput & instance)
            : instance{instance}
        {}
        TemporaryBaseInput(const TemporaryBaseInput &) = default;

        template<typename U>
        auto operator ,(U & obj)
            -> TemporaryBaseInput
        {
            auto & impl = static_cast<T&>(instance).get_impl();
            impl >> obj;
            return *this;
        }

    private:
        BaseInput & instance;
    };

    template<typename U>
    auto operator >(U & instance)
        -> TemporaryBaseInput
    {
        auto & impl = static_cast<T*>(this)->get_impl();
        impl >> instance;
        return static_cast<T&>(*this);
    }
};

template<typename ... T>
class BasicInput;

template<
    typename CharT,
    typename Traits
>
class BasicInput<std::basic_ifstream<CharT, Traits>>
    : public BaseInput<
        BasicInput<std::basic_ifstream<CharT, Traits>>,
        CharT,
        Traits
    >
{
    friend class BaseInput<
        BasicInput<std::basic_ifstream<CharT, Traits>>,
        CharT,
        Traits
    >;

public:
    template<typename ... Args>
    BasicInput(Args && ... args)
        : in{std::forward<Args>(args) ...}
    {}

private:
    auto get_impl()
        -> std::basic_ifstream<CharT, Traits> &
    {
        return in;
    }

    auto get_impl() const
        -> const std::basic_ifstream<CharT, Traits> &
    {
        return in;
    }

    std::basic_ifstream<CharT, Traits> in;
};

template<
    typename CharT,
    typename Traits
>
class BasicInput<std::basic_istream<CharT, Traits>>
    : public BaseInput<
        BasicInput<std::basic_istream<CharT, Traits>>,
        CharT,
        Traits
    >
{
    friend class BaseInput<
        BasicInput<std::basic_istream<CharT, Traits>>,
        CharT,
        Traits
    >;

public:
    BasicInput(std::basic_istream<CharT, Traits> & in)
        : in{in}
    {}

private:
    auto get_impl()
        -> std::basic_istream<CharT, Traits> &
    {
        return in;
    }

    auto get_impl() const
        -> const std::basic_istream<CharT, Traits> &
    {
        return in;
    }

    std::basic_istream<CharT, Traits> & in;
};

using Input = BasicInput<std::basic_ifstream<char>>;
using WInput = BasicInput<std::basic_ifstream<wchar_t>>;

template<
    typename CharT,
    typename Traits = std::char_traits<CharT>,
    typename Allocator = std::allocator<CharT>
>
BasicInput(BasicString<CharT, Traits, Allocator>)
    -> BasicInput<std::basic_ifstream<CharT, Traits> >;

template<typename CharT>
BasicInput(const CharT *)
    -> BasicInput<std::basic_ifstream<CharT>>;

template<
    typename CharT,
    typename Traits = std::char_traits<CharT>
>
BasicInput(std::basic_istream<CharT, Traits> &)
    -> BasicInput<std::basic_istream<CharT, Traits>>;

BasicInput in{std::cin};
BasicInput win(std::wcin);

} // namespace Noz

#endif // NOZ_INPUT_HPP
