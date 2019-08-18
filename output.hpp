#ifndef NOZ_OUTPUT_HPP
#define NOZ_OUTPUT_HPP

#include <cstdio>

namespace Noz {

template<typename>
class let;

template<typename>
struct snprintf;

template<>
struct snprintf<char> {
    template<typename ... args_t>
    auto operator()(args_t && ... args) const { return std::snprintf(args ...); }
};

template<>
struct snprintf<wchar_t> {
    template<typename ... args_t>
    auto operator()(args_t && ... args) const { return std::swprintf(args ...); }
};

template<
    typename base_type,
    typename CharT,
    typename Traits = std::char_traits<CharT>
>
class base_output {
public:
    auto write_char(CharT c)
        -> void
    {
        get_ostream() << c;
    }

    auto puts()
        -> void
    {
        get_ostream() << std::endl;
    }

    template<typename ... string_types>
    auto puts(string_types ... strings)
        -> void
    {
        write(std::forward<string_types>(strings) ...);
        puts();
    }

    template<typename input_type>
    auto write(input_type && input_value)
        -> void
    {
        get_ostream() << std::forward<input_type>(input_value);
    }

    template<typename input_type, typename ... cdr_t>
    auto write(input_type && input_value, cdr_t ... cdr)
        -> void
    {
        write(std::forward<input_type>(input_value));
        write(std::forward<cdr_t>(cdr) ...);
    }

    template<typename type>
    auto write_binary(const type * pointer, std::size_t length)
        -> void
    {
        const char * char_pointer = reinterpret_cast<const char *>(pointer);
        get_ostream().write(char_pointer, length);
    }

    explicit operator bool() const {
        return static_cast<bool>(get_ostream());
    }

    template<typename ... argment_types>
    int printf(const CharT * format, argment_types && ... argments) {
        int length;
        std::size_t buffer_size = Traits::length(format) + sizeof ... (argments) * 256 + 1;
        while (true) {
            std::unique_ptr<CharT[]> buffer = std::unique_ptr<CharT[]>(new CharT[buffer_size]);
            length = snprintf<CharT>{}(buffer.get(), buffer_size, format, std::forward<argment_types>(argments) ...);
            if (length > 0) {
                write(buffer.get());
                break;
            }
            buffer_size *= 2;
        }
        return length;
    }

    class temporary_base_output {
    public:
        temporary_base_output(base_output & base_output_instance) noexcept
            : base_output_instance{base_output_instance}
            , has_ownership{true}
        {
        }

        temporary_base_output(const temporary_base_output &) = delete;

        temporary_base_output(temporary_base_output && obj) noexcept
            : base_output_instance{obj.base_output_instance}
            , has_ownership{obj.has_ownership}
        {
            obj.has_ownership = false;
        }

        ~temporary_base_output() {
            if (has_ownership) {
                base_output_instance.puts();
            }
        }

        template<typename input_type>
        auto operator ,(const input_type & input_value)
            -> temporary_base_output
        {
            base_output_instance.get_ostream() << input_value;
            return temporary_base_output(std::move(*this));
        }

    private:
        base_output & base_output_instance;
        bool has_ownership;
    };

    template<typename input_type>
    auto operator <(input_type && input_value)
        -> temporary_base_output
    {
        get_ostream() << std::forward<input_type>(input_value);
        return temporary_base_output{get_base()};
    }

private:
    auto & get_base() {
        return *static_cast<base_type *>(this);
    }

    const auto & get_base() const {
        return *static_cast<const base_type *>(this);
    }

    auto & get_ostream() {
        return get_base().get_ostream();
    }

    const auto & get_ostream() const {
        return get_base().get_ostream();
    }
};

template<typename ...>
class ostream_tag;

struct as_reference_tag {};
struct as_value_tag {};

template<
    typename CharT,
    typename Traits
>
class let<ostream_tag<CharT, Traits>>
    : public base_output<let<ostream_tag<CharT, Traits>>, CharT, Traits>
{
    friend class base_output<let<ostream_tag<CharT, Traits>>, CharT, Traits>;

public:
    template<typename ostream_type, typename enable_if = std::enable_if_t<!is_let_v<ostream_type>>>
    let(ostream_type && ostream)
        : ostream_holder{std::make_shared<ostream_holder_impl<std::decay_t<ostream_type>, as_value_tag>>(std::forward<ostream_type>(ostream))}
    {}

    template<typename ostream_type, typename enable_if = std::enable_if_t<!is_let_v<ostream_type>>>
    let(ostream_type && ostream, as_reference_tag)
        : ostream_holder{std::make_shared<ostream_holder_impl<std::decay_t<ostream_type>, as_reference_tag>>(std::forward<ostream_type>(ostream))}
    {
    }

    let(let &) = default;
    let(let &&) = default;

    auto operator =(let &) -> let & = default;
    auto operator =(let &&) -> let & = default;

    //template<typename destination_type>
    //operator destination_type() const {
    //    return generic_cast<destination_type>(* this);
    //}

private:

    struct ostream_holder_interface {
    public:
        virtual ~ostream_holder_interface() {}
        virtual auto get() -> std::basic_ostream<CharT, Traits> & = 0;
    };

    template<typename ...>
    struct ostream_holder_impl;

    template<typename ostream_type>
    struct ostream_holder_impl<ostream_type, as_reference_tag>
        : ostream_holder_interface
    {
        ostream_holder_impl(ostream_type & ostream) : ostream{ostream} {}
        virtual ~ostream_holder_impl() {}
        virtual auto get() -> std::basic_ostream<CharT, Traits> & override { return ostream; }
        ostream_type & ostream;
    };

    template<typename ostream_type>
    struct ostream_holder_impl<ostream_type, as_value_tag>
        : ostream_holder_interface
    {
        template<typename ostream_type>
        ostream_holder_impl(ostream_type && ostream) : ostream{std::forward<ostream_type>(ostream)} {}
        virtual ~ostream_holder_impl() {}
        virtual auto get() -> std::basic_ostream<CharT, Traits> & override { return ostream; }
        ostream_type ostream;
    };

    std::shared_ptr<ostream_holder_interface> ostream_holder;

    auto get_ostream()
        -> std::basic_ostream<CharT, Traits> &
    {
        return ostream_holder->get();
    }

    auto get_ostream() const
        -> const std::basic_ostream<CharT, Traits> &
    {
        return ostream_holder->get();
    }
};

template<typename char_type, typename traits = std::char_traits<char_type>>
using basic_ostream = let<ostream_tag<char_type, traits>>;
using Output = basic_ostream<char>;
using WOutput = basic_ostream<wchar_t>;

template<typename CharT, typename Traits = std::char_traits<CharT>>
let(std::basic_ostream<CharT, Traits> &, as_reference_tag)
    ->let<ostream_tag<CharT, Traits>>;

template<typename CharT, typename Traits = std::char_traits<CharT>>
let(std::basic_ofstream<CharT, Traits> &&)
    ->let<ostream_tag<CharT, Traits>>;

template<typename CharT, typename Traits = std::char_traits<CharT>>
let(const let<ostream_tag<CharT, Traits>> &)
->let<ostream_tag<CharT, Traits>>;

template<typename CharT, typename Traits = std::char_traits<CharT>>
let(let<ostream_tag<CharT, Traits>> &&)
->let<ostream_tag<CharT, Traits>>;

let cout {std::cout,  as_reference_tag{}};
let wcout{std::wcout, as_reference_tag{}};
let cerr {std::cerr,  as_reference_tag{}};
let wcerr{std::wcerr, as_reference_tag{}};

} // namespace Noz

#endif // NOZ_OUTPUT_HPP
