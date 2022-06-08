export module cmoon.string.number_string;

import <limits>;
import <array>;
import <iterator>;
import <charconv>;
import <string_view>;
import <utility>;
import <type_traits>;
import <system_error>;
import <cstddef>;

namespace cmoon
{
    export
	template<class T>
        requires(std::is_arithmetic_v<T>)
    class number_string
    {
        static constexpr std::size_t number_max_size {std::numeric_limits<T>::digits10 + 1 + std::is_signed<T>::value};

        std::array<char, number_max_size + 1> data_;
        typename decltype(data_)::const_iterator end_;

        public:
            using value_type = typename decltype(data_)::value_type;
            using size_type = typename decltype(data_)::size_type;
            using difference_type = typename decltype(data_)::difference_type;
            using reference = typename decltype(data_)::reference;
            using const_reference = typename decltype(data_)::const_reference;
            using pointer = typename decltype(data_)::pointer;
            using const_pointer = typename decltype(data_)::const_pointer;
            using iterator = typename decltype(data_)::iterator;
            using const_iterator = typename decltype(data_)::const_iterator;
            using reverse_iterator = typename decltype(data_)::reverse_iterator;
            using const_reverse_iterator = typename decltype(data_)::const_reverse_iterator;

            constexpr number_string() noexcept
                : data_{'0', '\0'}, end_{begin() + 1} {}

            explicit number_string(T t, const int base = 10) requires(std::integral<T>)
                : data_{}
            {
                const auto r {std::to_chars(data(),
                                            data() + data_.size(),
                                            t,
                                            base)};

                if (r.ec != std::errc{})
                {
                    throw std::system_error{std::make_error_code(r.ec)};
                }

                end_ = std::cbegin(data_) + (r.ptr - data_.data());
            }

            explicit number_string(T t, const std::chars_format f = std::chars_format::fixed) requires(std::floating_point<T>)
                : data_{}
            {
                const auto r {std::to_chars(data(),
                                            data() + data_.size(),
                                            t,
                                            f)};

                if (r.ec != std::errc{})
                {
                    throw std::system_error{std::make_error_code(r.ec)};
                }

                end_ = std::cbegin(data_) + (r.ptr - data_.data());
            }

            explicit number_string(T t, const std::chars_format f, int precision) requires(std::floating_point<T>)
                : data_{}
            {
                const auto r {std::to_chars(data(),
                                            data() + data_.size(),
                                            t,
                                            f,
                                            precision)};

                if (r.ec != std::errc{})
                {
                    throw std::system_error{std::make_error_code(r.ec)};
                }

                end_ = std::cbegin(data_) + (r.ptr - data_.data());
            }

            [[nodiscard]] constexpr char* data() noexcept
            {
                return data_.data();
            }

            [[nodiscard]] constexpr const char* data() const noexcept
            {
                return data_.data();
            }

            [[nodiscard]] constexpr std::size_t size() const noexcept
            {
                return std::distance(begin(), end());
            }

            [[nodiscard]] constexpr auto begin() noexcept
            {
                return std::begin(data_);
            }

            [[nodiscard]] constexpr auto begin() const noexcept
            {
                return std::begin(data_);
            }

            [[nodiscard]] constexpr auto cbegin() const noexcept
            {
                return std::cbegin(data_);
            }

            [[nodiscard]] constexpr auto end() noexcept
            {
                return end_;
            }

            [[nodiscard]] constexpr auto end() const noexcept
            {
                return end_;
            }

            [[nodiscard]] constexpr auto cend() const noexcept
            {
                return end_;
            }

            [[nodiscard]] reference operator[](const size_type i) noexcept
            {
                return data_[i];
            }

            [[nodiscard]] const_reference operator[](const size_type i) const noexcept
            {
                return data_[i];
            }

            [[nodiscard]] constexpr operator std::string_view() const noexcept
            {
                return {data(), size()};
            }

            [[nodiscard]] constexpr std::string_view view() const noexcept
            {
                return {data(), size()};
            }

            [[nodiscard]] std::string str() const
            {
                return {begin(), end()};
            }
    };
}