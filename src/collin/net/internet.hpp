#pragma once

#include <cstdint>
#include <array>
#include <exception>
#include <tuple>
#include <functional>
#include <system_error>
#include <sstream>
#include <vector>
#include <limits>
#include <bitset>
#include <forward_list>
#include "../memory.hpp"
#include "../utility.hpp"
#include "socket.hpp"
#include "iocontext.hpp"

namespace collin
{
    namespace net
    {
        namespace ip
        {
            constexpr std::size_t inet_addrstrlen {16};
            constexpr std::size_t inet6_addrstrlen {46};

            enum class resolver_errc
            {
                #ifdef _WIN32
                host_not_found = WSAHOST_NOT_FOUND,
                try_again = WSATRY_AGAIN,
                service_not_found = WSATYPE_NOT_FOUND,
                #else
                host_not_found = EAI_NONAME,
                try_again = EAI_AGAIN,
                service_not_found = EAI_SERVICE,
                #endif
            };

            inline const std::error_category& resolver_category() noexcept
            {
                struct resolver_category_impl : std::error_category
                {
                    virtual const char* name() const noexcept override
                    {
                        return "resolver";
                    }

                    virtual std::string message(int val) const override
                    {
                        switch(static_cast<resolver_errc>(val))
                        {
                            case resolver_errc::host_not_found:
                                return "resolver host not found";
                            case resolver_errc::try_again:
                                return "resolver try again";
                            case resolver_errc::service_not_found:
                                return "service not found";
                        }

                        return "resolver unknown error";
                    }
                };
                static resolver_category_impl rc;

                return rc;
            }

            std::error_code make_error_code(resolver_errc e) noexcept
            {
                return std::error_code(static_cast<int>(e), resolver_category());
            }

            std::error_condition make_error_condition(resolver_errc e) noexcept
            {
                return std::error_condition(static_cast<int>(e), resolver_category());
            }

            using port_type = std::uint_least16_t;
            using scope_id_type = std::uint_least32_t;

            struct v4_mapped_t {};
            constexpr v4_mapped_t v4_mapped;

            class address_v4
            {
                public:
                    using uint_type = std::uint_least32_t;

                    struct BytesType : std::array<unsigned char, 4>
                    {
                        friend address_v4;

                        template<class... T>
                        explicit constexpr BytesType(T... t)
                            : std::array<unsigned char, 4>{{static_cast<unsigned char>(t)...}} {}
                    };

                    constexpr address_v4() noexcept : address{0} {}
                    constexpr address_v4(const address_v4& a) noexcept = default;
                    constexpr address_v4(const address_v4::BytesType& bytes)
                        : address((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]) // Basically hton on it
                    {
                        if constexpr(std::numeric_limits<unsigned char>::max() != 0xFF)
                        {
                            for(const auto& byte : bytes)
                            {
                                if(byte > 0xFF)
                                {
                                    throw std::out_of_range("Address byte is greater than 0xFF");
                                }
                            }
                        }
                    }

                    explicit constexpr address_v4(uint_type val)
                        : address(hton(val))
                    {
                        if constexpr(std::numeric_limits<uint_type>::max() != 0xFFFFFFFF)
                        {
                            if(val > 0xFFFFFFFF)
                            {
                                throw std::out_of_range("Address byte is greater than 0xFFFFFFFF");
                            }
                        }
                    }

                    address_v4& operator=(const address_v4& a) noexcept
                    {
                        if(this != &a)
                        {
                            address = a.address;		
                        }

                        return *this;
                    }

                    constexpr bool is_unspecified() const noexcept
                    {
                        return to_uint() == 0;
                    }

                    constexpr bool is_loopback() const noexcept
                    {
                        return (to_uint() & 0xFF000000) == 0x7F000000;
                    }

                    constexpr bool is_multicast() const noexcept
                    {
                        return (to_uint() & 0xF0000000) == 0xE0000000;
                    }

                    constexpr BytesType to_bytes() const noexcept
                    {
                        return BytesType((address >> 24) && 0xFF,
                                         (address >> 16) && 0xFF,
                                         (address >> 8) && 0xFF,
                                          address && 0xFF);
                    }

                    constexpr uint_type to_uint() const noexcept
                    {
                        return ntoh(address);
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    to_string(const Allocator& a = Allocator()) const
                    {
                        std::basic_string<char, std::char_traits<char>, Allocator> str;
                        str.resize(inet6_addrstrlen);
                        if(::inet_ntop(static_cast<int>(Family::inet), &address, str.data(), str.size()))
                        {
                            str.erase(str.find('\0'));
                        }
                        else
                        {
                            str.resize(0);
                        }

                        return str;
                    }

                    static constexpr address_v4 any() noexcept
                    {
                        return address_v4{};
                    }

                    static constexpr address_v4 loopback() noexcept
                    {
                        return address_v4{0x7F000001};
                    }

                    static constexpr address_v4 broadcast() noexcept
                    {
                        return address_v4{0xFFFFFFFF};
                    }

                private:
                    template<class InternetProcotol>
                    friend class basic_endpoint;

                    uint_type address;
            };

            constexpr bool operator==(const address_v4& a, const address_v4& b) noexcept
            {
                return a.to_uint() == b.to_uint();
            }

            constexpr bool operator!=(const address_v4& a, const address_v4& b) noexcept
            {
                return !(a == b);
            }

            constexpr bool operator<(const address_v4& a, const address_v4& b) noexcept
            {
                return a.to_uint() < b.to_uint();
            }

            constexpr bool operator>(const address_v4& a, const address_v4& b) noexcept
            {
                return b < a;
            }

            constexpr bool operator<=(const address_v4& a, const address_v4& b) noexcept
            {
                return !(b < a);
            }

            constexpr bool operator>=(const address_v4& a, const address_v4& b) noexcept
            {
                return !(a < b);
            }

            address_v4 make_address_v4(std::string_view str)
            {
                address_v4::BytesType bytes;
                std::array<char, inet_addrstrlen> buf;
                const auto len = str.copy(buf.data(), std::size(buf));
                if (len == std::size(buf))
                {
                    throw std::invalid_argument(str.data());
                }

                buf[len] = '\0';

                const auto ret = ::inet_pton(static_cast<int>(Family::inet), buf.data(), bytes.data());
                if(ret == -1)
                {
                    throw std::invalid_argument(str.data());
                }

                return address_v4(bytes);
            }

            address_v4 make_address_v4(const address_v4::BytesType& bytes)
            {
                return address_v4(bytes);
            }

            address_v4 make_address_v4(address_v4::uint_type val)
            {
                return address_v4(val);
            }

            template<class CharT, class Traits>
            std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const address_v4& addr)
            {
                return os << addr.to_string().c_str();
            }

            class address_v6
            {
            public:
                struct BytesType : std::array<unsigned char, 16>
                {
                    friend address_v6;

                    template<class... T>
                    explicit constexpr BytesType(T... t);
                };

                constexpr address_v6() noexcept;
                constexpr address_v6(const address_v6& a) noexcept;
                constexpr address_v6(const BytesType& bytes, scope_id_type scope = 0);

                address_v6& operator=(const address_v6& a) noexcept;

                void scope_id(scope_id_type id) noexcept;

                constexpr scope_id_type scope_id() const noexcept;

                constexpr bool is_unspecified() const noexcept;

                constexpr bool is_loopback() const noexcept;

                constexpr bool is_multicast() const noexcept;

                constexpr bool is_link_local() const noexcept;

                constexpr bool is_site_local() const noexcept;

                constexpr bool is_v4_mapped() const noexcept;

                constexpr bool is_multicast_node_local() const noexcept;

                constexpr bool is_multicast_link_local() const noexcept;

                constexpr bool is_multicast_site_local() const noexcept;

                constexpr bool is_multicast_org_local() const noexcept;

                constexpr bool is_multicast_global() const noexcept;

                constexpr BytesType to_bytes() const noexcept;

                template<class Allocator = std::allocator<char>>
                std::basic_string<char, std::char_traits<char>, Allocator>
                    to_string(const Allocator& a = Allocator()) const
                {
                    std::basic_string<char, std::char_traits<char>, Allocator> str;
                    str.resize(inet6_addrstrlen);
                    if (::inet_ntop(static_cast<int>(Family::inet6), address_.data(), str.data(), str.size()))
                    {
                        str.erase(str.find('\0'));
                    }
                    else
                    {
                        str.resize(0);
                    }

                    return str;
                }

                static constexpr address_v6 any() noexcept;

                static constexpr address_v6 loopback() noexcept;

            private:
                template<class InternetProcotol>
                friend class basic_endpoint;

                friend constexpr bool operator<(const address_v6& a, const address_v6& b) noexcept;
                friend constexpr bool operator==(const address_v6& a, const address_v6& b) noexcept;
                BytesType address_;
                scope_id_type scope_id_;
            };

            constexpr bool operator==(const address_v6& a, const address_v6& b) noexcept;
            constexpr bool operator!=(const address_v6& a, const address_v6& b) noexcept;
            constexpr address_v6 make_address_v6(const address_v6::BytesType& bytes, scope_id_type scope_id = 0);
            constexpr address_v6 make_address_v6(v4_mapped_t, const address_v4& a) noexcept;
            address_v6 make_address_v6(std::string_view str);

            template<class... T>
            constexpr address_v6::BytesType::BytesType(T... t)
                : std::array<unsigned char, 16>{{static_cast<unsigned char>(t)...}} {}

            constexpr address_v6::address_v6() noexcept : address_{}, scope_id_{} {}

            constexpr address_v6::address_v6(const address_v6& a) noexcept = default;

            constexpr address_v6::address_v6(const BytesType& bytes, scope_id_type scope)
                : address_(bytes), scope_id_(scope)
            {
                if constexpr (std::numeric_limits<unsigned char>::max() != 0xFF)
                {
                    for (const auto& byte : bytes)
                    {
                        if (byte > 0xFF)
                        {
                            throw std::out_of_range("Address byte is greater than 0xFF");
                        }
                    }
                }
            }

            address_v6& address_v6::operator=(const address_v6& a) noexcept
            {
                if (this != &a)
                {
                    address_ = a.address_;
                    scope_id_ = a.scope_id_;
                }

                return *this;
            }

            void address_v6::scope_id(scope_id_type id) noexcept
            {
                scope_id_ = id;
            }

            constexpr scope_id_type address_v6::scope_id() const noexcept
            {
                return scope_id_;
            }

            constexpr bool address_v6::is_unspecified() const noexcept
            {
                return *this == any();
            }

            constexpr bool address_v6::is_loopback() const noexcept
            {
                return *this == loopback();
            }

            constexpr bool address_v6::is_multicast() const noexcept
            {
                return address_[0] == 0xFF;
            }

            constexpr bool address_v6::is_link_local() const noexcept
            {
                return address_[0] == 0xFE && (address_[1] & 0xC0) == 0x80;
            }

            constexpr bool address_v6::is_site_local() const noexcept
            {
                return address_[0] == 0xFE && (address_[1] & 0xC0) == 0xC0;
            }

            constexpr bool address_v6::is_v4_mapped() const noexcept
            {
                return address_[0] == 0 && address_[1] == 0 && address_[2] == 0 && address_[3] == 0
                    && address_[4] == 0 && address_[5] == 0 && address_[6] == 0 && address_[7] == 0
                    && address_[8] == 0 && address_[9] == 0 && address_[10] == 0xFF && address_[11] == 0xFF;
            }

            constexpr bool address_v6::is_multicast_node_local() const noexcept
            {
                return is_multicast() && (address_[1] & 0x0F) == 0x01;
            }

            constexpr bool address_v6::is_multicast_link_local() const noexcept
            {
                return is_multicast() && (address_[1] & 0x0F) == 0x02;
            }

            constexpr bool address_v6::is_multicast_site_local() const noexcept
            {
                return is_multicast() && (address_[1] & 0x0F) == 0x05;
            }

            constexpr bool address_v6::is_multicast_org_local() const noexcept
            {
                return is_multicast() && (address_[1] & 0x0F) == 0x08;
            }

            constexpr bool address_v6::is_multicast_global() const noexcept
            {
                return is_multicast() && (address_[1] & 0x0F) == 0x0E;
            }

            constexpr address_v6::BytesType address_v6::to_bytes() const noexcept
            {
                return address_;
            }

            constexpr address_v6 address_v6::any() noexcept
            {
                return address_v6{};
            };

            constexpr address_v6 address_v6::loopback() noexcept
            {
                address_v6 temp;
                temp.address_[15] = 1;
                return temp;
            }

            constexpr bool operator==(const address_v6& a, const address_v6& b) noexcept
            {
                return a.address_[0] == b.address_[0] &&
                       a.address_[1] == b.address_[1] &&
                       a.address_[2] == b.address_[2] &&
                       a.address_[3] == b.address_[3] &&
                       a.address_[4] == b.address_[4] &&
                       a.address_[5] == b.address_[5] &&
                       a.address_[6] == b.address_[6] &&
                       a.address_[7] == b.address_[7] &&
                       a.address_[8] == b.address_[8] &&
                       a.address_[9] == b.address_[9] &&
                       a.address_[10] == b.address_[10] &&
                       a.address_[11] == b.address_[11] &&
                       a.address_[12] == b.address_[12] &&
                       a.address_[13] == b.address_[13] &&
                       a.address_[14] == b.address_[14] &&
                       a.address_[15] == b.address_[15] &&
                       a.scope_id_ == b.scope_id_;
            }

            constexpr bool operator!=(const address_v6& a, const address_v6& b) noexcept
            {
                return !(a == b);
            }

            constexpr bool operator<(const address_v6& a, const address_v6& b) noexcept
            {
                for (std::size_t i = 0; i < std::size(a.address_); i++)
                {
                    if (a.address_[i] < b.address_[i])
                    {
                        return true;
                    }
                    else if (a.address_[i] > b.address_[i])
                    {
                        return false;
                    }
                }

                return a.scope_id_ < b.scope_id_;
            }

            constexpr bool operator>(const address_v6& a, const address_v6& b) noexcept
            {
                return b < a;
            }

            constexpr bool operator<=(const address_v6& a, const address_v6& b) noexcept
            {
                return !(b < a);
            }

            constexpr bool operator>=(const address_v6& a, const address_v6& b) noexcept
            {
                return !(a < b);
            }

            constexpr address_v6 make_address_v6(const address_v6::BytesType& bytes, scope_id_type scope_id)
            {
                return address_v6(bytes, scope_id);
            }

            constexpr address_v6 make_address_v6(v4_mapped_t, const address_v4& a) noexcept
            {
                const auto v4b = a.to_bytes();
                const address_v6::BytesType v6b(0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0xFF, 0xFF, v4b[0], v4b[1], v4b[2], v4b[3]);

                return address_v6(v6b);
            }

            address_v6 make_address_v6(std::string_view str)
            {
                address_v6::BytesType bytes;
                std::array<char, inet6_addrstrlen> buf;
                const auto len = str.copy(buf.data(), std::size(buf));
                if(len == std::size(buf))
                {
                    throw std::invalid_argument(str.data());
                }

                auto ret = ::inet_pton(static_cast<int>(Family::inet6), buf.data(), bytes.data());
                if(ret == -1)
                {
                    throw std::invalid_argument(str.data());
                }

                return address_v6(bytes);
            }

            template<class CharT, class Traits>
            std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const address_v6& addr)
            {
                return os << addr.to_string();
            }

            class BadAddressCast : public std::bad_cast
            {
                public:
                    BadAddressCast() noexcept {}

                    const char* what() noexcept
                    {
                        return "Bad Address Cast";
                    }
            };

            class address
            {
                public:
                    constexpr address() noexcept
                        : version_{address_v4{}}, is_v4_(true) {}
                    constexpr address(const address& a) noexcept
                        : version_{a}, is_v4_(a.is_v4_) {}
                    constexpr address(const address_v4& a) noexcept
                        : version_{a}, is_v4_(true) {}
                    constexpr address(const address_v6& a) noexcept
                        : version_{a}, is_v4_(false) {}

                    address& operator=(const address& a) noexcept
                    {
                        if(this != &a)
                        {
                            version_ = a.version_;
                            is_v4_ = a.is_v4_;
                        }

                        return *this;
                    }

                    address& operator=(const address_v4& a) noexcept
                    {
                        version_.v4_ = a;
                        is_v4_ = true;
                        return *this;
                    }

                    address& operator=(const address_v6& a) noexcept
                    {
                        version_.v6_ = a;
                        is_v4_ = false;
                        return *this;
                    }

                    constexpr bool is_v4() const noexcept
                    {
                        return is_v4_;
                    }

                    constexpr bool is_v6() const noexcept
                    {
                        return !is_v4_;
                    }

                    constexpr address_v4 to_v4() const
                    {
                        if(!is_v4())
                        {
                            throw BadAddressCast();
                        }

                        return version_.v4_;
                    }

                    constexpr address_v6 to_v6() const
                    {
                        if(!is_v6())
                        {
                            throw BadAddressCast();
                        }

                        return version_.v6_;
                    }

                    constexpr bool is_unspecified() const noexcept
                    {
                        if(is_v4())
                        {
                            return version_.v4_.is_unspecified();
                        }

                        return version_.v6_.is_unspecified();
                    }

                    constexpr bool is_loopback() const noexcept
                    {
                        if(is_v4())
                        {
                            return version_.v4_.is_loopback();
                        }

                        return version_.v6_.is_loopback();
                    }

                    constexpr bool is_multicast() const noexcept
                    {
                        if(is_v4())
                        {
                            return version_.v4_.is_multicast();
                        }

                        return version_.v6_.is_multicast();
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    to_string(const Allocator& a = Allocator()) const
                    {
                        if(is_v4())
                        {
                            return version_.v4_.to_string(a);
                        }

                        return version_.v6_.to_string(a);
                    }

                private:
                    template<class InternetProcotol>
                    friend class basic_endpoint;
                
                    union U
                    {
                        address_v4 v4_;
                        address_v6 v6_;
                        bool placeholder; // Need something to initialize when v4_ and v6_ cannot.

                        constexpr U(const address& a)
                            : placeholder {}
                        {
                            if (a.is_v4())
                            {
                                v4_ = a.to_v4();
                            }
                            else
                            {
                                v6_ = a.to_v6();
                            }
                        }

                        constexpr U(const address_v4& v4)
                            : v4_(v4) {}

                        constexpr U(const address_v6& v6)
                            : v6_(v6) {}

                        constexpr U(const U& u)
                            : v4_(u.v4_), v6_(u.v6_) {}

                        U& operator=(const U& u)
                        {
                            v4_ = u.v4_;
                            v6_ = u.v6_;
                            return *this;
                        }
                    } version_;

                    bool is_v4_;
            };

            constexpr bool operator==(const address& a, const address& b) noexcept
            {
                if(a.is_v4() && !b.is_v4())
                {
                    return false;
                }

                if(a.is_v4() && a.to_v4() == b.to_v4())
                {
                    return true;
                }

                return a.to_v6() == b.to_v6();
            }

            constexpr bool operator!=(const address& a, const address& b) noexcept
            {
                return !(a == b);
            }

            constexpr bool operator<(const address& a, const address& b) noexcept
            {
                if(a.is_v4())
                {
                    if(!b.is_v4())
                    {
                        return true;
                    }
                    
                    return a.to_v4() < b.to_v4();
                }

                if(!a.is_v4() && b.is_v4())
                {
                    return false;
                }

                return a.to_v6() < b.to_v6();
            }

            constexpr bool operator>(const address& a, const address& b) noexcept
            {
                return b < a;
            }

            constexpr bool operator<=(const address& a, const address& b) noexcept
            {
                return !(b < a);
            }

            constexpr bool operator>=(const address& a, const address& b) noexcept
            {
                return !(a < b);
            }

            address make_address(std::string_view str)
            {
                try
                {
                    auto addr = make_address_v6(str);
                    return address(addr);
                }
                catch(const std::invalid_argument& e)
                {
                    auto addr = make_address_v4(str);
                    return address(addr);
                }
            }

            template<class CharT, class Traits>
            std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const address& addr)
            {
                return os << addr.to_string().c_str();
            }

            template<typename>
            class basic_address_iterator;

            template<>
            class basic_address_iterator<address_v4>
            {
                public:
                    using value_type = address_v4;
                    using difference_type = std::ptrdiff_t;
                    using pointer = const address_v4*;
                    using reference = const address_v4&;
                    using iterator_category = std::input_iterator_tag;

                    basic_address_iterator(const address_v4& a) noexcept
                        : address_(a) {}

                    reference operator*() const noexcept
                    {
                        return address_;
                    }

                    pointer operator->() const noexcept
                    {
                        return &address_;
                    }

                    basic_address_iterator& operator++() noexcept
                    {
                        address_ = value_type(address_.to_uint() + 1);
                        return *this;
                    }

                    basic_address_iterator operator++(int) noexcept
                    {
                        auto temp = *this;
                        ++*this;
                        return temp;
                    }

                    basic_address_iterator& operator--() noexcept
                    {
                        address_ = value_type(address_.to_uint() - 1);
                        return *this;
                    }

                    basic_address_iterator operator--(int) noexcept
                    {
                        auto temp = *this;
                        --*this;
                        return temp;
                    }

                    bool operator==(const basic_address_iterator& rhs) const noexcept
                    {
                        return address_ == rhs.address_;
                    }

                    bool operator!=(const basic_address_iterator& rhs) const noexcept
                    {
                        return !(*this == rhs);
                    }
                private:
                    address_v4 address_;
            };

            using address_v4_iterator = basic_address_iterator<address_v4>;

            template<>
            class basic_address_iterator<address_v6>
            {
                public:
                    using value_type = address_v6;
                    using difference_type = std::ptrdiff_t;
                    using pointer = const address_v6*;
                    using reference = const address_v6&;
                    using iterator_category = std::input_iterator_tag;

                    basic_address_iterator(const address_v6& a) noexcept
                        : address_(a) {}

                    reference operator*() const noexcept
                    {
                        return address_;
                    }

                    pointer operator->() const noexcept
                    {
                        return &address_;
                    }

                    basic_address_iterator& operator++() noexcept
                    {
                        address_v6::BytesType b = address_.to_bytes();
                        for (int i = std::size(b) - 1; i > 0; i--)
                        {
                            if(b[i] != 0xff)
                            {
                                b[i]++;
                                break;
                            }
                        }

                        address_ = value_type(b);
                        return *this;
                    }

                    basic_address_iterator operator++(int) noexcept
                    {
                        auto temp = *this;
                        ++* this;
                        return temp;
                    }

                    basic_address_iterator& operator--() noexcept
                    {
                        address_v6::BytesType b = address_.to_bytes();
                        for(int i = 0; i < std::size(b); i++)
                        {
                            if (b[i] != 0)
                            {
                                b[i]--;
                                break;
                            }
                        }

                        address_ = value_type(b);
                        return *this;
                    }

                    basic_address_iterator operator--(int) noexcept
                    {
                        auto temp = *this;
                        --* this;
                        return temp;
                    }

                    bool operator==(const basic_address_iterator& rhs) const noexcept
                    {
                        return address_ == rhs.address_;
                    }

                    bool operator!=(const basic_address_iterator& rhs) const noexcept
                    {
                        return !(*this == rhs);
                    }
                private:
                    address_v6 address_;
            };

            using address_v6_iterator = basic_address_iterator<address_v6>;

            template<typename>
            class basic_address_range;

            template<>
            class basic_address_range<address_v4>
            {
                public:
                    using iterator = basic_address_iterator<address_v4>;

                    basic_address_range() noexcept
                        : begin_({}), end_({}) {}

                    basic_address_range(const address_v4& first, const address_v4& last) noexcept
                        : begin_(first), end_(last) {}

                    iterator begin() const noexcept
                    {
                        return begin_;
                    }

                    iterator end() const noexcept
                    {
                        return end_;
                    }

                    bool empty() const noexcept
                    {
                        return begin_ == end_;
                    }

                    std::size_t size() const noexcept
                    {
                        return end_->to_uint() - begin_->to_uint();
                    }

                    iterator find(const address_v4& addr) const noexcept
                    {
                        if (*begin_ <= addr && addr < *end_)
                        {
                            return iterator{addr};
                        }

                        return end();
                    }
                private:
                    iterator begin_;
                    iterator end_;
            };

            using address_v4_range = basic_address_range<address_v4>;

            template<>
            class basic_address_range<address_v6>
            {
                public:
                    using iterator = basic_address_iterator<address_v6>;

                    basic_address_range() noexcept
                        : begin_({}), end_({}) {}

                    basic_address_range(const address_v6& first, const address_v6& last) noexcept
                        : begin_(first), end_(last) {}

                    iterator begin() const noexcept
                    {
                        return begin_;
                    }

                    iterator end() const noexcept
                    {
                        return end_;
                    }

                    bool empty() const noexcept
                    {
                        return begin_ == end_;
                    }

                    iterator find(const address_v6& addr) const noexcept
                    {
                        if (*begin_ <= addr && addr < *end_)
                        {
                            return iterator{addr};
                        }

                        return end();
                    }
                private:
                    iterator begin_;
                    iterator end_;
            };

            using address_v6_range = basic_address_range<address_v6>;

            class network_v4;

            bool operator==(const network_v4& a, const network_v4& b) noexcept;

            class network_v4
            {
                public:
                    constexpr network_v4() noexcept
                        : addr_{}, prefix_len_{0} {}

                    constexpr network_v4(const address_v4& addr, int prefix_len)
                        : addr_{addr}, prefix_len_{prefix_len}
                    {
                        if (prefix_len < 0 || prefix_len > 32)
                        {
                            throw std::out_of_range("network_v4: invalid prefix length");
                        }
                    }

                    constexpr network_v4(const address_v4& addr, const address_v4& mask)
                        : addr_{addr}, prefix_len_{static_cast<int>(memory::bitset<sizeof(typename address_v4::uint_type)>{mask.to_uint()}.count())}
                    {
                        if (prefix_len_ != 0)
                        {
                            const auto mask_uint = mask.to_uint();
                            if ((mask_uint & 0x80000000) == 0)
                            {
                                throw std::invalid_argument("network_v4: invalid mask");
                            }
                        }
                    }

                    constexpr address_v4 address() const noexcept
                    {
                        return addr_;
                    }

                    constexpr int prefix_length() const noexcept
                    {
                        return prefix_len_;
                    }

                    constexpr address_v4 netmask() const noexcept
                    {
                        auto val = address_v4::broadcast().to_uint();
                        val >>= (32 - prefix_len_);
                        val <<= (32 - prefix_len_);
                        return address_v4{val};
                    }

                    constexpr address_v4 network() const noexcept
                    {
                        return address_v4{addr_.to_uint() & netmask().to_uint()};
                    }

                    constexpr address_v4 broadcast() const noexcept
                    {
                        return address_v4{addr_.to_uint() | ~netmask().to_uint()};
                    }

                    constexpr network_v4 canonical() const noexcept
                    {
                        return network_v4(network(), prefix_length());
                    }

                    constexpr bool is_host() const noexcept
                    {
                        return prefix_len_ == 32;
                    }

                    address_v4_range hosts() const noexcept
                    {
                        if(is_host())
                        {
                            return {address(), *++address_v4_iterator(address())};
                        }

                        return {network(), broadcast()};
                    }

                    constexpr bool is_subnet_of(const network_v4& other) const noexcept
                    {
                        if (other.prefix_length() < prefix_length())
                        {
                            network_v4 net(address(), other.prefix_length());
                            return net.canonical() == other.canonical();
                        }

                        return false;
                    }

                    template<typename Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator> to_string(const Allocator& a = Allocator()) const
                    {
                        return address().to_string() + '/' + std::to_string(prefix_length());
                    }
                private:
                    address_v4 addr_;
                    int prefix_len_;
            };

            class network_v6;

            bool operator==(const network_v6& a, const network_v6& b) noexcept;

            class network_v6
            {
                public:
                    constexpr network_v6() noexcept
                        : addr_{}, prefix_len_{ 0 } {}

                    constexpr network_v6(const address_v6& addr, int prefix_len)
                        : addr_{ addr }, prefix_len_{ prefix_len }
                    {
                        if (prefix_len < 0 || prefix_len > 128)
                        {
                            throw std::out_of_range("network_v4: invalid prefix length");
                        }
                    }

                    constexpr address_v6 address() const noexcept
                    {
                        return addr_;
                    }

                    constexpr int prefix_length() const noexcept
                    {
                        return prefix_len_;
                    }

                    constexpr address_v6 network() const noexcept; // TODO

                    constexpr network_v6 canonical() const noexcept
                    {
                        return network_v6(network(), prefix_length());
                    }

                    constexpr bool is_host() const noexcept
                    {
                        return prefix_len_ == 32;
                    }

                    address_v6_range hosts() const noexcept
                    {
                        if (is_host())
                        {
                            return { address(), *++address_v6_iterator(address()) };
                        }

                        return {};
                    }

                    constexpr bool is_subnet_of(const network_v6& other) const noexcept
                    {
                        if (other.prefix_length() < prefix_length())
                        {
                            network_v6 net(address(), other.prefix_length());
                            return net.canonical() == other.canonical();
                        }

                        return false;
                    }

                    template<typename Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator> to_string(const Allocator& a = Allocator()) const
                    {
                        return address().to_string() + '/' + std::to_string(prefix_length());
                    }
                private:
                    address_v6 addr_;
                    int prefix_len_;
            };

            inline bool operator==(const network_v4& a, const network_v4& b) noexcept
            {
                return a.address() == b.address() && a.prefix_length() == b.prefix_length();
            }

            inline bool operator!=(const network_v4& a, const network_v4& b) noexcept
            {
                return !(a== b);
            }

            inline bool operator==(const network_v6& a, const network_v6& b) noexcept
            {
                return a.address() == b.address() && a.prefix_length() == b.prefix_length();
            }

            inline bool operator!=(const network_v6& a, const network_v6& b) noexcept
            {
                return !(a == b);
            }

            inline network_v4 make_network_v4(const address_v4& a, int prefix_length)
            {
                return network_v4{a, prefix_length};
            }

            network_v4 make_network_v4(std::string_view str); // TODO

            inline network_v6 make_network_v6(const address_v6& a, int prefix_length)
            {
                return network_v6{a, prefix_length};
            }

            network_v6 make_network_v6(std::string_view str); // TODO

            template<class CharT, class Traits>
            inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const network_v4& net)
            {
                return os << net.to_string();
            }

            template<class CharT, class Traits>
            inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const network_v6& net)
            {
                return os << net.to_string();
            }

            template<class InternetProtocol>
            class basic_endpoint
            {
                private:
                    using address_t = net::ip::address;
                public:
                    using protocol_type = InternetProtocol;

                    constexpr basic_endpoint() noexcept
                        : data_{}
                    {
                        data_.v4_.sin_family = static_cast<int>(protocol_type::v4().family());
                    }

                    constexpr basic_endpoint(const protocol_type& proto, port_type port_num) noexcept
                        : data_{}
                    {
                        data_.v4_.sin_family = proto.family();
                        dat_.v4_.sin_port = hton(port_num);
                    }

                    constexpr basic_endpoint(const address& addr, port_type port_num) noexcept
                        : data_{}
                    {
                        if (addr.is_v4())
                        {
                            data_.v4_.sin_family = static_cast<int>(protocol_type::v4().family());
                            data_.v4_.sin_port = hton(port_num);
                            data_.v4_.sin_addr.s_addr = addr.to_v4().to_uint();
                        }
                        else
                        {
                            data_.v6_ = {};
                            data_.v6_.sin6_family = static_cast<int>(protocol_type::v6().family());
                            data_.v6_.sin6_port = hton(port_num);
                            std::memcpy(data_.v6_.sin6_addr.s6_addr, addr.to_v6().address_.data(), 16);
                            data_.v6_.sin6_scope_id = addr.version_.v6_.scope_id_;
                        }
                    }
                    
                    constexpr protocol_type protocol() const noexcept
                    {
                        return static_cast<Family>(data_.v4_.sin_family) == Family::inet6 ? protocol_type::v6() : protocol_type::v4();
                    }

                    constexpr address address() const noexcept
                    {
                        ip::address addr;

                        if(protocol().family() == Family::inet6)
                        {
                            std::memcpy(&addr.version_.v6_.address_, data_.v6_.sin6_addr.s6_addr, 16);
                            addr.is_v4_ = false;
                        }
                        else
                        {
                            std::memcpy(&addr.version_.v4_.address, &data_.v4_.sin_addr.s_addr, 4);
                        }

                        return addr;
                    }

                    void address(const net::ip::address addr) noexcept
                    {
                        if (addr.is_v6())
                        {
                            data_.v6_ = {};
                            data_.v6_.sin6_family = static_cast<int>(protocol_type::v6().family());
                            std::memcpy(data_.v6_.sin6_addr.s6_addr, addr.version_.v6_.address_.data(), 16);
                            data_.v6_.sin6_scope_id = addr.version_.v6_.scope_id_;
                        }
                        else
                        {
                            data_.v4_.sin_family = static_cast<int>(protocol_type::v4().family());
                            data_.v4_.sin_addr.s_addr = addr.version_.v4_.address;
                        }
                    }

                    constexpr port_type port() const noexcept
                    {
                        return ntoh(data_.v4_.sin_port);
                    }

                    void port(port_type port_num) noexcept
                    {
                        data_.v4_.sin_port = hton(port_num);
                    }

                    void* data() noexcept
                    {
                        return std::addressof(data_);
                    }

                    const void* data() const noexcept
                    {
                        return std::addressof(data_);
                    }

                    constexpr std::size_t size() const noexcept
                    {
                        return protocol().family() == Family::inet6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);
                    }

                    void resize(std::size_t s)
                    {
                        // ?
                    }

                    constexpr std::size_t capacity() const noexcept
                    {
                        return sizeof(data_);
                    }

                private:
                    union
                    {
                        sockaddr_in v4_;
                        sockaddr_in6 v6_;
                    } data_;
            };

            template<class InternetProtocol>
            constexpr bool operator==(const basic_endpoint<InternetProtocol>& a, const basic_endpoint<InternetProtocol>& b) noexcept
            {
                return a.address() == b.address() && a.port() == b.port();
            }

            template<class InternetProtocol>
            constexpr bool operator!=(const basic_endpoint<InternetProtocol>& a, const basic_endpoint<InternetProtocol>& b) noexcept
            {
                return !(a == b);
            }

            template<class InternetProtocol>
            constexpr bool operator<(const basic_endpoint<InternetProtocol>& a, const basic_endpoint<InternetProtocol>& b) noexcept
            {
                return std::tie(a.address(), a.port()) < std::tie(b.address(), b.port());
            }

            template<class InternetProtocol>
            constexpr bool operator>(const basic_endpoint<InternetProtocol>& a, const basic_endpoint<InternetProtocol>& b) noexcept
            {
                return b < a;
            }

            template<class InternetProtocol>
            constexpr bool operator<=(const basic_endpoint<InternetProtocol>& a, const basic_endpoint<InternetProtocol>& b) noexcept
            {
                return !(b < a);
            }

            template<class InternetProtocol>
            constexpr bool operator>=(const basic_endpoint<InternetProtocol>& a, const basic_endpoint<InternetProtocol>& b) noexcept
            {
                return !(a < b);
            }

            template<class CharT, class Traits, class InternetProcotol>
            std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_endpoint<InternetProcotol>& ep)
            {
                std::basic_ostringstream<CharT, Traits> ss;
                if(ep.protocol() == basic_endpoint<InternetProcotol>::protocol_type::v6())
                {
                    ss << "[" << ep.address() << "]";
                }
                else
                {
                    ss << ep.address();
                }

                ss << ":" << ep.port();
                return os << ss.str();
            }

            template<class InternetProtocol>
            class basic_resolver_entry
            {
                public:
                    using protocol_type = InternetProtocol;
                    using endpoint_type = typename InternetProtocol::endpoint;

                    basic_resolver_entry() {}

                    basic_resolver_entry(const endpoint_type& ep, std::string_view h, std::string_view s)
                        : endpoint_(ep), host_name_(h), service_name_(s) {}

                    endpoint_type endpoint() const
                    {
                        return endpoint_;
                    }

                    operator endpoint_type() const
                    {
                        return endpoint_;
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    host_name(const Allocator& a = Allocator()) const
                    {
                        return {host_name_, a};
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    service_name(const Allocator& a = Allocator()) const
                    {
                        return {service_name_, a};
                    }
                private:
                    endpoint_type endpoint_;
                    std::string host_name_;
                    std::string service_name_;
            };

            template<class InternetProtocol>
            bool operator==(const basic_resolver_entry<InternetProtocol>& a, const basic_resolver_entry<InternetProtocol>& b)
            {
                return a.endpoint() == b.endpoint() && a.host_name() == b.host_name() && a.service_name() == b.service_name();
            }

            template<class InternetProtocol>
            bool operator!=(const basic_resolver_entry<InternetProtocol>& a, const basic_resolver_entry<InternetProtocol>& b)
            {
                return !(a == b);
            }

            template<class InternetProtocol>
            class basic_resolver;

            template<class InternetProtocol>
            class basic_resolver_results
            {
                public:
                    using protocol_type = InternetProtocol;
                    using endpoint_type = typename protocol_type::endpoint;
                    using value_type = basic_resolver_entry<protocol_type>;
                    using const_reference = const value_type&;
                    using reference = value_type&;
                    using const_iterator = typename std::forward_list<value_type>::const_iterator;
                    using iterator = const_iterator;
                    using difference_type = std::ptrdiff_t;
                    using size_type = std::size_t;

                    basic_resolver_results() = default;
                    basic_resolver_results(const basic_resolver_results&) = default;
                    basic_resolver_results(basic_resolver_results&&) noexcept = default;
                    basic_resolver_results& operator=(const basic_resolver_results&) = default;
                    basic_resolver_results& operator=(basic_resolver_results&&) = default;
                    ~basic_resolver_results() = default;

                    size_type size() const noexcept
                    {
                        return size_;
                    }

                    size_type max_size() const noexcept
                    {
                        return results.max_size();
                    }

                    bool empty() const noexcept
                    {
                        return size_ == 0;
                    }

                    const_iterator begin() const
                    {
                        return std::cbegin(results);
                    }

                    const_iterator end() const
                    {
                        return std::cend(results);
                    }

                    const_iterator cbegin() const
                    {
                        return std::cbegin(results);
                    }

                    const_iterator cend() const
                    {
                        return std::cend(results);
                    }

                    void swap(basic_resolver_results& that) noexcept
                    {
                        std::swap(results, that.results);
                    }

                private:
                    friend basic_resolver<InternetProtocol>;

                    std::forward_list<value_type> results;
                    std::size_t size_ {0};

                    basic_resolver_results(std::string_view host_name, std::string_view service_name, const ::addrinfo& hints)
                    {
                        std::string host;
                        std::string service;

                        const char* h = host_name.data() ? (host = std::string(host_name)).c_str() : nullptr;
                        const char* s = service_name.data() ? (service = std::string(service_name)).c_str() : nullptr;

                        struct scoped_addrinfo
                        {
                            ~scoped_addrinfo()
                            {
                                if (p)
                                {
                                    ::freeaddrinfo(p);
                                }
                            }

                            ::addrinfo* p{ nullptr };
                        } address_results;

                        if (auto err = ::getaddrinfo(h, s, &hints, &address_results.p) != 0)
                        {
                            throw std::system_error(err, resolver_category(), "Cannot resolve address");
                        }

                        auto tail = results.before_begin();
                        endpoint_type ep;

                        for (auto ai = address_results.p; ai != nullptr; ai = ai->ai_next)
                        {
                            if (static_cast<Family>(ai->ai_family) == Family::inet ||
                                static_cast<Family>(ai->ai_family) == Family::inet6)
                            {
                                if (ai->ai_addrlen <= ep.capacity())
                                {
                                    std::memcpy(ep.data(), ai->ai_addr, ai->ai_addrlen);
                                    ep.resize(ai->ai_addrlen);
                                    tail = results.emplace_after(tail, ep, host, service);
                                    size_++;
                                }
                            }
                        }
                    }

                    basic_resolver_results(const endpoint_type& ep)
                    {
                        std::array<char, 256> host_name;
                        std::array<char, 128> service_name;
                        int flags = 0;

                        if(ep.protocol().type() == Type::dgram)
                        {
                            flags |= NI_DGRAM;
                        }

                        auto sa = static_cast<const sockaddr*>(ep.data());
                        auto err = ::getnameinfo(sa, ep.size(), host_name, std::size(host_name), service_name, std::size(service_name), flags);

                        if(err)
                        {
                            flags |= NI_NUMERICSERV;
                            err = ::getnameinfo(sa, ep.size(), host_name, std::size(host_name), service_name, std::size(service_name), flags);
                        }

                        if(err)
                        {
                            throw std::system_error(err, resolver_category(), "Cannot resolve endpoint");
                        }
                        else
                        {
                            results.emplace_front(ep, host_name, service_name);
                            size_ = 1;
                        }
                    }
            };

            template<class InternetProtocol>
            bool operator==(const basic_resolver_results<InternetProtocol>& a, const basic_resolver_results<InternetProtocol>& b)
            {
                return a.size() == b.size() && std::equal(a.cbegin(), a.cend(), b.cbegin());
            }

            template<class InternetProtocol>
            bool operator!=(const basic_resolver_results<InternetProtocol>& a, const basic_resolver_results<InternetProtocol>& b)
            {
                return !(a == b);
            }

            class resolver_base
            {
                public:
                    enum class flags : int
                    {
                        flags_passive = AI_PASSIVE,
                        flags_canonical_name = AI_CANONNAME,
                        flags_numeric_host = AI_NUMERICHOST,
                        flags_numeric_service = AI_NUMERICSERV,
                        flags_v4_mapped = AI_V4MAPPED,
                        flags_all_matching = AI_ALL,
                        flags_address_configured = AI_ADDRCONFIG
                    };
                    static constexpr flags passive = flags::flags_passive;
                    static constexpr flags canonical_name = flags::flags_canonical_name;
                    static constexpr flags numeric_host = flags::flags_numeric_host;
                    static constexpr flags numeric_service = flags::flags_numeric_service;
                    static constexpr flags v4_mapped = flags::flags_v4_mapped;
                    static constexpr flags all_matching = flags::flags_all_matching;
                    static constexpr flags address_configured = flags::flags_address_configured;

                protected:
                    resolver_base() {}
                    ~resolver_base() {}
            };

            constexpr resolver_base::flags operator&(resolver_base::flags f1, resolver_base::flags f2)
            {
                return resolver_base::flags{static_cast<int>(f1) & static_cast<int>(f2)};
            }

            constexpr resolver_base::flags operator|(resolver_base::flags f1, resolver_base::flags f2)
            {
                return resolver_base::flags{static_cast<int>(f1) | static_cast<int>(f2)};
            }

            constexpr resolver_base::flags operator^(resolver_base::flags f1, resolver_base::flags f2)
            {
                return resolver_base::flags{static_cast<int>(f1) ^ static_cast<int>(f2)};
            }

            constexpr resolver_base::flags operator~(resolver_base::flags f)
            {
                return resolver_base::flags{~static_cast<int>(f)};
            }

            inline resolver_base::flags operator&=(resolver_base::flags& f1, resolver_base::flags f2)
            {
                return f1 = (f1 & f2);
            }

            inline resolver_base::flags operator|=(resolver_base::flags& f1, resolver_base::flags f2)
            {
                return f1 = (f1 | f2);
            }

            inline resolver_base::flags operator^=(resolver_base::flags& f1, resolver_base::flags f2)
            {
                return f1 = (f1 ^ f2);
            }

            template<class InternetProtocol>
            class basic_resolver : public resolver_base // Reimplement with executors when possible
            {
                public:
                    using protocol_type = InternetProtocol;
                    using endpoint_type = typename InternetProtocol::endpoint;
                    using results_type = basic_resolver_results<InternetProtocol>;

                    explicit basic_resolver(io_context& ctx) {}
                    basic_resolver(const basic_resolver&) = delete;
                    basic_resolver(basic_resolver&& rhs) noexcept {}

                    ~basic_resolver()
                    {
                        cancel();
                    }

                    basic_resolver& operator=(const basic_resolver&) = delete;
                    basic_resolver& operator=(basic_resolver&& rhs)
                    {
                        rhs.cancel();
                        return *this;
                    }

                    void cancel()
                    {
                        // Nothing for now
                    }

                    results_type resolve(std::string_view host_name, std::string_view service_name)
                    {
                        return resolve(host_name, service_name, resolver_base::flags());
                    }

                    results_type resolve(std::string_view host_name, std::string_view service_name, flags f)
                    {
                        constexpr auto end = endpoint_type();
                        constexpr auto family = end.protocol().family();
                        constexpr auto type = end.protocol().type();
                        constexpr auto protocol = end.protocol().protocol();

                        const auto hints = make_hints(f, family, type, protocol);
                        
                        return get_results(host_name, service_name, hints);
                    }

                    std::future<results_type> async_resolve(std::string_view host_name, std::string_view service_name)
                    {
                        return async_resolve(host_name, service_name, resolver_base::flags());
                    }

                    std::future<results_type> async_resolve(std::string_view host_name, std::string_view service_name, flags f)
                    {
                        return std::async(resolve, host_name, service_name, f);
                    }

                    results_type resolve(const protocol_type& protocol, std::string_view host_name, std::string_view service_name)
                    {
                        return resolve(protocol, host_name, service_name, resolver_base::flags());
                    }

                    results_type resolve(const protocol_type& protocol, std::string_view host_name, std::string_view service_name, flags f)
                    {
                        const auto hints = make_hints(f, protocol.family(), protocol.type(), protocol.protocol());
                        return get_results(host_name, service_name, hints);
                    }

                    std::future<results_type> async_resolve(const protocol_type& protocol, std::string_view host_name, std::string_view service_name)
                    {
                        return async_resolve(protocol, host_name, service_name, resolver_base::flags());
                    }

                    std::future<results_type> async_resolve(const protocol_type& protocol, std::string_view host_name, std::string_view service_name, flags f)
                    {
                        return std::async(resolve, std::ref(protocol), std::ref(host_name), std::ref(service_name), f);
                    }

                private:
                    enum class name_info_flags
                    {
                        nofqdn = NI_NOFQDN,
                        numeric_host = NI_NUMERICHOST,
                        name_reqd = NI_NAMEREQD,
                        numeric_service = NI_NUMERICSERV,
                        #ifndef _WIN32
                        numeric_scope = NI_NUMERICSCOPE,
                        #endif
                        dgram = NI_DGRAM
                    };
                    static constexpr socklen_t host_name_length {1025};
                    static constexpr socklen_t service_name_length {32};

                    constexpr addrinfo make_hints(flags f, Family family, Type type, Protocol protocol)
                    {
                        addrinfo hints;
                        std::memset(&hints, 0, sizeof(hints));
                        hints.ai_flags = static_cast<int>(f);
                        hints.ai_family = static_cast<int>(family);
                        hints.ai_socktype = static_cast<int>(type);
                        hints.ai_protocol = static_cast<int>(protocol);

                        return hints;
                    }

                    results_type get_results(std::string_view host_name, std::string_view service_name, const addrinfo& hints)
                    {
                        return {host_name, service_name, hints};
                    }
            };

            template<class Allocator>
            std::basic_string<char, std::char_traits<char>, Allocator>
                host_name(const Allocator& a)
            {
                std::array<char, 256 + 1> buf;
                if (::gethostname(buf.data(), std::size(buf) - 1) == -1)
                {
                    throw std::exception("error fetching host_name");
                }

                buf[std::size(buf) - 1] = '\0';

                return {buf.data(), a};
            }

            inline std::string host_name()
            {
                return host_name(std::allocator<char>{});
            }

            class tcp
            {
                public:
                    using endpoint = basic_endpoint<tcp>;
                    using resolver = basic_resolver<tcp>;
                    using socket = basic_stream_socket<tcp>;
                    using acceptor = basic_socket_acceptor<tcp>;
                    using iostream = basic_socket_iostream<tcp>;

                    static inline constexpr tcp v4() noexcept
                    {
                        return tcp(Family::inet);
                    }

                    static inline constexpr tcp v6() noexcept
                    {
                        return tcp(Family::inet6);
                    }

                    constexpr Family family() const noexcept
                    {
                        return family_;
                    }

                    constexpr Type type() const noexcept
                    {
                        return Type::stream;
                    }

                    constexpr Protocol protocol() const noexcept
                    {
                        return Protocol::tcp;
                    }

                    tcp() = delete;
                private:
                    constexpr tcp(Family f)
                        : family_(f) {}

                    Family family_;
            };

            constexpr bool operator==(const tcp& a, const tcp& b) noexcept
            {
                return a.family() == b.family();
            }

            constexpr bool operator!=(const tcp& a, const tcp& b) noexcept
            {
                return !(a == b);
            }

            class udp
            {
                public:
                    using endpoint = basic_endpoint<udp>;
                    using resolver = basic_resolver<udp>;
                    using socket = basic_datagram_socket<udp>;

                    static constexpr udp v4() noexcept
                    {
                        return udp(Family::inet);
                    }

                    static constexpr udp v6() noexcept
                    {
                        return udp(Family::inet6);
                    }

                    constexpr Family family() const noexcept
                    {
                        return family_;
                    }

                    constexpr Type type() const noexcept
                    {
                        return Type::dgram;
                    }

                    constexpr Protocol protocol() const noexcept
                    {
                        return Protocol::udp;
                    }

                    udp() = delete;
                private:
                    constexpr udp(Family f)
                        : family_(f) {}

                    Family family_;
            };

            constexpr bool operator==(const udp& a, const udp& b) noexcept
            {
                return a.family() == b.family();
            }

            constexpr bool operator!=(const udp& a, const udp& b) noexcept
            {
                return !(a == b);
            }
        }
    }
}

namespace std
{
    template<>
    struct hash<collin::net::ip::address>
    {
        using result_type = std::size_t;
        using argument_type = collin::net::ip::address;

        std::size_t operator()(const argument_type& a) const
        {
            if (a.is_v4())
            {
                const auto a4 = a.to_v4();
                return collin::hash_bytes(&a4, sizeof(a4), 0xc70f6907UL);
            }
            else
            {
                const auto a6 = a.to_v6();
                return collin::hash_bytes(&a6, sizeof(a6), 0xc70f6907UL);
            }
        }
    };

    template<>
    struct hash<collin::net::ip::address_v4>
    {
        using result_type = std::size_t;
        using argument_type = collin::net::ip::address_v4;

        std::size_t operator()(const argument_type& a) const
        {
            const auto b = a.to_bytes();
            return collin::hash_bytes(b.data(), sizeof(b), 0xc70f6907UL);
        }
    };

    template<>
    struct hash<collin::net::ip::address_v6>
    {
        using result_type = std::size_t;
        using argument_type = collin::net::ip::address_v6;

        std::size_t operator()(const argument_type& a) const
        {
            const auto b = a.to_bytes();
            return collin::hash_bytes(b.data(), sizeof(b), 0xc70f6907UL);
        }
    };
}