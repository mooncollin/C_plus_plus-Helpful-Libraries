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
#include <bit>

#include "collin/net/iocontext.hpp"
#include "collin/net/buffers.hpp"
#include "collin/span.hpp"
#include "collin/utility.hpp"
#include "collin/concepts.hpp"

#ifdef max
#undef max
#endif

namespace collin
{
	namespace net
	{
		using socket_type = 
		#ifdef _WIN32
		SOCKET
		#else
		int
		#endif
		;

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
			return std::endian::native == std::endian::big;
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

		template<collin::concepts::unsigned_integral T>
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

		template<collin::concepts::unsigned_integral T>
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

		inline int get_last_error()
		{
			#ifdef _WIN32
			return ::WSAGetLastError();
			#else
			return errno;
			#endif
		}

		inline void clear_last_error()
		{
			#ifdef _WIN32
			::WSASetLastError(0);
			#else
			errno = 0;
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

						template<class Protocol> const void* data(const Protocol& p) const noexcept
						{
							return reinterpret_cast<const void*>(std::addressof(value_));
						}

						template<class Protocol> std::size_t size(const Protocol& p) const noexcept
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

				enum class shutdown_type : int
				{
					shutdown_receive = 
					#ifdef _WIN32
					SD_RECEIVE
					#else
					SHUT_RD
					#endif
					,
					shutdown_send = 
					#ifdef _WIN32
					SD_SEND
					#else
					SHUT_WR
					#endif
					,
					shutdown_both =
					#ifdef _WIN32
					SD_BOTH
					#else
					SHUT_RDWR
					#endif
				};

				static inline constexpr shutdown_type shutdown_receive = shutdown_type::shutdown_receive;
				static inline constexpr shutdown_type shutdown_send = shutdown_type::shutdown_send;
				static inline constexpr shutdown_type shutdown_both = shutdown_type::shutdown_both;

				enum class wait_type :
										#ifdef _WIN32
										SHORT
										#else
										int
										#endif
				{
					wait_read = 
					#if defined _MSC_VER || defined __linux__
					POLLIN
					#else
						#if defined _WIN32
						(0x0100 | 0x0200)
						#endif
					#endif
					,
					wait_write = 
					#if defined _MSC_VER || defined __linux__
					POLLOUT
					#else
						#if defined _WIN32
						(0x0010)
						#endif
					#endif
					,
					wait_error = 
					#if defined _MSC_VER || defined __linux__
					POLLERR
					#else
						#if defined _WIN32
						0x0001
						#endif
					#endif
				};

				static inline constexpr wait_type wait_read = wait_type::wait_read;
				static inline constexpr wait_type wait_write = wait_type::wait_write;
				static inline constexpr wait_type wait_error = wait_type::wait_error;

				enum class message_flags : int
				{
					message_peek = MSG_PEEK,
					message_out_of_band = MSG_OOB,
					message_do_not_route = MSG_DONTROUTE
				};

				static inline constexpr message_flags message_peek = message_flags::message_peek;
				static inline constexpr message_flags message_out_of_band = message_flags::message_out_of_band;
				static inline constexpr message_flags message_do_not_route = message_flags::message_peek;

				static inline constexpr int max_listen_connections {SOMAXCONN};
				static inline constexpr socket_type invalid_socket {
					#ifdef _WIN32
					INVALID_SOCKET
					#else
					-1
					#endif
				};

			protected:
				socket_base() = default;
				~socket_base() = default;
		};

		constexpr socket_base::message_flags operator&(socket_base::message_flags f1, socket_base::message_flags f2)
		{
			return socket_base::message_flags(static_cast<std::underlying_type_t<socket_base::message_flags>>(f1) & static_cast<std::underlying_type_t<socket_base::message_flags>>(f2));
		}

		constexpr socket_base::message_flags operator|(socket_base::message_flags f1, socket_base::message_flags f2)
		{
			return socket_base::message_flags(static_cast<std::underlying_type_t<socket_base::message_flags>>(f1) | static_cast<std::underlying_type_t<socket_base::message_flags>>(f2));
		}

		constexpr socket_base::message_flags operator^(socket_base::message_flags f1, socket_base::message_flags f2)
		{
			return socket_base::message_flags(static_cast<std::underlying_type_t<socket_base::message_flags>>(f1) ^ static_cast<std::underlying_type_t<socket_base::message_flags>>(f2));
		}

		constexpr socket_base::message_flags operator~(socket_base::message_flags f)
		{
			return socket_base::message_flags(~static_cast<std::underlying_type_t<socket_base::message_flags>>(f));
		}

		inline socket_base::message_flags operator&=(socket_base::message_flags& f1, socket_base::message_flags f2)
		{
			return f1 = (f1 & f2);
		}

		inline socket_base::message_flags operator|=(socket_base::message_flags& f1, socket_base::message_flags f2)
		{
			return f1 = (f1 | f2);
		}

		inline socket_base::message_flags operator^=(socket_base::message_flags& f1, socket_base::message_flags f2)
		{
			return f1 = (f1 ^ f2);
		}

		template<std::size_t Ext>
		auto socket_receive(socket_type s, span<msgtype, Ext> bufs, int flags, std::error_code& ec)
		{
			clear_last_error();
			ec.clear();
			#ifdef _WIN32
			DWORD bytes_transferred = 0;
			DWORD recv_flags = flags;
			const auto result = ::WSARecv(s, bufs.data(), bufs.size(), &bytes_transferred, &recv_flags, 0, 0);
			if(result != 0)
			{
				ec.assign(get_last_error(), std::generic_category());
			}
			return bytes_transferred;
			#else
			struct msghdr msg {};
			msg.msg_iov = bufs.data();
			msg.msg_iovlen = static_cast<int>(buf.size());
			const auto result = ::recvmsg(s, &msg, flags);
			if(result != 0)
			{
				ec.assign(get_last_error(), std::generic_category());
			}
			return result;
			#endif
		}

		template<class ElementType, std::size_t Extent>
		std::size_t socket_receive(socket_type s, span<ElementType, Extent> buf, int flags, std::error_code& ec)
		{
			clear_last_error();
			ec.clear();
			auto bytes = as_writable_bytes(buf);
			std::size_t total = 0;
			while(total != bytes.size())
			{
				const auto result = ::recv(s, reinterpret_cast<char*>(bytes.data() + total), bytes.size() - total, flags);
				switch(get_last_error())
				{
					case socket_errc::try_again:
						ec = make_error_code(socket_errc::try_again);
						break;
					case 0:
						break;
					default:
						ec.assign(get_last_error(), std::generic_category());
						break;
				}

				if(ec || result <= 0)
				{
					return total;
				}

				total += result;
			}

			return total;
		}

		template<std::size_t Ext>
		auto socket_send(socket_type s, span<msgtype, Ext> bufs, int flags, std::error_code& ec)
		{
			clear_last_error();
			ec.clear();
			#ifdef _WIN32
			DWORD bytes_transferred = 0;
			DWORD send_flags = flags;
			const auto result = ::WSASend(s, bufs.data(), bufs.size(), &bytes_transferred, send_flags, 0, 0);
			if (result != 0)
			{
				ec.assign(get_last_error(), std::generic_category());
			}
			return bytes_transferred;
			#else
			struct msghdr msg {};
			msg.msg_iov = bufs.data();
			msg.msg_iovlen = static_cast<int>(buf.size());
			const auto result = ::sendmsg(s, &msg, flags);
			if (result != 0)
			{
				ec.assign(get_last_error(), std::generic_category());
			}
			return result;
			#endif
		}

		template<class ElementType, std::size_t Extent>
		std::size_t socket_send(socket_type s, const span<ElementType, Extent> buf, int flags, std::error_code& ec)
		{
			clear_last_error();
			ec.clear();
			const auto bytes = as_bytes(buf);
			
			std::size_t total = 0;
			while(total != bytes.size())
			{
				const auto result = ::send(s, reinterpret_cast<const char*>(bytes.data() + total), bytes.size() - total, flags);
				switch(get_last_error())
				{
					case socket_errc::try_again:
						ec = make_error_code(socket_errc::try_again);
					case 0:
						break;
					default:
						ec.assign(get_last_error(), std::generic_category());
						break;
				}

				if(ec || result <= 0)
				{
					return total;
				}

				total += result;
			}

			return total;
		}

		template<class Protocol>
		class socket_impl
		{
			protected:
				using native_handle_type = socket_type;

				explicit socket_impl(io_context& ctx)
					: ctx(std::addressof(ctx)) {}

				socket_impl(socket_impl&& rhs) noexcept
					: ctx(rhs.ctx), sockfd(std::exchange(rhs.sockfd, socket_base::invalid_socket)), bits(std::exchange(rhs.bits, {})) {}

				socket_impl& operator=(socket_impl&& rhs) noexcept
				{
					ctx = rhs.ctx;
					sockfd = std::exchange(rhs.sockfd, socket_base::invalid_socket);
					bits = std::exchange(rhs.bits, {});
					return *this;
				}

				~socket_impl() noexcept = default;

				socket_impl(const socket_impl&) = delete;
				socket_impl& operator=(const socket_impl&) = delete;

				native_handle_type native_handle() noexcept
				{
					return sockfd;
				}

				bool is_open() const noexcept
				{
					return sockfd != socket_base::invalid_socket;
				}

				void close(std::error_code& ec)
				{
					if (is_open())
					{
						cancel(ec);
						if(!ec)
						{
							int err;
							#ifdef _WIN32
								err = ::closesocket(sockfd);
							#else
								err = ::close(sockfd);
							#endif

							if(err == -1)
							{
								ec.assign(get_last_error(), std::generic_category());
							}
							else
							{
								sockfd = socket_base::invalid_socket;
							}
						}
					}
				}

				void cancel(std::error_code& ec)
				{
					// TODO
				}

				void non_blocking(bool mode, std::error_code& ec)
				{
					bits.non_blocking = mode;
				}

				bool non_blocking() const
				{
					return bits.non_blocking;
				}

				void native_non_blocking(bool mode, std::error_code& ec)
				{
					#if defined _WIN32
					auto mode_ul = static_cast<unsigned long>(mode);
					::ioctlsocket(sockfd, FIONBIO, &mode_ul);
					#elif defined F_GETFL
					auto flags = ::fcntl(sockfd, F_GETFL, 0);
					if(flags >= 0)
					{
						if(mode)
						{
							flags |= O_NONBLOCK;
						}
						else
						{
							flags &= ~O_NONBLOCK;
						}

						::fnctl(sockfd, F_SETFL, flags);
					}
					#else
					ec = std::make_error_code(std::errc::not_supported);
					return;
					#endif
					bits.native_non_blocking = mode;
				}

				bool native_non_blocking() const
				{
					#if defined _WIN32
					return bits.native_non_blocking;
					#elif defined F_GETFL
					const auto flags = ::fcntl(sockfd, F_GETFL, 0);
					if(flags == -1)
					{
						return false;
					}
					bits.native_non_blocking = falgs & O_NONBLOCK;
					return bits.native_non_blocking;
					#else
					return false;
					#endif
				}

				io_context* ctx;
				native_handle_type sockfd {socket_base::invalid_socket};

				struct
				{
					unsigned non_blocking : 1;
					mutable signed native_non_blocking : 2;
					unsigned enable_connection_aborted : 1;
				} bits {};
		};

		template<class Protocol>
		class basic_socket_impl : public socket_impl<Protocol>
		{
			using base = socket_impl;

			protected:
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				explicit basic_socket_impl(io_context& ctx)
					: base(ctx) {}

				basic_socket_impl(basic_socket_impl&&) noexcept = default;

				template<class OtherProtocol>
				basic_socket_impl(basic_socket_impl<OtherProtocol>&& rhs)
					: base(std::move(rhs)), protocol_(std::move(rhs.protocol)) {}

				basic_socket_impl& operator=(basic_socket_impl&& rhs) noexcept
				{
					if (this != std::addressof(rhs))
					{
						std::error_code ec;
						close(ec);
						base::operator=(std::move(rhs));
					}

					return *this;
				}

				~basic_socket_impl()
				{
					std::error_code ec;
					close(ec);
				}

				basic_socket_impl(const basic_socket_impl&) = delete;
				basic_socket_impl& operator=(const basic_socket_impl&) = delete;

				void open(const protocol_type& protocol, std::error_code& ec)
				{
					if (is_open())
					{
						ec = make_error_code(socket_errc::already_open);
					}
					else
					{
						protocol_ = protocol;
						sockfd = ::socket(static_cast<int>(protocol_.family()),
										  static_cast<int>(protocol_.type()),
										  static_cast<int>(protocol_.protocol()));

						if(is_open())
						{
							ec.clear();
						}
						else
						{
							ec.assign(get_last_error(), std::generic_category());
						}
					}
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_sockfd, std::error_code& ec)
				{
					if (is_open())
					{
						ec = make_error_code(socket_errc::already_open);
					}
					else
					{
						protocol_ = protocol;
						bits.native_non_blocking = -1;
						sockfd = native_sockfd;
						if(is_open())
						{
							ec.clear();
						}
						else
						{
							ec.assign(get_last_error(), std::generic_category());
						}
					}
				}

				native_handle_type release(std::error_code& ec)
				{
					cancel(ec);
					return std::exchange(sockfd, socket_base::invalid_socket);
				}

				template<class Option>
				void set_option(const Option option, std::error_code& ec)
				{
					const auto result = ::setsockopt(sockfd,
													 option.level(protocol_),
													 option.name(protocol_),
													 static_cast<const char*>(option.data(protocol_)),
													 option.size(protocol_));

					if(result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				template<class Option>
				void get_option(Option& option, std::error_code& ec) const
				{
					const auto result = ::getsockopt(sockfd,
													 option.level(protocol_),
													 option.name(protocol_),
													 option.data(protocol_),
													 option.size(protocol_));

					if(result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				template<class Control>
				void io_control(Control& control, std::error_code& ec)
				{
					const auto result =
					#ifdef _WIN32
					::ioctlsocket(sockfd, control.name(), control.data());
					#else
					::ioctl(sockfd, control.name(), control.data());
					#endif
					;

					if(result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				endpoint_type local_endpoint(std::error_code& ec)
				{
					endpoint_type endpoint;
					auto endpoint_len = endpoint.capacity();
					if(::getsockname(sockfd, static_cast<::sockaddr*>(endpoint.data()), &endpoint_len) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
						return endpoint_type{};
					}

					ec.clear();
					endpoint.resize(endpoint_len);
					return endpoint;
				}

				void bind(const endpoint_type& endpoint, std::error_code& ec)
				{
					if (::bind(sockfd, static_cast<::sockaddr*>(endpoint.data()), endpoint.size()) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				Protocol protocol_ {endpoint_type{}.protocol()};
		};

		template<class Protocol>
		class basic_socket : public socket_base, private basic_socket_impl<Protocol>
		{
			using base = basic_socket_impl<Protocol>;

			public:
				using native_handle_type = socket_type;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				native_handle_type native_handle() noexcept
				{
					return base::native_handle();
				}

				void open(const protocol_type& protocol, std::error_code& ec)
				{
					base::open(protocol, ec);
				}

				void open(const protocol_type& protocol)
				{
					open(protocol, collin::throw_on_error("basic_socket::open"));
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_socket, std::error_code& ec)
				{
					base::assign(protocol, native_socket, ec);
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_socket)
				{
					assign(protocol, native_socket, collin::throw_on_error("basic_socket::assign"));
				}

				native_handle_type release(std::error_code& ec)
				{
					return base::release(ec);
				}

				native_handle_type release()
				{
					return release(collin::throw_on_error("basic_socket::release"));
				}

				bool is_open() const noexcept
				{
					return base::is_open();
				}

				void close(std::error_code& ec)
				{
					base::close(ec);
				}

				void close()
				{
					close(collin::throw_on_error("basic_socket::close"));
				}

				void cancel(std::error_code& ec)
				{
					base::cancel(ec);
				}

				void cancel()
				{
					cancel(collin::throw_on_error("basic_socket::cancel"));
				}

				template<class SettableSocketOption>
				void set_option(const SettableSocketOption& option, std::error_code& ec)
				{
					base::set_option(option, ec);
				}

				template<class SettableSocketOption>
				void set_option(const SettableSocketOption& option)
				{
					set_option(option, collin::throw_on_error("basic_socket::set_option"));
				}

				template<class GettableSocketOption>
				void get_option(GettableSocketOption& option, std::error_code ec) const
				{
					base::get_option(option, ec);
				}

				template<class GettableSocketOption>
				void get_option(GettableSocketOption& option) const
				{
					get_option(option, collin::throw_on_error("basic_socket::get_option"));
				}

				template<class IOControlCommand>
				void io_control(IOControlCommand& command, std::error_code& ec)
				{
					base::io_control(command, ec);
				}

				template<class IOControlCommand>
				void io_control(IOControlCommand& command)
				{
					io_control(command, collin::throw_on_error("basic_socket::io_control"));
				}

				void non_blocking(bool mode, std::error_code& ec)
				{
					base::non_blocking(mode, ec);
				}

				void non_blocking(bool mode)
				{
					non_blocking(mode, collin::throw_on_error("basic_socket::non_blocking"));
				}

				bool non_blocking() const
				{
					return base::non_blocking();
				}

				void native_non_blocking(bool mode, std::error_code& ec)
				{
					base::native_non_blocking(mode, ec);
				}

				void native_non_blocking(bool mode)
				{
					native_non_blocking(mode, collin::throw_on_error("basic_socket::native_non_blocking"));
				}

				bool native_non_blocking() const
				{
					return base::native_non_blocking();
				}

				bool at_mark(std::error_code& ec) const
				{
					
					#ifdef _WIN32
					const auto result = return ::ioctlsocket(native_handle(), SIOCATMARK);
					#else
					const auto result = return ::sockatmark(native_handle());
					#endif

					if(result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
						return false;
					}

					ec.clear();
					return static_cast<bool>(result);
				}

				std::size_t available(std::error_code& ec)
				{
					if(!is_open())
					{
						ec = std::make_error_code(std::errc::bad_file_descriptor);
						return 0;
					}

					#if defined _WIN32
					std::size_t bytes_available{0};
					const auto err = ioctlsocket(native_handle(), FIONREAD, &bytes_available);
					#elif defined FIONREAD
					std::size_t bytes_available{0};
					const auto err = ioctl(native_handle(), FIONREAD, &bytes_available);
					#else
					return 0;
					#endif

					if(err == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
						return 0;
					}

					ec.clear();
					return bytes_available;
				}

				void bind(const endpoint_type& endpoint, std::error_code& ec)
				{
					return base::bind(endpoint, ec);
				}

				void bind(const endpoint_type& endpoint)
				{
					return bind(endpoint, collin::throw_on_error("basic_socket::bind"));
				}

				void shutdown(shutdown_type what, std::error_code& ec)
				{	
					if(::shutdown(native_handle(), static_cast<std::underlying_type_t<shutdown_type>(what)) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				void shutdown(shutdown_type what)
				{
					shutdown(what, collin::throw_on_error("basic_socket::shutdown"));
				}

				endpoint_type local_endpoint(std::error_code& ec)
				{
					return base::local_endpoint(ec);
				}

				endpoint_type local_endpoint()
				{
					return local_endpoint(collin::throw_on_error("basic_socket::local_endpoint"));
				}

				endpoint_type remote_endpoint(std::error_code& ec)
				{
					return base::remote_endpoint(ec);
				}

				endpoint_type remote_endpoint()
				{
					return remote_endpoint(collin::throw_on_error("basic_socket::remote_endpoint"));
				}

				void connect(const endpoint_type& endpoint, std::error_code& ec)
				{
					if(!is_open())
					{
						open(endpoint.protocol(), ec);
						if(ec)
						{
							return;
						}
					}

					if(::connect(native_handle(), static_cast<const ::sockaddr*>(endpoint.data()), endpoint.size()) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				void connect(const endpoint_type& endpoint)
				{
					connect(endpoint, collin::throw_on_error("basic_socket::connect"));
				}

				// async_connect() TODO

				int wait(wait_type w, std::error_code& ec)
				{
					::pollfd p{native_handle(), static_cast<std::underlying_type_t<wait_type>>(w)};
					#ifdef _WIN32
					const auto result = ::WSAPoll(&p, 1, std::chrono::milliseconds(200).count());
					#else
					const auto result = ::poll(&p, 1, std::chrono::milliseconds(200).count());
					#endif
					if (result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}

					return result;
				}

				int wait(wait_type w)
				{
					return wait(w, collin::throw_on_error("basic_socket::wait"));
				}

				// async_wait TODO

				explicit operator bool() const noexcept
				{
					return is_open();
				}

			protected:
				using base::base;
				
				explicit basic_socket(io_context& ctx) : base(ctx) {}

				basic_socket(io_context& ctx, const protocol_type& protocol)
					: base(ctx)
				{
					open(protocol);
				}

				basic_socket(io_context& ctx, const endpoint_type& endpoint)
					: basic_socket(ctx, endpoint.protocol())
				{
					bind(endpoint);
				}

				basic_socket(io_context& ctx, const protocol_type& protocol, const native_handle_type& handle)
					: base(ctx)
				{
					assign(protocol, handle);
				}

				basic_socket(const basic_socket&) = delete;
				basic_socket(basic_socket&& rhs) = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
				basic_socket(basic_socket<OtherProtocol>&& rhs)
					: base(std::move(rhs)) {}

				~basic_socket() noexcept = default;

				basic_socket& operator=(const basic_socket&) = delete;
				basic_socket& operator=(basic_socket&&) noexcept = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
					basic_socket& operator=(basic_socket<OtherProtocol>&& rhs)
				{
					return *this = basic_socket{std::move(rhs)};
				}
		};

		template<class AcceptableProtocol>
		class basic_socket_acceptor : public socket_base, private basic_socket_impl<AcceptableProtocol>
		{
			using base = basic_socket_impl<AcceptableProtocol>;

			public:
				using native_handle_type = socket_type;
				using protocol_type = AcceptableProtocol;
				using endpoint_type = typename protocol_type::endpoint;
				using socket_type = typename protocol_type::socket;

				explicit basic_socket_acceptor(io_context& ctx)
					: base(ctx), protocol_(endpoint_type{}.protocol()) {}

				basic_socket_acceptor(io_context& ctx, const protocol_type& protocol)
					: base(ctx), protocol_(protocol)
				{
					open(protocol);
				}

				basic_socket_acceptor(io_context& ctx, const endpoint_type& endpoint, bool reuse_addr = true)
					: basic_socket_acceptor(ctx, endpoint.protocol())
				{
					if (reuse_addr)
					{
						set_option(socket_base::reuse_address(true));
					}

					bind(endpoint);
					listen();
				}

				basic_socket_acceptor(io_context& ctx, const protocol_type& protocol, const native_handle_type& handle)
					: basic_socket_acceptor(ctx, protocol)
				{
					assign(protocol, handle);
				}

				~basic_socket_acceptor() = default;

				basic_socket_acceptor(const basic_socket_acceptor&) = delete;
				basic_socket_acceptor(basic_socket_acceptor&&) = default;

				basic_socket_acceptor& operator=(const basic_socket_acceptor&) = delete;
				basic_socket_acceptor& operator=(basic_socket_acceptor&&) = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
				basic_socket_acceptor& operator=(basic_socket_acceptor<OtherProtocol>&& rhs)
				{
					base::operator=(std::move(rhs));
					return *this;
				}

				native_handle_type native_handle()
				{
					return base::native_handle();
				}

				void open(const protocol_type& protocol, std::error_code& ec)
				{
					base::open(protocol, ec);
				}

				void open(const protocol_type& protocol = protocol_type())
				{
					open(protocol, collin::throw_on_error("basic_socket_acceptor::open"));
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_socket, std::error_code& ec)
				{
					base::assign(protocol, native_socket, ec);
				}

				void assign(const protocol_type& protocol, const native_handle_type& native_socket)
				{
					assign(protocol, native_socket, collin::throw_on_error("basic_socket_acceptor::assign"))
				}

				native_handle_type release(std::error_code& ec)
				{
					return base::release(ec);
				}

				native_handle_type release()
				{
					return release(collin::throw_on_error("basic_socket_acceptor::release"));
				}

				bool is_open() const noexcept
				{
					return base::is_open();
				}

				void close(std::error_code& ec)
				{
					base::close(ec);
				}

				void close()
				{
					close(collin::throw_on_error("basic_socket_acceptor::close"));
				}

				template<class SettableSocketOption>
				void set_option(const SettableSocketOption& option, std::error_code& ec)
				{
					base::set_option(option, ec);
				}

				template<class SettableSocketOption>
				void set_option(const SettableSocketOption& option)
				{
					set_option(option, collin::throw_on_error("basic_socket_acceptor::set_option"));
				}

				template<class GettableSocketOption>
				void get_option(GettableSocketOption& option, std::error_code ec) const
				{
					base::get_option(option, ec);
				}

				template<class GettableSocketOption>
				void get_option(GettableSocketOption& option) const
				{
					get_option(option, collin::throw_on_error("basic_socket_acceptor::get_option"));
				}

				template<class IOControlCommand>
				void io_control(IOControlCommand& command, std::error_code& ec)
				{
					base::io_control(command, ec);
				}

				template<class IOControlCommand>
				void io_control(IOControlCommand& command)
				{
					io_control(command, collin::throw_on_error("basic_socket_acceptor::io_control"));
				}

				void non_blocking(bool mode, std::error_code& ec)
				{
					base::non_blocking(mode, ec);
				}

				void non_blocking(bool mode)
				{
					non_blocking(mode, collin::throw_on_error("basic_socket_acceptor::non_blocking"));
				}

				bool non_blocking() const
				{
					return base::non_blocking();
				}

				void native_non_blocking(bool mode, std::error_code& ec)
				{
					base::native_non_blocking(mode, ec);
				}

				void native_non_blocking(bool mode)
				{
					native_non_blocking(mode, collin::throw_on_error("basic_socket_acceptor::native_non_blocking"));
				}

				bool native_non_blocking() const
				{
					return base::native_non_blocking();
				}

				bool at_mark(std::error_code& ec) const
				{

					#ifdef _WIN32
					const auto result = return ::ioctlsocket(native_handle(), SIOCATMARK);
					#else
					const auto result = return ::sockatmark(native_handle());
					#endif

					if (result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
						return false;
					}

					ec.clear();
					return static_cast<bool>(result);
				}

				std::size_t available(std::error_code& ec)
				{
					if (!is_open())
					{
						ec = std::make_error_code(std::errc::bad_file_descriptor);
						return 0;
					}

					#if defined _WIN32
					std::size_t bytes_available{ 0 };
					const auto err = ioctlsocket(native_handle(), FIONREAD, &bytes_available);
					#elif defined FIONREAD
					std::size_t bytes_available{ 0 };
					const auto err = ioctl(native_handle(), FIONREAD, &bytes_available);
					#else
					return 0;
					#endif

					if (err == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
						return 0;
					}

					ec.clear();
					return bytes_available;
				}

				void bind(const endpoint_type& endpoint, std::error_code& ec)
				{
					base::bind(endpoint, ec);
				}

				void bind(const endpoint_type& endpoint)
				{
					bind(endpoint, collin::throw_on_error("basic_socket_acceptor::bind"));
				}

				void listen(int backlog, std::error_code& ec)
				{
					if (::listen(native_handle(), backlog) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				void listen(int backlog = socket_base::max_listen_connections)
				{
					listen(backlog, collin::throw_on_error("basic_socket_acceptor::listen"));
				}

				void enable_connection_aborted(bool mode)
				{
					base::bits.enable_connection_aborted = mode;
				}

				bool enable_connection_aborted() const
				{
					return base::bits.enable_connection_aborted;
				}

				socket_type accept(io_context& ctx, std::error_code& ec)
				{
					do
					{
						const auto h = ::accept(native_handle(), nullptr, 0);
						if(h != -1)
						{
							ec.clear();
							return socket_type{ctx, protocol_, h};
						}
					} while(get_last_error() == ECONNABORTED && enable_connection_aborted());

					ec.assign(get_last_error(), std::generic_category());

					return socket_type{ctx};
				}

				socket_type accept(io_context& ctx, endpoint_type& endpoint, std::error_code& ec)
				{
					do
					{
						auto len = endpoint.capacity();
						const auto h = ::accept(native_handle(), static_cast<::sockaddr*>(endpoint.data()), &len);
						if (h != -1)
						{
							ec.clear();
							endpoint.resize(len);
							return socket_type{ctx, protocol_, h};
						}
					} while (get_last_error() == ECONNABORTED && enable_connection_aborted());

					ec.assign(get_last_error(), std::generic_category());

					return socket_type{ctx};
				}

				socket_type accept(std::error_code& ec)
				{
					return accept(collin::net::io_context{}, endpoint_type{}, ec);
				}

				socket_type accept()
				{
					return accept(collin::net::io_context{}, endpoint_type{}, collin::throw_on_error("basic_socket_acceptor::accept"));
				}

				socket_type accept(io_context& ctx)
				{
					return accept(ctx, endpoint_type{}, collin::throw_on_error("basic_socket_acceptor::accept"));
				}

				socket_type accept(endpoint_type& endpoint)
				{
					return acccept(collin::net::io_context{}, endpoint, collin::throw_on_error("basic_socket_acceptor::accept"))
				}

				socket_type accept(endpoint_type& endpoint, std::error_code& ec)
				{
					return accept(collin::net::io_context{}, endpoint, ec);
				}

				socket_type accept(io_context& ctx, endpoint_type& endpoint)
				{
					return accept(ctx, endpoint, collin::throw_on_error("basic_socket_acceptor::accept"));
				}

				std::future<socket_type> async_accept(io_context& ctx, endpoint_type& endpoint, std::error_code& ec, const std::launch l = std::launch::async)
				{
					return std::async(l, &basic_socket_acceptor::accept, std::ref(ctx), std::ref(endpoint), std::ref(ec));
				}

				std::future<socket_type> async_accept(std::error_code& ec, const std::launch l = std::launch::async)
				{
					return async_accept(collin::net::io_context{}, endpoint_type{}, ec, l);
				}

				std::future<socket_type> async_accept(const std::launch l = std::launch::async)
				{
					return async_accept(collin::net::io_context{}, endpoint_type{}, collin::throw_on_error("basic_socket_acceptor::async_accept"), l);
				}

				std::future<socket_type> async_accept(io_context& ctx, const std::launch l = std::launch::async)
				{
					return async_accept(ctx, endpoint_type{}, collin::throw_on_error("basic_socket_acceptor::async_accept"), l);
				}

				std::future<socket_type> async_accept(endpoint_type& endpoint, const std::launch l = std::launch::async)
				{
					return async_acccept(collin::net::io_context{}, endpoint, collin::throw_on_error("basic_socket_acceptor::async_accept"), l)
				}

				std::future<socket_type> async_accept(endpoint_type& endpoint, std::error_code& ec, const std::launch l = std::launch::async)
				{
					return async_accept(collin::net::io_context{}, endpoint, ec, l);
				}

				std::future<socket_type> async_accept(io_context& ctx, endpoint_type& endpoint, const std::launch l = std::launch::async)
				{
					return async_accept(ctx, endpoint, collin::throw_on_error("basic_socket_acceptor::async_accept"), l);
				}

				void shutdown(shutdown_type what, std::error_code& ec)
				{
					if (::shutdown(native_handle(), static_cast<std::underlying_type_t<shutdown_type>(what)) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				void shutdown(shutdown_type what)
				{
					shutdown(what, collin::throw_on_error("basic_socket_acceptor::shutdown"));
				}

				endpoint_type local_endpoint(std::error_code& ec)
				{
					return base::local_endpoint(ec);
				}

				endpoint_type local_endpoint()
				{
					return local_endpoint(collin::throw_on_error("basic_socket_acceptor::local_endpoint"));
				}

				endpoint_type remote_endpoint(std::error_code& ec)
				{
					return base::remote_endpoint(ec);
				}

				endpoint_type remote_endpoint()
				{
					return remote_endpoint(collin::throw_on_error("basic_socket_acceptor::remote_endpoint"));
				}

				void connect(const endpoint_type& endpoint, std::error_code& ec)
				{
					if (!is_open())
					{
						open(endpoint.protocol(), ec);
						if (ec)
						{
							return;
						}
					}

					if (::connect(native_handle(), static_cast<const ::sockaddr*>(endpoint.data()), endpoint.size()) == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}
				}

				std::future<void> async_connect(const endpoint_type& endpoint, std::error_code& ec, const std::launch l = std::launch::async)
				{
					return std::async(l, &basic_socket_acceptor::connect, std::cref(endpoint), std::ref(ec));
				}

				int wait(wait_type w, std::error_code& ec)
				{
					::pollfd p{ native_handle(), static_cast<std::underlying_type_t<wait_type>>(w) };
					#ifdef _WIN32
					const auto result = ::WSAPoll(&p, 1, std::chrono::milliseconds(200).count());
					#else
					const auto result = ::poll(&p, 1, std::chrono::milliseconds(200).count());
					#endif
					if (result == -1)
					{
						ec.assign(get_last_error(), std::generic_category());
					}
					else
					{
						ec.clear();
					}

					return result;
				}

				int wait(wait_type w)
				{
					return wait(w, collin::throw_on_error("basic_socket::wait"));
				}

				std::future<int> async_wait(wait_type w, std::error_code& ec, const std::launch l = std::launch::async)
				{
					return std::async(l, &basic_socket_acceptor::wait, w, std::ref(ec));
				}

				std::future<int> async_wait(wait_type w, const std::launch l = std::launch::async)
				{
					return async_wait(w, collin::throw_on_error("basic_socket::async_wait"), l);
				}

				explicit operator bool() const noexcept
				{
					return is_open();
				}
		};

		template<class Protocol>
		class basic_datagram_socket : public basic_socket<Protocol>
		{
			using base = basic_socket<Protocol>;

			public:
				using native_handle_type = basic_socket<Protocol>::native_handle_type;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				explicit basic_datagram_socket(io_context& ctx)
					: base(ctx) {}

				basic_datagram_socket(io_context& ctx, const protocol_type& protocol)
					: base(ctx, protocol) {}

				basic_datagram_socket(io_context& ctx, const endpoint_type& endpoint)
					: base(ctx, endpoint) {}

				basic_datagram_socket(io_context& ctx, const protocol_type& protocol, const native_handle_type& handle)
					: base(ctx, protocol, handle) {}

				basic_datagram_socket(const basic_datagram_socket&) = delete;

				basic_datagram_socket(basic_datagram_socket&&) = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
					basic_datagram_socket(basic_datagram_socket<OtherProtocol>&& rhs)
						: base(std::move(rhs)) {}

				~basic_datagram_socket() = default;

				basic_datagram_socket& operator=(const basic_datagram_socket&) = delete;

				basic_datagram_socket& operator=(basic_datagram_socket&& rhs) = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
					basic_datagram_socket & operator=(basic_datagram_socket<OtherProtocol>&& rhs)
				{
					base::operator=(std::move(rhs));
					return *this;
				}

				template<class ElementType, std::size_t Extent>
				std::size_t receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					const auto result = socket_receive(native_handle(), buf, flags, ec);
					if (ec)
					{
						close();
					}
					return result;
				}

				template<class ElementType, std::size_t Extent>
				std::size_t receive(span<ElementType, Extent> buf, int flags = 0)
				{
					return receive(buf, collin::throw_on_error{"basic_datagram_socket::receive"}, flags);
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return std::async(l, &basic_datagram_socket::receive, buf, std::ref(ec), flags);
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_receive(span<ElementType, Extent> buf, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return async_receive(buf, collin::throw_on_error{"basic_datagram_socket::async_receive"}, flags, l);
				}

				template<class ElementType, std::size_t Extent>
				std::size_t send(const span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					const auto result = socket_send(native_handle(), buf, flags, ec);
					if (ec)
					{
						close();
					}
					return result;
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_send(const span<ElementType, Extent> buf, std::error_code& ec, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return std::async(l, &basic_datagram_socket::send, std::ref(ec), flags);
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_send(const span<ElementType, Extent> buf, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return async_send(buf, collin::throw_on_error{"basic_datagram_socket::async_send"}, flags, l);
				}
		};

		template<class Protocol>
		class basic_stream_socket : public basic_socket<Protocol>
		{
			using base = basic_socket<Protocol>;

			public:
				using native_handle_type = basic_socket<Protocol>::native_handle_type;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				explicit basic_stream_socket(net::io_context& ctx)
					: base(ctx) {}

				basic_stream_socket(net::io_context& ctx, const protocol_type& protocol)
					: base(ctx, protocol) {}

				basic_stream_socket(net::io_context& ctx, const endpoint_type& endpoint)
					: base(ctx, endpoint) {}

				basic_stream_socket(net::io_context& ctx, const protocol_type& protocol, const native_handle_type& handle)
					: base(ctx, protocol, handle) {}

				basic_stream_socket(const basic_stream_socket&) = delete;
				basic_stream_socket(basic_stream_socket&& rhs) noexcept = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
				basic_stream_socket(basic_stream_socket<OtherProtocol>&& rhs)
					: base(std::move(rhs)) {}

				~basic_stream_socket() noexcept = default;

				basic_stream_socket& operator=(const basic_stream_socket&) = delete;

				basic_stream_socket& operator=(basic_stream_socket &&) noexcept = default;

				template<collin::concepts::convertible_to<Protocol> OtherProtocol>
					basic_stream_socket & operator=(basic_stream_socket<OtherProtocol>&& rhs)
				{
					base::operator=(std::move(rhs));
					return *this;
				}

				template<class ElementType, std::size_t Extent>
				std::size_t receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					const auto result = socket_receive(native_handle(), buf, flags, ec);
					if (ec)
					{
						close();
					}
					return result;
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_receive(span<ElementType, Extent> buf, std::error_code& ec, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return std::async(l, &basic_stream_socket::receive, buf, std::ref(ec), flags);
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_receive(span<ElementType, Extent> buf, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return async_receive(buf, collin::throw_on_error{ "basic_stream_socket::async_receive" }, flags, l);
				}

				template<class ElementType, std::size_t Extent>
				std::size_t send(const span<ElementType, Extent> buf, std::error_code& ec, int flags = 0) noexcept
				{
					const auto result = socket_send(native_handle(), buf, flags, ec);
					if (ec)
					{
						close();
					}
					return result;
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_send(const span<ElementType, Extent> buf, std::error_code& ec, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return std::async(l, &basic_stream_socket::send, std::ref(ec), flags);
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_send(const span<ElementType, Extent> buf, int flags = 0, const std::launch l = std::launch::async) noexcept
				{
					return async_send(buf, collin::throw_on_error{ "basic_stream_socket::async_send" }, flags, l);
				}
		};

		template<class Protocol>
		class basic_socket_streambuf : public std::basic_streambuf<char>
		{
			public:
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_socket_streambuf()
					: socket_(ctx())
				{
					set_buffers();
				}

				explicit basic_socket_streambuf(basic_stream_socket<protocol_type>&& s)
					: socket_(std::move(s))
				{
					set_buffers();
				}

				basic_socket_streambuf(const basic_socket_streambuf&) = delete;

				basic_socket_streambuf(basic_socket_streambuf&& rhs)
					: std::basic_streambuf<char>(std::move(rhs)), socket_(std::move(rhs.socket_))
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
						get_buf = std::exchange(rhs.get_buf, {});
						put_buf = std::exchange(rhs.put_buf, {});
						set_buffers();
					}

					return *this;
				}
				
				template<class... Args>
					requires (sizeof...(Args) == 1 ? 
								!collin::concepts::convertible_to<std::tuple_element_t<0, std::tuple<Args...>>, const endpoint_type&>
								: true)
				basic_socket_streambuf* connect(Args&&... args)
				{
					const auto results = protocol_type::resolver(ctx()).resolve(std::forward<Args>(args)...);
					for (const auto& result : results)
					{
						auto connection_result = this->connect(result.endpoint());
						if (connection_result != nullptr)
						{
							return connection_result;
						}
					}

					return nullptr;
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

				basic_stream_socket<protocol_type>& socket()
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

					buffer_sequence seq = {msgbuf(get_buf)};
					const auto bytes_read = socket_receive(socket_.native_handle(), seq.get_span(), 0, ec_);



					if (bytes_read > 0)
					{
						setg(get_buf.data(), get_buf.data(), get_buf.data() + bytes_read);
						return std::istream::traits_type::to_int_type(*gptr());
					}

					return std::istream::traits_type::eof();
				}

				virtual std::ios::int_type overflow(std::ios::int_type c = std::istream::traits_type::eof()) override
				{
					auto ch = std::istream::traits_type::to_char_type(c);
					auto begin = &ch;
					auto size = sizeof(std::istream::traits_type::char_type);
					if(put_buf[0] != '\0')
					{
						begin = put_buf.data();
						size = (pptr() - pbase()) * sizeof(std::istream::traits_type::char_type);
					}

					buffer_sequence seq = {msgbuf(begin, size)};
					socket_send(socket_.native_handle(), seq.get_span(), 0, ec_);

					if (ec_ && ec_ != make_error_code(socket_errc::try_again))
					{
						return std::istream::traits_type::eof();
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

				static io_context& ctx()
				{
					static io_context ctx_;
					return ctx_;
				}

				basic_stream_socket<protocol_type> socket_;
				std::array<char, buffer_size> get_buf;
				std::array<char, buffer_size> put_buf;
				std::error_code ec_;

				void set_buffers() noexcept
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

				basic_socket_iostream()
					: std::basic_iostream<char>(&sb_), sb_()
				{
					setf(std::ios_base::unitbuf);
				}

				explicit basic_socket_iostream(basic_stream_socket<protocol_type>&& s)
					: std::basic_iostream<char>(&sb_), sb_(std::move(s))
				{
					setf(std::ios_base::unitbuf);
				}

				basic_socket_iostream(const basic_socket_iostream&) = delete;
				basic_socket_iostream(basic_socket_iostream&& rhs) noexcept
					: std::basic_iostream<char>(std::addressof(sb_)), sb_(std::move(rhs.sb_))
				{
					//set_rdbuf(std::addressof(sb_));
				}

				template<class... Args>
				explicit basic_socket_iostream(Args&&... args)
					: std::basic_iostream<char>(&sb_), sb_()
				{
					setf(std::ios_base::unitbuf);
					connect(std::forward<Args>(args)...);
				}

				basic_socket_iostream& operator=(const basic_socket_iostream&) = delete;
				basic_socket_iostream& operator=(basic_socket_iostream&& rhs)
				{
					if (this != &rhs)
					{
						sb_ = std::move(rhs.sb_);
						std::basic_iostream::operator=(std::move(rhs));
					}
				}

				template<class... Args>
				inline void connect(Args&&... args)
				{
					if (rdbuf()->connect(std::forward<Args>(args)...) == nullptr)
					{
						setstate(std::ios::failbit);
					}
				}

				inline void close()
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

				basic_stream_socket<protocol_type>& socket()
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
			ec.clear();
			auto found = false;
			for(auto& ep : endpoints)
			{
				if(c(ec, ep))
				{
					found = true;
					s.close(ec);
					if(!ec)
						s.open(ep.protocol(), ec);
					if(!ec)
						s.connect(ep, ec);
					if (!ec)
						return ep;
				}
			}

			if(!found)
				ec = std::make_error_code(socket_errc::not_found);

			return typename Protocol::endpoint{};
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
