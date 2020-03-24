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
#include "collin_socket.hpp"
#include "collin_iocontext.hpp"

namespace collin
{
    namespace net
    {
        namespace ip
        {
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

                        private:
                            static constexpr address_v4::uint_type to_uint(const BytesType& bytes)
                            {
                                return (std::get<0>(bytes) << 24) | (std::get<1>(bytes) << 16) | (std::get<2>(bytes) << 8) | std::get<3>(bytes);
                            }
                    };

                    constexpr address_v4() noexcept {}
                    constexpr address_v4(const address_v4& a) noexcept
                        : address(a.address) {}
                    constexpr address_v4(const address_v4::BytesType& bytes)
                        : address(BytesType::to_uint(bytes))
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
                        : address(val)
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
                        const auto network_based = to_uint();
                        return BytesType(static_cast<unsigned char>((network_based >> 24) && 0xFF),
                                            static_cast<unsigned char>((network_based >> 16) && 0xFF),
                                            static_cast<unsigned char>((network_based >> 8) && 0xFF),
                                            static_cast<unsigned char>(network_based && 0xFF));
                    }

                    constexpr uint_type to_uint() const noexcept
                    {
                        return htonl(address);
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    to_string(const Allocator& a = Allocator()) const
                    {
                        return inet_ntop(Family::inet, (void*) &address);
                    }

                    static constexpr address_v4 any() noexcept
                    {
                        return address_v4();
                    }

                    static constexpr address_v4 loopback() noexcept
                    {
                        return address_v4(0x7F000001);
                    }

                    static constexpr address_v4 broadcast() noexcept
                    {
                        return address_v4(0xFFFFFFFF);
                    }

                private:
                    uint_type address {0};
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
                auto ret = inet_pton(Family::inet, str, bytes.data(), nullptr);
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
                    return inet_ntop(Family::inet6, address_.data(), scope_id_);
                }

                static constexpr address_v6 any() noexcept;

                static constexpr address_v6 loopback() noexcept;

            private:
                friend constexpr bool operator<(const address_v6& a, const address_v6& b) noexcept;
                friend constexpr bool operator==(const address_v6& a, const address_v6& b) noexcept;
                BytesType address_{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                scope_id_type scope_id_{ 0 };
            };

            constexpr bool operator==(const address_v6& a, const address_v6& b) noexcept;
            constexpr bool operator!=(const address_v6& a, const address_v6& b) noexcept;
            constexpr address_v6 make_address_v6(const address_v6::BytesType& bytes, scope_id_type scope_id = 0);
            constexpr address_v6 make_address_v6(v4_mapped_t, const address_v4& a) noexcept;
            address_v6 make_address_v6(std::string_view str);

            template<class... T>
            constexpr address_v6::BytesType::BytesType(T... t)
                : std::array<unsigned char, 16>{{static_cast<unsigned char>(t)...}} {}

            constexpr address_v6::address_v6() noexcept {}

            constexpr address_v6::address_v6(const address_v6& a) noexcept
                : address_(a.address_), scope_id_(a.scope_id_) {}

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
                return to_bytes()[0] == 0xFF;
            }

            constexpr bool address_v6::is_link_local() const noexcept
            {
                const auto b = to_bytes();
                return b[0] == 0xFE && (b[1] & 0xC0) == 0x80;
            }

            constexpr bool address_v6::is_site_local() const noexcept
            {
                const auto b = to_bytes();
                return b[0] == 0xFE && (b[1] & 0xC0) == 0xC0;
            }

            constexpr bool address_v6::is_v4_mapped() const noexcept
            {
                const auto b = to_bytes();
                return b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 0
                    && b[4] == 0 && b[5] == 0 && b[6] == 0 && b[7] == 0
                    && b[8] == 0 && b[9] == 0 && b[10] == 0xFF && b[11] == 0xFF;
            }

            constexpr bool address_v6::is_multicast_node_local() const noexcept
            {
                return is_multicast() && (to_bytes()[1] & 0x0F) == 0x01;
            }

            constexpr bool address_v6::is_multicast_link_local() const noexcept
            {
                return is_multicast() && (to_bytes()[1] & 0x0F) == 0x02;
            }

            constexpr bool address_v6::is_multicast_site_local() const noexcept
            {
                return is_multicast() && (to_bytes()[1] & 0x0F) == 0x05;
            }

            constexpr bool address_v6::is_multicast_org_local() const noexcept
            {
                return is_multicast() && (to_bytes()[1] & 0x0F) == 0x08;
            }

            constexpr bool address_v6::is_multicast_global() const noexcept
            {
                return is_multicast() && (to_bytes()[1] & 0x0F) == 0x0E;
            }

            constexpr address_v6::BytesType address_v6::to_bytes() const noexcept
            {
                return address_;
            }

            constexpr address_v6 address_v6::any() noexcept
            {
                return address_v6();
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
                scope_id_type scope_id {0};
                auto ret = inet_pton(Family::inet6, str, bytes.data(), &scope_id);
                if(ret == -1)
                {
                    throw std::invalid_argument(str.data());
                }
                return address_v6(bytes, scope_id);
            }

            template<class CharT, class Traits>
            std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const address_v6& addr)
            {
                return os << addr.to_string().c_str();
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
                        : version_{address_v4::any()}, is_v4_(true) {}
                    constexpr address(const address& a) noexcept
                        : version_{a.version_}, is_v4_(a.is_v4_) {}
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
                            return version_.v4_.to_string();
                        }

                        return version_.v6_.to_string();
                    }

                private:
                    union U
                    {
                        address_v4 v4_;
                        address_v6 v6_;

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

            // template<>
            // struct std::hash<address>;

            // template<>
            // struct std::hash<address_v4>;

            // template<>
            // struct std::hash<address_v6>;

            template<class InternetProtocol>
            class basic_endpoint
            {
                private:
                    using address_t = net::ip::address;
                public:
                    using protocol_type = InternetProtocol;

                    constexpr basic_endpoint() noexcept
                    {
                        fill_c_types();
                    }

                    constexpr basic_endpoint(const protocol_type& proto, port_type port_num) noexcept
                        : address_(proto == protocol_type::v6() ? address_v6() : address_v4()), port_(port_num)
                    {
                        fill_c_types();
                    }

                    constexpr basic_endpoint(const address& addr, port_type port_num) noexcept
                        : address_(addr), port_(port_num)
                    {
                        fill_c_types();
                    }
                    
                    constexpr protocol_type protocol() const noexcept
                    {
                        return address_.is_v6() ? protocol_type::v6() : protocol_type::v4();
                    }

                    constexpr address address() const noexcept
                    {
                        return address_;
                    }

                    void address(const net::ip::address addr) noexcept
                    {
                        address_ = addr;
                        fill_c_types();
                    }

                    constexpr port_type port() const noexcept
                    {
                        return port_;
                    }

                    void port(port_type port_num) noexcept
                    {
                        port_ = port_num;
                        fill_c_types();
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
                    } data_ {0};
                    
                    address_t address_ {};
                    port_type port_ {0};

                    constexpr void fill_c_types() noexcept
                    {
                        std::memset(&data_, 0, sizeof(data_));

                        if(protocol() == protocol_type::v6())
                        {
                            const auto v6 = address().to_v6();
                            data_.v6_.sin6_family = static_cast<short>(Family::inet6);
                            data_.v6_.sin6_port = hton(static_cast<unsigned short>(port()));
                            std::memcpy(&data_.v6_.sin6_addr, v6.to_bytes().data(), 16);
                            data_.v6_.sin6_scope_id = v6.scope_id();
                        }
                        else
                        {
                            const auto v4 = address().to_v4();
                            data_.v4_.sin_family = static_cast<short>(Family::inet);
                            data_.v4_.sin_port = hton(static_cast<unsigned short>(port()));
                            data_.v4_.sin_addr.s_addr = v4.to_uint();
                        }
                    }
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

                    basic_resolver_entry()
                        : basic_resolver_entry(endpoint_type(), "", "") {}

                    basic_resolver_entry(const endpoint_type& ep, std::string_view h, std::string_view s)
                        : endpoint_(ep), host_name_(h), service_name_(s) {}

                    endpoint_type endpoint() const
                    {
                        return endpoint_;
                    }

                    operator endpoint_type() const
                    {
                        return endpoint_();
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    host_name(const Allocator& a = Allocator()) const
                    {
                        return host_name_;
                    }

                    template<class Allocator = std::allocator<char>>
                    std::basic_string<char, std::char_traits<char>, Allocator>
                    service_name(const Allocator& a = Allocator()) const
                    {
                        return service_name_;
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
                private:
                    using container_type = std::vector<value_type>;
                public:
                    using const_reference = const value_type&;
                    using reference = value_type&;
                    using const_iterator = typename container_type::const_iterator;
                    using iterator = const_iterator;
                    using difference_type = std::ptrdiff_t;
                    using size_type = std::size_t;

                    basic_resolver_results() {}
                    basic_resolver_results(const basic_resolver_results& rhs)
                        : results(rhs.results) {}
                    basic_resolver_results(basic_resolver_results&& rhs) noexcept
                        : results(std::move(rhs.results)) {}
                    basic_resolver_results& operator=(const basic_resolver_results& rhs)
                    {
                        if(this != &rhs)
                        {
                            results = rhs.results;
                        }
                        
                        return *this;
                    }

                    basic_resolver_results& operator=(basic_resolver_results&& rhs)
                    {
                        if(this != &rhs)
                        {
                            results = std::move(rhs.results);
                        }

                        return *this;
                    }

                    ~basic_resolver_results() {}

                    size_type size() const noexcept
                    {
                        return std::size(results);
                    }

                    size_type max_size() const noexcept
                    {
                        return std::numeric_limits<size_type>::max();
                    }

                    bool empty() const noexcept
                    {
                        return results.empty();
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
                    container_type results;
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
                    using flags = decltype(AI_PASSIVE);
                    static inline constexpr flags passive {AI_PASSIVE};
                    static inline constexpr flags canonical_name {AI_CANONNAME};
                    static inline constexpr flags numeric_host {AI_NUMERICHOST};
                    static inline constexpr flags numeric_service {AI_NUMERICSERV};
                    static inline constexpr flags v4_mapped {AI_V4MAPPED};
                    static inline constexpr flags all_matching {AI_ALL};
                    static inline constexpr flags address_configured {AI_ADDRCONFIG};

                protected:
                    resolver_base() {}
                    ~resolver_base() {}
            };

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
                        const auto hints = make_hints(f, Family::unspec, 
                            endpoint_type().protocol().type(), endpoint_type().protocol().protocol());
                        
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

                    results_type resolve(const endpoint_type& e)
                    {
                        std::array<char, host_name_length> host_name {};
                        std::array<char, service_name_length> service_name {};
                        int flags {0};
                        if(endpoint_type().protocol().type() == Type::dgram)
                        {
                            flags |= static_cast<int>(name_info_flags::dgram);
                        }

                        auto result = ::getnameinfo(static_cast<const sockaddr*>(e.data()), e.size(),
                                                    host_name.data(), std::size(host_name),
                                                    service_name.data(), std::size(service_name),
                                                    flags);

                        if(result != 0)
                        {
                            flags |= static_cast<int>(name_info_flags::numeric_service);
                            result = ::getnameinfo(static_cast<const sockaddr*>(e.data()), e.size(),
                                                    host_name.data(), std::size(host_name),
                                                    service_name.data(), std::size(service_name),
                                                    flags);
                        }

                        return 
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

                    addrinfo make_hints(flags f, Family family, Type type, Protocol protocol)
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
                        results_type results;
                        addrinfo* address_results;
                        if(::getaddrinfo(host_name.data(), service_name.data(), &hints, &address_results) != 0)
                        {
                            return results;
                        }

                        while(address_results != nullptr)
                        {
                            port_type port;
                            address overall_address;
                            
                            if(static_cast<Family>(address_results->ai_family) == Family::inet6)
                            {
                                const auto sockaddr = reinterpret_cast<sockaddr_in6*>(address_results->ai_addr);
                                address_v6::BytesType bytes(sockaddr->sin6_addr.u.Byte[0],
                                                            sockaddr->sin6_addr.u.Byte[1],
                                                            sockaddr->sin6_addr.u.Byte[2], 
                                                            sockaddr->sin6_addr.u.Byte[3], 
                                                            sockaddr->sin6_addr.u.Byte[4], 
                                                            sockaddr->sin6_addr.u.Byte[5], 
                                                            sockaddr->sin6_addr.u.Byte[6], 
                                                            sockaddr->sin6_addr.u.Byte[7], 
                                                            sockaddr->sin6_addr.u.Byte[8], 
                                                            sockaddr->sin6_addr.u.Byte[9], 
                                                            sockaddr->sin6_addr.u.Byte[10], 
                                                            sockaddr->sin6_addr.u.Byte[11], 
                                                            sockaddr->sin6_addr.u.Byte[12], 
                                                            sockaddr->sin6_addr.u.Byte[13], 
                                                            sockaddr->sin6_addr.u.Byte[14], 
                                                            sockaddr->sin6_addr.u.Byte[15]);
                                overall_address = make_address_v6(bytes, sockaddr->sin6_scope_id);
                                port = ntoh(sockaddr->sin6_port);
                            }
                            else
                            {
                                const auto sockaddr = reinterpret_cast<sockaddr_in*>(address_results->ai_addr);
                                address_v4::BytesType bytes(sockaddr->sin_addr.S_un.S_un_b.s_b1,
                                                            sockaddr->sin_addr.S_un.S_un_b.s_b2,
                                                            sockaddr->sin_addr.S_un.S_un_b.s_b3,
                                                            sockaddr->sin_addr.S_un.S_un_b.s_b4);
                                overall_address = make_address_v4(bytes);
                                port = ntoh(sockaddr->sin_port);
                            }

                            results.results.emplace_back(endpoint_type(overall_address, port), host_name, service_name);
                            address_results = address_results->ai_next;
                        }

                        ::freeaddrinfo(address_results);
                        return results;
                    }
            };

            std::string host_name()
            {
                std::string name;
                name.resize(64);
                ::gethostname(name.data(), std::size(name));
                return name;
            }

            template<class Allocator>
            std::basic_string<char, std::char_traits<char>, Allocator>
            host_name(const Allocator& a)
            {
                std::basic_string<char, std::char_traits<char>, Allocator> name {a};
                name.resize(64);
                ::gethostname(name.data(), std::size(name));
                return name;
            }


            class tcp
            {
                public:
                    using endpoint = basic_endpoint<tcp>;
                    using resolver = basic_resolver<tcp>;
                    using socket = basic_stream_socket<tcp>;

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
                        return type_;
                    }

                    constexpr Protocol protocol() const noexcept
                    {
                        return protocol_;
                    }

                    tcp() = delete;
                private:
                    constexpr tcp(Family f)
                        : family_(f), type_(Type::stream), protocol_(Protocol::tcp) {}

                    Family family_;
                    Type type_;
                    Protocol protocol_;
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
                        return type_;
                    }

                    constexpr Protocol protocol() const noexcept
                    {
                        return protocol_;
                    }

                    udp() = delete;
                private:
                    constexpr udp(Family f)
                        : family_(f), type_(Type::dgram), protocol_(Protocol::udp) {}

                    Family family_;
                    Type type_;
                    Protocol protocol_;
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