#ifndef NOZ_FIXED_STRING_HPP
#define NOZ_FIXED_STRING_HPP

#include <string>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <memory>

namespace Noz
{
    template<typename char_type_, std::size_t fixed_length_, typename traits_type_ = std::char_traits<char_type_>>
    class basic_fixed_string
    {
    public:
        class iterator;
        class const_iterator;

        using traits_type            = traits_type_;
        using value_type             = char_type_;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using reference              = value_type &;
        using const_reference        = const value_type &;
        using pointer                = value_type *;
        using const_pointer          = const value_type *;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static constexpr size_type npos = static_cast<size_type>(-1);

        basic_fixed_string()
        {}

        template<size_type fixed_length_>
        basic_fixed_string(const basic_fixed_string<value_type, fixed_length_> & str)
        {
            append(str);
        }

        template<size_type fixed_length_>
        basic_fixed_string(const basic_fixed_string<value_type, fixed_length_> & str, size_type pos, size_type n = npos)
        {
            append(str, pos, n);
        }

        template<typename char_type, typename traits_type = std::char_traits<char_type>, typename allocator_type = std::allocator<char_type>>
        basic_fixed_string(const std::basic_string<char_type, traits_type, allocator_type> & str)
        {
            append(str);
        }

        template<typename char_type, typename traits_type = std::char_traits<char_type>, typename allocator_type = std::allocator<char_type>>
        basic_fixed_string(const std::basic_string<char_type, traits_type, allocator_type> & str, size_type pos, size_type n = npos)
        {
            append(str, pos, n);
        }

        basic_fixed_string(const value_type * s, size_type pos, size_type n = npos)
        {
            append(s, pos, n);
        }

        basic_fixed_string(const value_type * s)
        {
            append(s);
        }

        basic_fixed_string(size_type n, value_type c)
        {
            append(n, c);
        }

        template<typename input_iterator>
        basic_fixed_string(input_iterator begin, input_iterator end)
        {
            append(begin, end);
        }

        basic_fixed_string(std::basic_string_view<value_type, traits_type> sv)
        {
            append(sv);
        }

        basic_fixed_string(std::basic_string_view<value_type, traits_type> sv, size_type pos, size_type n = npos)
        {
            append(sv, pos, n);
        }

        class iterator
        {
        public:
            using difference_type   = std::ptrdiff_t;
            using value_type        = value_type;
            using pointer           = value_type *;
            using reference         = value_type &;
            using iterator_category = std::random_access_iterator_tag;

            iterator(value_type * ptr)
                : ptr{ptr}
            {
            }

            value_type & operator *()
            {
                return *ptr;
            }

            const value_type & operator *() const
            {
                return *ptr;
            }

            value_type & operator &()
            {
                return *ptr;
            }

            const value_type & operator ->() const
            {
                return *ptr;
            }

            value_type & operator ->()
            {
                return *ptr;
            }

            const value_type & operator [](size_type n) const
            {
                return *(ptr + n);
            }

            value_type & operator [](size_type n)
            {
                return *(ptr + n);
            }

            iterator & operator ++()
            {
                ++ptr;
                return *this;
            }

            iterator operator ++(int)
            {
                iterator temp{ptr};
                ++ptr;
                return temp;
            }

            iterator & operator --()
            {
                --ptr;
                return *this;
            }

            iterator operator --(int)
            {
                iterator temp{ptr};
                --ptr;
                return temp;
            }

            iterator & operator +=(size_type n)
            {
                ptr += n;
                return *this;
            }

            iterator operator +(size_type n) const
            {
                return iterator{ptr + n};
            }

            iterator & operator -=(size_type n)
            {
                ptr -= n;
                return *this;
            }

            iterator operator -(size_type n) const
            {
                return iterator{ptr - n};
            }

            difference_type operator -(iterator iter) const
            {
                return ptr - iter.ptr;
            }

            bool operator ==(const iterator & iter) const
            {
                return ptr == iter.ptr;
            }

            bool operator !=(const iterator & iter) const
            {
                return ptr != iter.ptr;
            }

            bool operator >(const iterator & iter) const
            {
                return ptr > iter.ptr;
            }

            bool operator <(const iterator & iter) const
            {
                return ptr < iter.ptr;
            }

            bool operator >=(const iterator & iter) const
            {
                return ptr >= iter.ptr;
            }

            bool operator <=(const iterator & iter) const
            {
                return ptr <= iter.ptr;
            }

        private:
            value_type * ptr;
        };

        class const_iterator
        {
        public:
            using difference_type   = std::ptrdiff_t;
            using value_type        = value_type;
            using pointer           = const value_type *;
            using reference         = const value_type &;
            using iterator_category = std::random_access_iterator_tag;

            const_iterator(const value_type * ptr)
                : ptr{ptr}
            {
            }

            const value_type & operator *() const
            {
                return *ptr;
            }

            const value_type & operator ->() const
            {
                return *ptr;
            }

            const value_type & operator [](size_type n) const
            {
                return *(ptr + n);
            }

            const_iterator & operator ++()
            {
                ++ptr;
                return *this;
            }

            const_iterator operator ++(int)
            {
                const_iterator temp{ptr};
                ++ptr;
                return temp;
            }

            const_iterator & operator --()
            {
                --ptr;
                return *this;
            }

            const_iterator operator --(int)
            {
                const_iterator temp{ptr};
                --ptr;
                return temp;
            }

            const_iterator & operator +=(size_type n)
            {
                ptr += n;
                return *this;
            }

            const_iterator operator +(size_type n) const
            {
                return const_iterator{ptr + n};
            }

            const_iterator & operator -=(size_type n)
            {
                ptr -= n;
                return *this;
            }

            const_iterator operator -(size_type n) const
            {
                return const_iterator{ptr - n};
            }

            difference_type operator -(const_iterator iter) const
            {
                return ptr - iter.ptr;
            }


            bool operator ==(const const_iterator & iter) const
            {
                return ptr == iter.ptr;
            }

            bool operator !=(const const_iterator & iter) const
            {
                return ptr != iter.ptr;
            }

            bool operator >(const const_iterator & iter) const
            {
                return ptr > iter.ptr;
            }

            bool operator <(const const_iterator & iter) const
            {
                return ptr < iter.ptr;
            }

            bool operator >=(const const_iterator & iter) const
            {
                return ptr >= iter.ptr;
            }

            bool operator <=(const const_iterator & iter) const
            {
                return ptr <= iter.ptr;
            }

        private:
            const value_type * ptr;
        };

        iterator begin() noexcept
        {
            return iterator{buffer};
        }

        const_iterator begin() const noexcept
        {
            return const_iterator{buffer};
        }

        iterator end() noexcept
        {
            return iterator{buffer + length_};
        }

        const_iterator end() const noexcept
        {
            return const_iterator{buffer + length_};
        }

        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator{end()};
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator{end()};
        }

        reverse_iterator rend() noexcept
        {
            return reverse_iterator{begin()};
        }

        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator{begin()};
        }

        const_iterator cbegin() const noexcept
        {
            return const_iterator{buffer};
        }

        const_iterator cend() const noexcept
        {
            return const_iterator{buffer + length_};
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator{end()};
        }

        const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator{begin()};
        }

        constexpr size_type size() const noexcept
        {
            return length_;
        }

        constexpr size_type length() const noexcept
        {
            return size();
        }

        constexpr size_type max_size() const noexcept
        {
            return fixed_length_ - 1;
        }

        void resize(size_type, value_type) = delete;

        void resize(size_type) = delete;

        constexpr size_type capacity() const noexcept
        {
            return fixed_length_;
        }

        void reserve() = delete;

        basic_fixed_string & append(const basic_fixed_string & str)
        {
            return append(str.begin(), str.end());
        }

        basic_fixed_string & append(const basic_fixed_string & str, size_type pos, size_type n = npos)
        {
            if (n == npos)
                return append(str.begin() + pos, str.end());
            return append(str.begin() + pos, str.begin() + pos + n);
        }

        template<typename char_type, typename traits_type = std::char_traits<char_type>, typename allocator_type = std::allocator<char_type>>
        basic_fixed_string & append(const std::basic_string<char_type, traits_type, allocator_type> & str)
        {
            return append(str.begin(), str.end());
        }

        template<typename char_type, typename traits_type = std::char_traits<char_type>, typename allocator_type = std::allocator<char_type>>
        basic_fixed_string & append(const std::basic_string<char_type, traits_type, allocator_type> & str, size_type pos, size_type n = npos)
        {
            if (n == npos)
                return append(str.begin() + pos, str.end());
            return append(str.begin() + pos, str.begin() + pos + n);
        }

        basic_fixed_string & append(const value_type * s, size_type n)
        {
            return append(s, std::next(s, n));
        }

        basic_fixed_string & append(const value_type * s)
        {
            const value_type * first = s;
            const value_type * last = s;
            while (*last != value_type{})
                ++last;
            return append(first, last);
        }

        basic_fixed_string & append(size_type n, value_type c)
        {
            size_type copy_length = std::min(n, max_size() - length());
            std::fill(end(), std::next(end, copy_length + 1), c);
            null_terminate();
            return *this;
        }

        template<typename input_iterator>
        basic_fixed_string & append(input_iterator first, input_iterator last)
        {
            size_type copy_length = std::min(static_cast<size_type>(std::distance(first, last)), max_size() - length_);
            std::copy
            (
                first,
                std::next(first, copy_length),
                end()
            );
            (*this)[length_ + copy_length] = value_type{};
            length_ += copy_length;
            return *this;
        }

        basic_fixed_string & append(std::initializer_list<value_type> il)
        {
            return append(il.begin(), il.end());
        }

        basic_fixed_string & append(std::basic_string_view<value_type, traits_type> sv)
        {
            return append(sv.begin(), sv.end());
        }

        basic_fixed_string & append(std::basic_string_view<value_type, traits_type> sv, size_type pos, size_type n = npos)
        {
            if (n == npos)
                return append(sv.begin() + pos, sv.end());
            return append(sv.begin() + pos, sv.begin() + pos + n);
        }

        template<size_type fixed_length_>
        basic_fixed_string & operator +=(const basic_fixed_string<value_type, fixed_length_> & str)
        {
            return append(str);
        }

        basic_fixed_string & operator +=(const value_type * s)
        {
            return append(s);
        }

        basic_fixed_string & operator +=(value_type c)
        {
            if (length() >= max_size())
                return *this;
            (*this)[length()] = c;
            (*this)[length() + 1] = value_type{};
            ++length_;
            return *this;
        }

        basic_fixed_string & operator +=(std::initializer_list<value_type> il)
        {
            return append(il);
        }

        void reserve(size_type) = delete;

        void clear() noexcept
        {
            length_ = 0;
            front() = value_type{};
        };

        bool empty() noexcept
        {
            return size() == 0;
        }

        const_reference operator [](size_type pos) const noexcept
        {
            return buffer[pos];
        }

        reference operator [](size_type pos) noexcept
        {
            return buffer[pos];
        }

        const_reference at(size_type pos) const
        {
            if (pos >= size())
                throw std::out_of_range{"pos >= size()"};
            return operator [](pos);
        }

        reference at(size_type pos)
        {
            if (pos >= size())
                throw std::out_of_range{"pos >= size()"};
            return operator [](pos);
        }

        const value_type & front() const
        {
            return operator [](0);
        }

        value_type & front()
        {
            return operator [](0);
        }

        const value_type & back() const
        {
            return operator [](size() - 1);
        }

        value_type & back()
        {
            return operator [](size() - 1);
        }

        void push_back(value_type c)
        {
            if (size() < max_size())
            {
                operator [](length_) = c;
                operator [](++length_) = value_type{};
            }
        }

        void pop_back()
        {
            erase(size() - 1, 1);
        }

        template<size_type fixed_length_>
        basic_fixed_string & assign(const basic_fixed_string<value_type, fixed_length_> & str)
        {
            length_ = 0;
            return append(str);
        }

        template<size_type fixed_length_>
        basic_fixed_string & assign(const basic_fixed_string<value_type, fixed_length_> & str, size_type pos, size_type n = npos)
        {
            length_ = 0;
            return append(str, pos, n);
        }

        template<typename char_type, typename traits_type = std::char_traits<char_type>, typename allocator_type = std::allocator<char_type>>
        basic_fixed_string & assign(const std::basic_string<char_type, traits_type, allocator_type> & str)
        {
            length_ = 0;
            return append(str.begin(), str.end());
        }

        template<typename char_type, typename traits_type = std::char_traits<char_type>, typename allocator_type = std::allocator<char_type>>
        basic_fixed_string & assign(const std::basic_string<char_type, traits_type, allocator_type> & str, size_type pos, size_type n = npos)
        {
            length_ = 0;
            append(str, pos, n);
        }

        basic_fixed_string & assign(const value_type * s, size_type n)
        {
            length_ = 0;
            return append(s, n);
        }

        basic_fixed_string & assign(const value_type * s)
        {
            length_ = 0;
            return append(s);
        }

        basic_fixed_string & assign(size_type n, value_type c)
        {
            length_ = 0;
            return append(n, c);
        }

        template<typename input_iterator>
        basic_fixed_string & assign(input_iterator first, input_iterator last)
        {
            length_ = 0;
            return append(first, last);
        }

        basic_fixed_string & assign(std::initializer_list<value_type> il)
        {
            length_ = 0;
            return append(li);
        }

        basic_fixed_string & assign(std::basic_string_view<value_type, traits_type> sv)
        {
            length_ = 0;
            return append(sv);
        }

        basic_fixed_string & assign(std::basic_string_view<value_type, traits_type> sv, size_type pos, size_type n = npos)
        {
            length_ = 0;
            return append(sv, pos, n);
        }

        template<size_type fixed_length_>
        basic_fixed_string & insert(size_type pos, const basic_fixed_string<value_type, fixed_length_> & str)
        {
            //TODO
        }


        template<typename traits_type, typename allocator_type>
        operator std::basic_string<value_type, traits_type, allocator_type>() const
        {
            return std::basic_string<value_type, traits_type, allocator_type>(begin(), end());
        }

    private:
        value_type buffer[fixed_length_]{};
        size_type length_{};
    };

    template<typename char_type, std::size_t left_fixed_length, std::size_t right_fixed_length>
    bool operator ==(
        const basic_fixed_string<char_type, left_fixed_length> & left,
        const basic_fixed_string<char_type, left_fixed_length> & right)
    {
        return std::equal(left.begin(), left.end(), right.begin(), right.end());
    }

    template<typename char_type, std::size_t fixed_length>
    bool operator ==(
        const char_type * left,
        const basic_fixed_string<char_type, fixed_length> & right)
    {
        std::basic_string_view<char_type> sv{left};
        return std::equal(sv.begin(), sv.end(), right.begin(), right.end());
    }

    template<typename char_type, std::size_t fixed_length>
    bool operator ==(
        const basic_fixed_string<char_type, fixed_length> & left,
        const char_type * right)
    {
        std::basic_string_view<char_type> sv{right};
        return std::equal(left.begin(), left.end(), sv.begin(), sv.end());
    }

    template<typename char_type, std::size_t left_fixed_length, std::size_t right_fixed_length>
    bool operator !=(
        const basic_fixed_string<char_type, left_fixed_length> & left,
        const basic_fixed_string<char_type, left_fixed_length> & right)
    {
        return !(left == right);
    }

    template<typename char_type, std::size_t fixed_length>
    bool operator !=(
        const char_type * left,
        const basic_fixed_string<char_type, fixed_length> & right)
    {
        return !(left == right);
    }

    template<typename char_type, std::size_t fixed_length>
    bool operator !=(
        const basic_fixed_string<char_type, fixed_length> & left,
        const char_type * right)
    {
        return !(left == right);
    }

    template<std::size_t length>
    using fixed_string = basic_fixed_string<char, length>;

    template<std::size_t length>
    using fixed_wstring = basic_fixed_string<wchar_t, length>;
}

#endif // NOZ_FIXED_STRING_HPP
