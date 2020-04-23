#pragma once

#ifdef _WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
	#include <errno.h>
    using SOCKET = int;
    #define SOCKET_ERROR -1
#endif

#include <iostream>
#include <string_view>
#include <optional>
#include <atomic>
#include <mutex>
#include <memory>
#include <string>
#include <cstdint>
#include <array>
#include <limits>
#include <exception>
#include <charconv>
#include <type_traits>
#include <vector>
#include <cstdio>
#include <chrono>
#include <future>
#include <list>
#include <condition_variable>
#include <any>
#include "iocontext.hpp"
#include "../span.hpp"
#include "../utility.hpp"

#ifdef max
#undef max
#endif

namespace collin
{
	namespace net
	{
		constexpr std::size_t max_addr_v4_size = 15;
		constexpr std::size_t max_addr_v6_size = 45;
		using scope_id_type = std::uint_least32_t;

		enum class Endian
		{
		#ifdef _WIN32
			little = 0,
			big    = 1,
			native = little
		#else
			little = __ORDER_LITTLE_ENDIAN__,
			big    = __ORDER_BIG_ENDIAN__,
			native = __BYTE_ORDER__
		#endif
		};

		constexpr uint16_t swap_bytes(uint16_t data) noexcept
		{
			return static_cast<uint16_t>(
				((data >> 8) & 0xFF) | 
				((data & 0xFF) << 8)
			);
		}

		constexpr uint32_t swap_bytes(uint32_t data) noexcept
		{
			return static_cast<uint32_t>(
				((data & 0xFF000000) >> 24) |
				((data & 0x00FF0000) >> 8) | 
				((data & 0x0000FF00) << 8) |
				((data & 0x000000FF) << 24)
			);
		}

		constexpr uint64_t swap_bytes(uint64_t data) noexcept
		{
			return static_cast<uint64_t>(
				(static_cast<uint64_t>(swap_bytes(static_cast<uint32_t>(data & 0xFFFFFFFF))) << 32) |
				 swap_bytes(static_cast<uint32_t>(data >> 32))
			);
		}

		constexpr bool is_network_endianness() noexcept
		{
			return Endian::native == Endian::big;
		}

		constexpr uint32_t htonl(uint32_t host) noexcept
		{
			if constexpr(is_network_endianness())
			{
				return host;
			}
			else
			{
				return swap_bytes(host);
			}
		}

		constexpr uint64_t htonll(uint64_t host) noexcept
		{
			if constexpr(is_network_endianness())
			{
				return host;
			}
			else
			{
				return swap_bytes(host);
			}
		}

		constexpr uint16_t htons(uint16_t host) noexcept
		{
			if constexpr(is_network_endianness())
			{
				return host;
			}
			else
			{
				return swap_bytes(host);
			}
		}

		template<class T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
		constexpr T hton(T host) noexcept
		{
			if constexpr(sizeof(T) <= sizeof(uint16_t))
			{
				return htons(static_cast<uint16_t>(host));
			}
			else if constexpr(sizeof(T) <= sizeof(uint32_t))
			{
				return htonl(static_cast<uint32_t>(host));
			}
			else if constexpr(sizeof(T) <= sizeof(uint64_t))
			{
				return htonll(static_cast<uint64_t>(host));
			}
		}

		constexpr uint64_t ntohll(uint64_t network) noexcept
		{
			if constexpr(is_network_endianness())
			{
				return network;
			}
			else
			{
				return swap_bytes(network);
			}
		}

		constexpr uint32_t ntohl(uint32_t network) noexcept
		{
			if constexpr(is_network_endianness())
			{
				return network;
			}
			else
			{
				return swap_bytes(network);
			}
		}
		
		constexpr uint16_t ntohs(uint16_t network) noexcept
		{
			if constexpr(is_network_endianness())
			{
				return network;
			}
			else
			{
				return swap_bytes(network);
			}
		}

		template<class T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
		constexpr T ntoh(T network) noexcept
		{
			if constexpr(sizeof(T) <= sizeof(uint16_t))
			{
				return ntohs(static_cast<uint16_t>(network));
			}
			else if constexpr(sizeof(T) <= sizeof(uint32_t))
			{
				return ntohl(static_cast<uint32_t>(network));
			}
			else if constexpr(sizeof(T) <= sizeof(uint64_t))
			{
				return ntohll(static_cast<uint64_t>(network));
			}
		}

		int get_last_error()
		{
			#ifdef _WIN32
			return WSAGetLastError();
			#else
			#endif
		}

		enum class Family
		{
			#ifdef AF_UNSPEC
			unspec = AF_UNSPEC,
			#endif
			#ifdef AF_INET
			inet = AF_INET,
			#endif
			#ifdef AF_IPX
			ipx = AF_IPX,
			#endif
			#ifdef AF_APPLETALK
			appletalk = AF_APPLETALK,
			#endif
			#ifdef AF_NETBIOS
			netbios = AF_NETBIOS,
			#endif
			#ifdef AF_INET6
			inet6 = AF_INET6,
			#endif
			#ifdef AF_IRDA
			irda = AF_IRDA,
			#endif
			#ifdef AF_BTH
			bth = AF_BTH,
			#endif
			#ifdef AF_UNIX
			unix = AF_UNIX,
			#endif
			#ifdef AF_LOCAL
			local = AF_LOCAL,
			#endif
			#ifdef AF_AX25
			ax25 = AF_AX25,
			#endif
			#ifdef AF_X25
			x25 = AF_X25,
			#endif
			#ifdef AF_DECnet
			decnet = AF_DECnet,
			#endif
			#ifdef AF_KEY
			key = AF_KEY
			#endif
			#ifdef AF_NETLINK
			netlink = AF_NETLINK,
			#endif
			#ifdef AF_PACKET
			packet = AF_PACKET,
			#endif
			#ifdef AF_RDS
			rds = AF_RDS,
			#endif
			#ifdef AF_PPPOX
			pppox = AF_PPPOX,
			#endif
			#ifdef AF_LLC
			llc = AF_LLC,
			#endif
			#ifdef AF_IB
			ib = AF_IB,
			#endif
			#ifdef AF_MPLS
			mpls = AF_MPLS,
			#endif
			#ifdef AF_CAN
			can = AF_CAN,
			#endif
			#ifdef AF_TIPC
			tipc = AF_TIPC,
			#endif
			#ifdef AF_BLUETOOTH
			bluetooth = AF_BLUETOOTH,
			#endif
			#ifdef AF_ALG
			alg = AF_ALG,
			#endif
			#ifdef AF_VSOCK
			vsock = AF_VSOCK,
			#endif
			#ifdef AF_KCM
			kcm = AF_KCM,
			#endif
			#ifdef AF_XDP
			xdp = AF_XDP,
			#endif
		};
		
		enum class Type
		{
			#ifdef SOCK_STREAM
			stream = SOCK_STREAM,
			#endif
			#ifdef SOCK_DGRAM
			dgram = SOCK_DGRAM,
			#endif
			#ifdef SOCK_RAW
			raw = SOCK_RAW,
			#endif
			#ifdef SOCK_RDM
			rdm = SOCK_RDM,
			#endif
			#ifdef SOCK_SEQPACKET
			seqpacket = SOCK_SEQPACKET,
			#endif
			#ifdef SOCK_PACKET
			packet = SOCK_PACKET,
			#endif
		};

		enum class Protocol
		{
			automatic = 0,
			icmp = IPPROTO_ICMP,
			igmp = IPPROTO_IGMP,
			tcp = IPPROTO_TCP,
			udp = IPPROTO_UDP,
			icmpv6 = IPPROTO_ICMPV6,
		};

		std::string inet_ntop(Family af, const void* src, scope_id_type scope_id = 0)
		{
			auto bytes = static_cast<const unsigned char*>(src);
			std::string out;
			if(af == Family::inet)
			{
				out.resize(max_addr_v4_size + 1);
				std::snprintf(out.data(), std::size(out), "%u.%u.%u.%u",
					bytes[3], bytes[2], bytes[1], bytes[0]);
			}
			else if(af == Family::inet6)
			{
				out.resize(max_addr_v6_size + 1);
				std::size_t n = 0, b = 0, z = 0;

				while(n < std::size(out) && b < 16)
				{
					if(bytes[b] == 0 && bytes[b + 1] == 0 && z == 0)
					{
						do
						{
							b += 2;
						} while(b < 16 && bytes[b] == 0 && bytes[b + 1] == 0);
						n += std::snprintf(out.data() + n, std::size(out) - n, ":%s", b < 16 ? "" : ":");
						z++;
					}
					else
					{
						n += std::snprintf(out.data() + n, std::size(out) - n, "%s%x", b ? ":" : "", (static_cast<unsigned long>(bytes[b]) << 8) | bytes[b + 1]);
						b += 2;
					}
				}

				if(scope_id)
				{
					n += std::snprintf(out.data() + n, std::size(out) - n, "%%%lu", scope_id);
				}
			}

			return out;
		}

		int inet_pton(Family af, std::string_view src, void* dest, scope_id_type* scope_id)
		{
			auto bytes = static_cast<unsigned char*>(dest);
			if(af == Family::inet)
			{
				unsigned int b0, b1, b2, b3;
				if(std::sscanf(src.data(), "%u.%u.%u.%u", &b0, &b1, &b2, &b3) != 4)
				{
					return -1;
				}
				if(b0 > 0xFF || b1 > 0xFF || b2 > 0xFF || b3 > 0xFF)
				{
					return -1;
				}

				bytes[0] = static_cast<unsigned char>(b0);
				bytes[1] = static_cast<unsigned char>(b1);
				bytes[2] = static_cast<unsigned char>(b2);
				bytes[3] = static_cast<unsigned char>(b3);
				return 1;
			}
			else if(af == Family::inet6)
			{
				std::memset(bytes, 0, 16);
				std::array<unsigned char, 16> back_bytes = {0};
				auto num_front_bytes = 0, num_back_bytes = 0;

				auto p = src.data();
				
				enum {fword, fcolon, bword, scope, done} state = fword;

				unsigned long current_word = 0;
				while(state != done)
				{
					if(current_word > 0xFFFF)
					{
						return -1;
					}

					switch(state)
					{
						case fword:
							if(*p >= '0' && *p <= '9')
							{
								current_word = current_word * 16 + *p - '0';
								p++;
							}
							else if(*p >= 'a' && *p <= 'f')
							{
								current_word = current_word * 16 + *p - 'a' + 10;
								p++;
							}
							else if(*p >= 'A' && *p <= 'F')
							{
								current_word = current_word * 16 + *p - 'A' + 10;
								p++;
							}
							else
							{
								if(num_front_bytes == 16)
								{
									return -1;
								}

								bytes[num_front_bytes++] = (current_word >> 8) & 0xFF;
								bytes[num_front_bytes++] = current_word & 0xFF;
								current_word = 0;

								if(*p == ':')
								{
									state = fcolon;
									p++;
								}
								else if(*p == '%')
								{
									state = scope;
									p++;
								}
								else if(*p == 0)
								{
									state = done;
								}
								else
								{
									return -1;
								}
							}
							break;

						case fcolon:
							if(*p == ':')
							{
								state = bword;
								p++;
							}
							else
							{
								state = fword;
							}
							break;

						case bword:
							if(*p >= '0' && *p <= '9')
							{
								current_word = current_word * 16 + *p - '0';
								p++;
							}
							else if(*p >= 'a' && *p <= 'f')
							{
								current_word = current_word * 16 + *p - 'a' + 10;
								p++;
							}
							else if(*p >= 'A' && *p <= 'F')
							{
								current_word = current_word * 16 + *p - 'A' + 10;
								p++;
							}
							else
							{
								if(num_front_bytes + num_back_bytes == 16)
								{
									return -1;
								}

								back_bytes[num_back_bytes++] = (current_word >> 8) & 0xFF;
								back_bytes[num_back_bytes++] = current_word & 0xFF;
								current_word = 0;

								if(*p == ':')
								{
									state = bword;
									p++;
								}
								else if(*p == '%')
								{
									state = scope;
									p++;
								}
								else if(*p == 0)
								{
									state = done;
								}
								else
								{
									return -1;
								}
							}
							break;

						case scope:
							if(*p >= '0' && *p <= '9')
							{
								current_word = current_word * 10 + *p - '0';
								p++;
							}
							else if(*p == 0)
							{
								*scope_id = current_word;
								state = done;
							}
							else
							{
								return -1;
							}
							break;
					}
				}

				for(auto i = 0; i < num_back_bytes; i++)
				{
					bytes[16 - num_back_bytes + i] = back_bytes[i];
				}

				return 1;
			}

			return -1;
		}

		enum class socket_errc
		{
			already_open = 1,
			not_found = 2,
			send_error = 3, 
			receive_error = 4,
			eof = 5,
			try_again = 
			#ifdef _WIN32
			WSAEWOULDBLOCK
			#else
			EWOULDBLOCK
			#endif
		};

		inline const std::error_category& socket_category() noexcept
		{
			struct socket_category_impl : std::error_category
			{
				virtual const char* name() const noexcept override
				{
					return "socket";
				}

				virtual std::string message(int val) const override
				{
					switch(static_cast<socket_errc>(val))
					{
						case socket_errc::already_open:
							return "socket already open";
						case socket_errc::not_found:
							return "socket not found";
						case socket_errc::send_error:
							return "socket send error";
						case socket_errc::receive_error:
							return "socket receive error";
						case socket_errc::eof:
							return "socket at eof";
						case socket_errc::try_again:
							return "socket try again";
					}

					return "socket unknown error";
				}
			};

			static socket_category_impl sc;

			return sc;
		}

		std::error_code make_error_code(socket_errc e) noexcept
		{
			return std::error_code(static_cast<int>(e), socket_category());
		}

		std::error_condition make_error_condition(socket_errc e) noexcept
		{
			return std::error_condition(static_cast<int>(e), socket_category());
		}

		class socket_base
		{
			public:
				class broadcast
				{
					public:
						broadcast() noexcept {}
						explicit broadcast(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						broadcast& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_BROADCAST;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class debug
				{
					public:
						debug() noexcept {}
						explicit debug(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						debug& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_DEBUG;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class do_not_route
				{
					public:
						do_not_route() noexcept {}
						explicit do_not_route(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						do_not_route& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_DONTROUTE;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class keep_alive
				{
					public:
						keep_alive() noexcept {}
						explicit keep_alive(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						keep_alive& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_KEEPALIVE;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class linger
				{
					public:
						linger() noexcept
							: value_(::linger{0, 0}) {}
						explicit linger(bool v, std::chrono::seconds t) noexcept
							: value_({v, static_cast<unsigned short>(t.count())}) {}

						linger& operator=(bool v) noexcept
						{
							value_.l_onoff = v;
							return *this;
						}

						bool value() const noexcept
						{
							return enabled();
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_LINGER;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

						bool enabled() const noexcept
						{
							return value_.l_onoff != 0;
						}

						void enabled(bool e) noexcept
						{
							value_.l_onoff = e;
						}

						std::chrono::seconds timeout() const noexcept
						{
							return std::chrono::seconds(value_.l_linger);
						}

						void timeout(std::chrono::seconds t) noexcept
						{
							value_.l_linger = t.count();
						}

					private:
						::linger value_;
				};

				class out_of_band_inline
				{
					public:
						out_of_band_inline() noexcept {}
						explicit out_of_band_inline(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						out_of_band_inline& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_OOBINLINE;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class receive_buffer_size
				{
					public:
						receive_buffer_size() noexcept {}
						explicit receive_buffer_size(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						receive_buffer_size& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_RCVBUF;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class receive_low_watermark
				{
					public:
						receive_low_watermark() noexcept {}
						explicit receive_low_watermark(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						receive_low_watermark& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_RCVLOWAT;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class reuse_address
				{
					public:
						reuse_address() noexcept {}
						explicit reuse_address(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						reuse_address& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_REUSEADDR;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class send_buffer_size
				{
					public:
						send_buffer_size() noexcept {}
						explicit send_buffer_size(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						send_buffer_size& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_SNDBUF;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class send_low_watermark
				{
					public:
						send_low_watermark() noexcept {}
						explicit send_low_watermark(bool v) noexcept
							: value_(static_cast<int>(v)) {}

						send_low_watermark& operator=(bool v) noexcept
						{
							value_ = static_cast<int>(v);
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_SNDLOWAT;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

					private:
						int value_ {0};
				};

				class timeout
				{
					public:
						timeout() noexcept {}
						explicit timeout(std::chrono::milliseconds v) noexcept
							: value_(v.count()) {}

						timeout& operator=(std::chrono::milliseconds v) noexcept
						{
							value_ = v.count();
							return *this;
						}

						bool value() const noexcept
						{
							return value_ != 0;
						}

						explicit operator bool() const noexcept
						{
							return value();
						}

						bool operator!() const noexcept
						{
							return !value();
						}

						template<class Protocol> int level(const Protocol& p) const noexcept
						{
							return SOL_SOCKET;
						}

						template<class Protocol> int name(const Protocol& p) const noexcept
						{
							return SO_RCVTIMEO;
						}

						template<class Protocol> void* data(const Protocol& p) noexcept
						{
							return std::addressof(value_);
						}

						template<class Protocol> std::size_t size(const Protocol& p) noexcept
						{
							return sizeof(value_);
						}

						template<class Protocol> void resize(const Protocol& p, std::size_t s) {}

						std::chrono::milliseconds timeout_amount() const noexcept
						{
							return std::chrono::milliseconds(value_);
						}

					private:
						int value_ {0};
				};

				using shutdown_type = int;
				static inline constexpr shutdown_type shutdown_receive
				{
					#ifdef _WIN32
					SD_RECEIVE
					#else
					SHUT_RD
					#endif	
				};
				static inline constexpr shutdown_type shutdown_send
				{
					#ifdef _WIN32
					SD_SEND
					#else
					SHUT_WR
					#endif
				};
				static inline constexpr shutdown_type shutdown_both
				{
					#ifdef _WIN32
					SD_BOTH
					#else
					SHUT_RDWR
					#endif
				};

				#ifdef _WIN32
				using wait_type = SHORT;
				#else
				using wait_type = int;
				#endif
				static inline constexpr wait_type wait_read
				{
					#if defined _MSC_VER || defined __linux__
					POLLIN
					#else
						#if defined _WIN32
						(0x0100 | 0x0200)
						#endif
					#endif
				};
				static inline constexpr wait_type wait_write
				{
					#if defined _MSC_VER || defined __linux__
					POLLOUT
					#else
						#if defined _WIN32
						(0x0010)
						#endif
					#endif
				};
				static inline constexpr wait_type wait_error
				{
					#if defined _MSC_VER || defined __linux__
					POLLERR
					#else
						#if defined _WIN32
						0x0001
						#endif
					#endif
				};

				using message_flags = int;
				static inline constexpr message_flags message_peek {MSG_PEEK};
				static inline constexpr message_flags message_out_of_band {MSG_OOB};
				static inline constexpr message_flags message_do_not_route {MSG_DONTROUTE};

				static inline constexpr int max_listen_connections {SOMAXCONN};
				static inline constexpr auto invalid_socket {
					#ifdef _WIN32
					INVALID_SOCKET
					#else
					-1
					#endif
				};

			protected:
				socket_base() {}
				~socket_base() {}
		};

		template<class Protocol>
		class basic_socket : public socket_base
		{
			public:
				using native_handle_type = SOCKET;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_socket(io_context& ctx)
				{
				}

				basic_socket(basic_socket&& rhs) noexcept
					: socket_(rhs.release()), protocol_(std::move(rhs.protocol_))
				{
					#ifdef _WIN32
					is_blocking = rhs.is_blocking;
					#endif
				}

				basic_socket& operator=(basic_socket&& rhs) noexcept
				{
					if (this != &rhs)
					{
						socket_ = rhs.release();
						protocol_ = std::move(rhs.protocol_);

						#ifdef _WIN32
						is_blocking = rhs.is_blocking;
						#endif
					}

					return *this;
				}

				~basic_socket()
				{
					close();
				}

				native_handle_type native_handle()
				{
					return socket_;
				}

				void open(const protocol_type& protocol)
				{
					if(is_open())
					{
						throw make_error_condition(socket_errc::already_open);
					}
					
					protocol_ = protocol;
					socket_ = ::socket(static_cast<int>(protocol_.family()), 
									   static_cast<int>(protocol_.type()),
									   static_cast<int>(protocol_.protocol()));
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_socket)
				{
					if(is_open())
					{
						throw make_error_condition(socket_errc::already_open);
					}

					protocol_ = protocol;
					socket_ = native_socket;
				}

				native_handle_type release()
				{
					std::unique_lock l(task_m);
					if(last_call.valid())
					{
						last_call.wait();
					}
					l.unlock();

					native_handle_type h = native_handle();
					socket_ = socket_base::invalid_socket;
					return h;
				}

				bool is_open() const noexcept
				{
					return socket_ != socket_base::invalid_socket;
				}

				void close()
				{
					std::unique_lock l(task_m);
					if(last_call.valid())
					{
						last_call.wait();
					}
					l.unlock();

					shutdown(shutdown_both);

					#ifdef _WIN32
					::closesocket(native_handle());
					#else
					::close(native_handle());
					#endif

					socket_ = socket_base::invalid_socket;
				}

				template<class SettableSocketOption>
				void set_option(SettableSocketOption& option)
				{
					setsockopt(native_handle(), option.level(protocol_),
						option.name(protocol_), static_cast<const char*>(option.data(protocol_)),
							option.size(protocol_));
				}

				template<class GettableSocketOption>
				void get_option(GettableSocketOption& option) const
				{
					socklen_t option_len = option.size(protocol_);

					auto result = ::getsockopt(native_handle(), option.level(protocol_),
									option.name(protocol_), option.data(protocol_),
										&option_len);

					if(result == 0)
					{
						option.resize(option_len);
					}
				}

				template<class IOControlCommand>
				void io_control(IOControlCommand& command)
				{
					#ifdef _WIN32
					ioctlsocket(native_handle(), command.name(), command.data());
					#else
					ioctl(native_handle(), command.name(), command.data());
					#endif
				}

				void non_blocking(bool mode)
				{
					native_non_blocking(mode);
				}

				bool non_blocking() const
				{
					return native_non_blocking();
				}

				void native_non_blocking(bool mode)
				{
					#ifdef _WIN32
					auto mode_ul = static_cast<unsigned long>(mode);
					ioctlsocket(native_handle(), FIONBIO, &mode_ul);
					is_blocking = mode;
					#else
					auto flags = fcntl(native_handle(), F_GETFL, 0);
					if(flags >= 0)
					{
						if(mode)
							flags |= O_NONBLOCK;
						else
							flags &= ~O_NONBLOCK;

						fnctl(native_handle(), F_SETLFL, flags);
					}
					#endif
				}

				bool native_non_blocking() const
				{
					#ifdef _WIN32
					return is_blocking;
					#else
					return fcntl(native_handle(), F_GETFL, 0) & O_NONBLOCK;
					#endif
				}

				bool at_mark() const
				{
					#ifdef _WIN32
					return ioctlsocket(native_handle(), SIOCATMARK);
					#else
					return sockatmark(native_handle());
					#endif
				}

				std::size_t available()
				{
					if(!is_open())
					{
						throw std::errc::bad_file_descriptor;
					}


					#ifdef _WIN32
					unsigned long bytes_available;
					ioctlsocket(native_handle(), FIONREAD, &bytes_available);
					#else
					std::size_t bytes_available;
					ioctl(native_handle(), FIONREAD, &bytes_available);
					#endif

					return static_cast<std::size_t>(bytes_available);
				}

				void bind(const endpoint_type& endpoint)
				{
					::bind(native_handle(), endpoint.data(), endpoint.size());
				}

				void shutdown(shutdown_type what)
				{
					::shutdown(native_handle(), what);
				}

				endpoint_type local_endpoint()
				{
					endpoint_type endpoint;
					socklen_t endpoint_len = endpoint.capacity();
					int result = getsockname(native_handle(), endpoint.data(), &endpoint_len);
					if(result == 0)
					{
						endpoint.resize(endpoint_len);
					}

					return endpoint;
				}

				endpoint_type remote_endpoint()
				{
					endpoint_type endpoint;
					socklen_t endpoint_len = endpoint.capacity();
					int result = getpeername(native_handle(), static_cast<sockaddr*>(endpoint.data()), &endpoint_len);
					if(result == 0)
					{
						endpoint.resize(endpoint_len);
					}

					return endpoint;
				}

				void connect(const endpoint_type& endpoint, std::error_code& ec)
				{
					if(!is_open())
					{
						open(endpoint.protocol());
					}

					auto result = ::connect(native_handle(), static_cast<const sockaddr*>(endpoint.data()), endpoint.size());
					if (result == socket_base::invalid_socket)
					{
						#ifdef _WIN32
						ec = std::error_code(::WSAGetLastError(), std::system_category());
						#else
						ec = std::error_code(errno, std::system_category());
						#endif
					}
				}

				std::shared_future<void> async_connect(const endpoint_type& endpoint, std::error_code& ec)
				{
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(connect, std::ref(endpoint), std::ref(ec)));
					return last_call;
				}

				int wait(wait_type w)
				{
					pollfd p {native_handle(), w, };
					#ifdef _WIN32
					return ::WSAPoll(&p, 1, std::chrono::milliseconds(200).count());
					#else
					return ::poll(&p, 1, std::chrono::milliseconds(200).count());
					#endif
				}

				std::shared_future<void> async_wait(wait_type w)
				{
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(wait, w));
					return last_call;
				}

				explicit operator bool() const noexcept
				{
					return is_open();
				}

			protected:
				std::shared_future<void> last_call;
				static inline std::mutex task_m;

			private:
				native_handle_type socket_ {static_cast<native_handle_type>(socket_base::invalid_socket)};
				protocol_type protocol_ {protocol_type::v4()};
				#ifdef _WIN32
				bool is_blocking {false};
				#endif
		};

		template<class AcceptableProtocol>
		class basic_socket_acceptor : public socket_base
		{
			public:
				using native_handle_type = SOCKET;
				using protocol_type = AcceptableProtocol;
				using endpoint_type = typename protocol_type::endpoint;
				using socket_type = typename protocol_type::socket;

				basic_socket_acceptor(io_context& ctx)
					: ctx_(ctx)
				{
					std::scoped_lock l(ref_count_m);
					ref_count++;
					if(ref_count == 1)
					{
						socket_base::init();
					}
				}

				~basic_socket_acceptor()
				{
					close();
					std::scoped_lock l(ref_count_m);
					ref_count--;
					if(ref_count == 0)
					{
						socket_base::cleanup();
					}
				}

				native_handle_type native_handle()
				{
					return socket_;
				}

				void open(const protocol_type& protocol)
				{
					if(is_open())
					{
						throw make_error_condition(socket_errc::already_open);
					}
					
					protocol_ = protocol;
					socket_ = ::socket(static_cast<int>(protocol_.family()), 
									   static_cast<int>(protocol_.type()),
									   static_cast<int>(protocol_.protocol()));
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_socket)
				{
					if(is_open())
					{
						throw make_error_condition(socket_errc::already_open);
					}

					protocol_ = protocol;
					socket_ = native_socket;
				}

				native_handle_type release()
				{
					std::unique_lock l(task_m);
					if(last_call.valid())
					{
						last_call.wait();
					}
					l.unlock();

					native_handle_type h = native_handle();
					socket_ = socket_base::invalid_socket;
					return h;
				}

				bool is_open() const noexcept
				{
					return socket_ != socket_base::invalid_socket;
				}

				void close()
				{
					std::unique_lock l(task_m);
					if(last_call.valid())
					{
						last_call.wait();
					}
					l.unlock();

					int status = 0;
					if(*this)
					{
						shutdown(shutdown_both);
						#ifdef _WIN32
						status = ::closesocket(native_handle());
						#else
						status = ::close(native_handle());
						#endif
						socket_ = socket_base::invalid_socket;
					}

					return status;
				}

				template<class SettableSocketOption>
				void set_option(const SettableSocketOption& option)
				{
					setsockopt(native_handle(), option.level(protocol_),
						option.name(protocol_), option.data(protocol_),
							option.size(protocol_));
				}

				template<class GettableSocketOption>
				void get_option(GettableSocketOption& option) const
				{
					socklen_t option_len = option.size(protocol_);

					auto result = ::getsockopt(native_handle(), option.level(protocol_),
									option.name(protocol_), option.data(protocol_),
										&option_len);

					if(result == 0)
					{
						option.resize(option_len);
					}
				}

				template<class IOControlCommand>
				void io_control(IOControlCommand& command)
				{
					#ifdef _WIN32
					ioctlsocket(native_handle(), command.name(), command.data());
					#else
					ioctl(native_handle(), command.name(), command.data());
					#endif
				}

				void non_blocking(bool mode)
				{
					native_non_blocking(mode);
				}

				bool non_blocking() const
				{
					return native_non_blocking();
				}

				void native_non_blocking(bool mode)
				{
					#ifdef _WIN32
					ioctlsocket(native_handle(), FIONBIO, &mode);
					blocking = mode;
					#else
					auto flags = fcntl(native_handle(), F_GETFL, 0);
					if(flags >= 0)
					{
						if(mode)
							flags |= O_NONBLOCK;
						else
							flags &= ~O_NONBLOCK;

						fnctl(native_handle(), F_SETLFL, flags);
					}
					#endif
				}

				bool native_non_blocking() const
				{
					#ifdef _WIN32
					return is_blocking;
					#else
					return fcntl(native_handle(), F_GETFL, 0) & O_NONBLOCK;
					#endif
				}

				bool at_mark() const
				{
					#ifdef _WIN32
					return ioctlsocket(native_handle(), SIOCATMARK);
					#else
					return sockatmark(native_handle());
					#endif
				}

				std::size_t available()
				{
					if (!is_open())
					{
						throw std::errc::bad_file_descriptor;
					}


					#ifdef _WIN32
					unsigned long bytes_available;
					ioctlsocket(native_handle(), FIONREAD, &bytes_available);
					#else
					std::size_t bytes_available;
					ioctl(native_handle(), FIONREAD, &bytes_available);
					#endif

					return static_cast<std::size_t>(bytes_available);
				}

				void bind(const endpoint_type& endpoint)
				{
					::bind(native_handle(), endpoint.data(), endpoint.size());
				}

				void listen(int backlog = max_listen_connections)
				{
					::listen(native_handle(), backlog);
				}

				socket_type accept()
				{
					native_handle_type h = ::accept(native_handle(), nullptr, 0);
					return socket_type(protocol_, h);
				}

				socket_type accept(endpoint_type& endpoint)
				{
					socklen_t endpoint_len = endpoint.capacity();
					native_handle_type h = ::accept(native_handle(), endpoint.data(), &endpoint_len);
					return socket_type(protocol_, h);
				}

				std::shared_future<socket_type> async_accept()
				{
					auto call = std::shared_future<socket_type>(std::async(accept));
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(std::launch::deferred, [=]() {
						call.wait();
					}));

					return call;
				}

				std::shared_future<socket_type> async_accept(endpoint_type& endpoint)
				{
					auto call = std::shared_future<socket_type>(std::async(accept, std::ref(endpoint)));
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(std::launch::deferred, [=]() {
						call.wait();
					}));

					return call;
				}

				void shutdown(shutdown_type what)
				{
					::shutdown(native_handle(), what);
				}

				endpoint_type local_endpoint()
				{
					endpoint_type endpoint;
					socklen_t endpoint_len = endpoint.capacity();
					int result = getsockname(native_handle(), endpoint.data(), &endpoint_len);
					if(result == 0)
					{
						endpoint.resize(endpoint_len);
					}

					return endpoint;
				}

				endpoint_type remote_endpoint()
				{
					endpoint_type endpoint;
					socklen_t endpoint_len = endpoint.capacity();
					int result = getpeername(native_handle(), endpoint.data(), &endpoint_len);
					if(result == 0)
					{
						endpoint.resize(endpoint_len);
					}

					return endpoint;
				}

				void connect(const endpoint_type& endpoint, std::error_code& ec)
				{
					if(!is_open())
					{
						open(endpoint.protocol());
					}

					auto result = ::connect(native_handle(), endpoint.data(), endpoint.size());
					if (result == socket_base::invalid_socket)
					{
						#ifdef _WIN32
						ec = std::error_code(::WSAGetLastError(), std::system_category());
						#else
						ec = std::error_code(errno, std::system_category());
						#endif
					}
				}

				std::shared_future<void> async_connect(const endpoint_type& endpoint)
				{
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(connect, std::ref(endpoint)));
					return last_call;
				}

				void wait(wait_type w)
				{
					pollfd p {native_handle(), w, 0};
					#ifdef _WIN32
					::WSAPoll(&p, 1, std::chrono::milliseconds(200).count());
					#else
					::poll(&p, 1, std::chrono::milliseconds(200).count());
					#endif
				}

				std::shared_future<void> async_wait(wait_type w)
				{
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(wait, w));
					return last_call;
				}

				explicit operator bool() const noexcept
				{
					return is_open();
				}

			protected:
				std::shared_future<void> last_call;
				static inline std::mutex task_m;

			private:
				native_handle_type socket_ {socket_base::invalid_socket};
				protocol_type protocol_;
				io_context& ctx_;
				#ifdef _WIN32
				bool is_blocking {false};
				#endif
		};

		template<class Protocol>
		class basic_datagram_socket : public basic_socket<Protocol>
		{
			public:
				using native_handle_type = basic_socket<Protocol>::native_handle_type;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				template<class ElementType, std::size_t Extent>
				std::size_t receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					ec.clear();
					auto bytes = as_writable_bytes(buf);

					std::size_t total = 0;
					while (total != bytes.size())
					{
						const auto n = ::recv(native_handle(), reinterpret_cast<char*>(bytes.data() + total), bytes.size() - total, flags);
						switch (get_last_error())
						{
							case socket_errc::try_again:
								ec = make_error_code(socket_errc::try_again);
								break;
							case 0:
								break;
							default:
								ec = make_error_code(socket_errc::receive_error);
								close();
								break;
						}

						if (ec || n <= 0)
						{
							return total;
						}

						total += n;
					}

					return total;
				}

				template<class ElementType, std::size_t Extent>
				std::shared_future<std::size_t> async_receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					auto call = std::shared_future<std::size_t>(std::async(receive, buf, std::ref(ec), flags));
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(std::launch::deferred, [=]() {
						call.wait();
					}));
					return call;
				}

				template<class ElementType, std::size_t Extent>
				std::size_t send(const span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					ec.clear();
					auto bytes = as_bytes(buf);

					std::size_t total = 0;
					while (total != bytes.size())
					{
						const auto n = ::send(native_handle(), reinterpret_cast<const char*>(bytes.data() + total), bytes.size() - total, flags);
						switch (get_last_error())
						{
							case socket_errc::try_again:
								ec = make_error_code(socket_errc::try_again);
								break;
							case 0:
								break;
							default:
								ec = make_error_code(socket_errc::receive_error);
								close();
								break;
						}

						if (ec || n <= 0)
						{
							return total;
						}

						total += n;
					}

					return total;
				}

				template<class ElementType, std::size_t Extent>
				std::shared_future<std::size_t> async_send(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					auto call = std::shared_future<std::size_t>(std::async(send, buf, std::ref(ec), flags));
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(std::launch::deferred, [=]() {
						call.wait();
					}));
					return call;
				}
		};

		template<class Protocol>
		class basic_stream_socket : public basic_socket<Protocol>
		{
			public:
				using native_handle_type = basic_socket<Protocol>::native_handle_type;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_stream_socket(net::io_context& ctx)
					: basic_socket(ctx) {}

				template<class ElementType, std::size_t Extent>
				std::size_t receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					ec.clear();
					auto bytes = as_writable_bytes(buf);

					std::size_t total = 0;
					while (total != bytes.size())
					{
						const auto n = ::recv(native_handle(), reinterpret_cast<char*>(bytes.data() + total), bytes.size() - total, flags);
						switch (get_last_error())
						{
							case socket_errc::try_again:
								ec = make_error_code(socket_errc::try_again);
								break;
							case 0:
								break;
							default:
								ec = make_error_code(socket_errc::receive_error);
								close();
								break;
						}

						if(ec || n <= 0)
						{
							return total;
						}

						total += n;
					}

					return total;
				}

				template<class ElementType, std::size_t Extent>
				std::shared_future<std::size_t> async_receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					auto call = std::shared_future<std::size_t>(std::async(receive, buf, std::ref(ec), flags));
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(std::launch::deferred, [=]() {
						call.wait();
					}));
					return call;
				}

				template<class ElementType, std::size_t Extent>
				std::size_t send(const span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					ec.clear();
					auto bytes = as_bytes(buf);

					std::size_t total = 0;
					while (total != bytes.size())
					{
						const auto n = ::send(native_handle(), reinterpret_cast<const char*>(bytes.data() + total), bytes.size() - total, flags);
						switch (get_last_error())
						{
							case socket_errc::try_again:
								ec = make_error_code(socket_errc::try_again);
								break;
							case 0:
								break;
							default:
								ec = make_error_code(socket_errc::receive_error);
								close();
								break;
						}

						if (ec || n <= 0)
						{
							return total;
						}

						total += n;
					}

					return total;
				}

				template<class ElementType, std::size_t Extent>
				std::shared_future<std::size_t> async_send(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					auto call = std::shared_future<std::size_t>(std::async(send, buf, std::ref(ec), flags));
					std::scoped_lock l {task_m};
					last_call = std::shared_future<void>(std::async(std::launch::deferred, [=]() {
						call.wait();
					}));
					return call;
				}
		};

		template<class Protocol>
		class basic_socket_streambuf : public std::basic_streambuf<char>
		{
			public:
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_socket_streambuf(net::io_context& ctx)
					: ctx_(ctx), socket_(ctx)
				{
					set_buffers();
				}

				explicit basic_socket_streambuf(basic_stream_socket<protocol_type> s)
					: socket_(std::move(s))
				{
					set_buffers();
				}

				basic_socket_streambuf(const basic_socket_streambuf&) = delete;
				basic_socket_streambuf(basic_socket_streambuf&& rhs)
					: std::basic_streambuf<char>(std::move(rhs)), ctx_(rhs.ctx_), socket_(std::move(rhs.socket_))
				{
					set_buffers();
				}

				virtual ~basic_socket_streambuf()
				{
					if(pptr() != pbase())
					{
						overflow(std::istream::traits_type::eof());
					}
					close();
				}

				basic_socket_streambuf& operator=(const basic_socket_streambuf&) = delete;
				basic_socket_streambuf& operator=(basic_socket_streambuf&& rhs)
				{
					if (this != &rhs)
					{
						close();
						socket_ = rhs.socket_;
						rhs.socket_.assign(protocol_type::v4(), socket_base::invalid_socket);
						std::basic_streambuf<char>::operator=(std::move(rhs));
					}

					return *this;
				}

				basic_socket_streambuf* connect(const endpoint_type& e)
				{
					if (close() == nullptr)
					{
						return nullptr;
					}

					ec_.clear();
					socket_.connect(e, ec_);
					set_buffers();
					
					return ec_ ? nullptr : this;
				}
				
				template<class... Args>
				basic_socket_streambuf* connect(Args&&... args)
				{
					const auto results = protocol_type::resolver(ctx_).resolve(std::forward<Args>(args)...);
					for (const auto& result : results)
					{
						const auto end = static_cast<endpoint_type>(result.endpoint()); // Needed for overload resolution to work properly
						auto connection_result = connect(end);
						if (connection_result != nullptr)
						{
							return connection_result;
						}
					}

					return nullptr;
				}

				basic_socket_streambuf* close()
				{
					ec_.clear();
					if(socket_.is_open())
					{
						overflow(std::istream::traits_type::eof());
						socket_.close();
					}
					return ec_ ? nullptr : this;
				}

				basic_socket<protocol_type>& socket()
				{
					return socket_;
				}

				std::error_code error() const
				{
					return ec_;
				}

			protected:
				virtual std::ios::int_type underflow() override
				{
					if (gptr() != egptr())
					{
						return std::istream::traits_type::eof();
					}

					while(true)
					{
						const auto bytes_read = socket_.receive(make_span(get_buf), ec_);
						if (bytes_read > 0)
						{
							setg(get_buf.data(), get_buf.data(), get_buf.data() + bytes_read);
							return std::istream::traits_type::to_int_type(*gptr());
						}

						if (bytes_read == 0)
						{
							if(ec_ != make_error_code(socket_errc::receive_error))
							{
								ec_.clear();
							}

							return std::istream::traits_type::eof();
						}

						if (ec_ != make_error_code(socket_errc::try_again))
						{
							return std::istream::traits_type::eof();
						}

						if (socket().wait(socket_base::wait_read) < 0)
						{
							return std::istream::traits_type::eof();
						}
					}
				}

				virtual std::ios::int_type overflow(std::ios::int_type c = std::istream::traits_type::eof()) override
				{
					const auto ch = std::istream::traits_type::to_char_type(c);
					auto begin = &ch;
					auto size = sizeof(std::istream::traits_type::char_type);
					if(put_buf[0] != '\0')
					{
						begin = put_buf.data();
						size = (pptr() - pbase()) * sizeof(std::istream::traits_type::char_type);
					}

					while (size > 0)
					{
						const auto bytes_sent = socket_.send(make_span(begin, size), ec_);

						if (bytes_sent > 0)
						{
							begin += size;
							size -= bytes_sent;
							continue;
						}

						if (ec_ != make_error_code(socket_errc::try_again))
						{
							return std::istream::traits_type::eof();
						}

						if (socket().wait(socket_base::wait_write) < 0)
						{
							return std::istream::traits_type::eof();
						}
					}

					if (put_buf[0] != '\0')
					{
						setp(put_buf.data(), put_buf.data() + put_buf.size());
						if (std::istream::traits_type::eq_int_type(c, std::istream::traits_type::eof()))
						{
							return std::istream::traits_type::not_eof(c);
						}

						*pptr() = ch;
						pbump(1);
					}

					return std::istream::traits_type::not_eof(c);
				}

				virtual int sync() override
				{
					return overflow(std::istream::traits_type::eof());
				}

				std::streambuf* setbuf(std::istream::char_type* s, std::streamsize n)
				{
					if (pptr() == pbase() && s == nullptr && n == 0)
					{
						setp(0, 0);
						sync();
						return this;
					}
					
					return nullptr;
				}
			private:
				static inline constexpr std::size_t buffer_size {4096};
				net::io_context& ctx_;
				basic_stream_socket<protocol_type> socket_;
				std::array<char, buffer_size> get_buf;
				std::array<char, buffer_size> put_buf;
				std::error_code ec_;

				void set_buffers()
				{
					put_buf[0] = '\0';
					setg(get_buf.data(), get_buf.data(), get_buf.data());
					setp(put_buf.data(), put_buf.data() + put_buf.size());
				}
		};

		template<class Protocol>
		class basic_socket_iostream : public std::basic_iostream<char>
		{
			public:
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_socket_iostream(io_context& ctx)
					: std::basic_iostream<char>(&sb_), sb_(ctx)
				{
					setf(std::ios_base::unitbuf);
				}

				explicit basic_socket_iostream(basic_stream_socket<protocol_type> s)
					: std::basic_iostream<char>(&sb_), sb_(std::move(s))
				{
					setf(std::ios_base::unitbuf);
				}

				basic_socket_iostream(const basic_socket_iostream&) = delete;
				basic_socket_iostream(basic_socket_iostream&& rhs)
					: std::basic_iostream<char>(std::move(rhs)), sb_(std::move(rhs.sb_))
				{
					if (this != &rhs)
					{
						std::basic_iostream<char>::set_rdbuf(&sb_);
					}
				}

				template<class... Args>
				explicit basic_socket_iostream(io_context& ctx, Args&&... args)
					: std::basic_iostream<char>(&sb_), sb_(ctx)
				{
					setf(std::ios_base::unitbuf);
					connect(std::forward<Args>(args)...);
				}

				basic_socket_iostream& operator=(const basic_socket_iostream&) = delete;
				basic_socket_iostream& operator=(basic_socket_iostream&& rhs)
				{
					if (this != &rhs)
					{
						std::basic_iostream<char>::operator=(std::move(rhs));
						sb_ = std::move(rhs.sb_);
					}
				}

				template<class... Args>
				void connect(Args&&... args)
				{
					if (rdbuf()->connect(std::forward<Args>(args)...) == nullptr)
					{
						setstate(std::ios::failbit);
					}
				}

				void close()
				{
					if (rdbuf()->close() == nullptr)
					{
						setstate(std::ios::failbit);
					}
				}

				basic_socket_streambuf<protocol_type>* rdbuf() const
				{
					return const_cast<decltype(sb_)*>(std::addressof(sb_));
				}

				basic_socket<protocol_type>& socket()
				{
					return rdbuf()->socket();
				}

				std::error_code error() const
				{
					return rdbuf()->error();
				}
			private:
				basic_socket_streambuf<protocol_type> sb_;
		};

		template<class Protocol, class EndpointSequence, class ConnectCondition>
		typename Protocol::endpoint connect(basic_socket<Protocol>& s, const EndpointSequence& endpoints, ConnectCondition c, std::error_code& ec)
		{
			auto found = false;
			for(auto& ep_i : endpoints)
			{
				const auto ep = ep_i.endpoint();
				if(c(ep))
				{
					s.close();
					s.open(ep.protocol());
					s.connect(ep, ec);
					if (!ec)
					{
						found = true;
						return ep;
					}
					ec.clear();
				}
			}

			return typename Protocol::endpoint();
		}

		template<class Protocol, class EndpointSequence>
		typename Protocol::endpoint connect(basic_socket<Protocol>& s, const EndpointSequence& endpoints, std::error_code& ec)
		{
			return connect(s, endpoints, [](auto){ return true; }, ec);
		}

		template<class Protocol, class EndpointSequence, class ConnectCondition>
		std::future<typename Protocol::endpoint> async_connect(basic_socket<Protocol>& s, const EndpointSequence& endpoints, ConnectCondition c)
		{
			return std::async(connect, std::ref(s), std::ref(endpoints), c);
		}

		template<class Protocol, class EndpointSequence>
		typename Protocol::endpoint async_connect(basic_socket<Protocol>& s, const EndpointSequence& endpoints)
		{
			return async_connect(s, endpoints, [](auto){ return true; });
		}
	}
}
