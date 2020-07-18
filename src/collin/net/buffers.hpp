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

#include <array>
#include <vector>
#include <type_traits>
#include "collin/span.hpp"

namespace collin
{
	namespace net
	{
		using msgtype =
		#if _WIN32
		WSABUF
		#else
		struct iovec
		#endif
		;

		struct msgbuf : public msgtype
		{
			using pointer_type =
			#ifdef _WIN32
				decltype(std::declval<msgtype>().buf)
			#else
				decltype(std::declval<msgtype>().iov_base)
			#endif
			;

			using len_type =
			#ifdef _WIN32
				decltype(std::declval<msgtype>().len)
			#else
				decltype(std::declval<msgtype>().iov_len)
			#endif
			;

			using data_type = std::remove_pointer_t<pointer_type>;

			constexpr msgbuf() noexcept
				: msgtype{} {}

			constexpr msgbuf(pointer_type p, len_type l)
				:
					#ifdef _WIN32
					msgtype{l, p}
					#else
					msgtype{p, l}
					#endif
				{}

			constexpr msgbuf(len_type l, pointer_type p)
				: msgbuf{p, l} {}

			constexpr msgbuf(const msgtype& t)
				: msgtype{t} {}

			template<class ElementType, std::size_t Extent>
			constexpr msgbuf(span<ElementType, Extent> s)
				: msgbuf{static_cast<pointer_type>(s.data()), static_cast<len_type>(s.size())} {}

			template<class Type, std::size_t N>
			constexpr msgbuf(std::array<Type, N> & a)
				: msgbuf{static_cast<pointer_type>(a.data()), static_cast<len_type>(N)} {}

			constexpr operator span<data_type, collin::dynamic_extent>() const
			{
				return collin::make_span(buf, len);
			}
		};

		static_assert(sizeof(msgtype) == sizeof(msgbuf), "Something is very wrong");
		static_assert(std::is_convertible_v<decltype(std::declval<msgbuf>()), msgtype>, "Something is very wrong");

		class buffer_sequence
		{
			public:
				buffer_sequence() = default;

				template<class... Args>
				buffer_sequence(Args&&... args)
					: msgs{ {std::forward<Args>(args)...} } {}

				buffer_sequence(std::initializer_list<msgbuf> bufs)
					: msgs{ bufs } {}

				buffer_sequence& operator=(std::initializer_list<msgbuf> bufs)
				{
					msgs = bufs;
					return *this;
				}

				msgtype* buffers() noexcept
				{
					return msgs.data();
				}

				const msgtype* buffers() const noexcept
				{
					return msgs.data();
				}

				operator span<msgtype, collin::dynamic_extent>() noexcept
				{
					return make_span(buffers(), size());
				}

				auto get_span() noexcept
				{
					return operator span<msgtype, collin::dynamic_extent>();
				}

				std::size_t size() const noexcept
				{
					return std::size(msgs);
				}
			private:
				std::vector<msgbuf> msgs;
		};
	}
}