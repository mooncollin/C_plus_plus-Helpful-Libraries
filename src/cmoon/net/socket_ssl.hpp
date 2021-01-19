#pragma once

#include <iostream>
#include <span>
#include <future>
#include <limits>
#include <algorithm>
#include <initializer_list>

#include "cmoon/net/iocontext.hpp"
#include "cmoon/net/buffers.hpp"
#include "cmoon/net/socket.hpp"
#include "cmoon/net/openssl_socket_service.hpp"
#include "cmoon/net/windows_socket_service.hpp"
#include "cmoon/utility.hpp"

#include <openssl/ssl.h>
#include <openssl/err.h>

namespace cmoon
{
	namespace net
	{
		using ssl_socket_type = SSL*;

		template<class ElementType, std::size_t Extent>
		std::size_t socket_receive(ssl_socket_type s, std::span<ElementType, Extent> buf, std::error_code& ec) noexcept
		{
			ERR_clear_error();
			ec.clear();
			auto bytes = std::as_writable_bytes(buf);
			std::size_t total = 0;
			constexpr std::size_t max_int = static_cast<std::size_t>(std::numeric_limits<int>::max());
			while (total != bytes.size())
			{
				const auto result = SSL_read(s, bytes.data() + total, static_cast<int>(std::min(std::initializer_list<std::size_t>{bytes.size() - total, max_int})));
				const auto last_error = ERR_get_error();
				switch (last_error)
				{
					case SSL_ERROR_WANT_READ:
						ec = make_error_code(socket_errc::try_again);
						break;
					case 0:
						break;
					default:
						ec.assign(last_error, std::generic_category());
				}

				if (ec || result <= 0)
				{
					return total;
				}

				total += result;
			}

			return total;
		}

		template<class ElementType, std::size_t Extent>
		std::size_t socket_send(ssl_socket_type s, std::span<const ElementType, Extent> buf, std::error_code& ec) noexcept
		{
			ERR_clear_error();
			ec.clear();
			const auto bytes = std::as_bytes(buf);
			constexpr std::size_t max_int = static_cast<std::size_t>(std::numeric_limits<int>::max());
			std::size_t total = 0;
			while (total != bytes.size())
			{
				const auto result = SSL_write(s, bytes.data() + total, static_cast<int>(std::min(std::initializer_list<std::size_t>{bytes.size() - total, max_int})));
				const auto last_error = ERR_get_error();
				switch (last_error)
				{
					case SSL_ERROR_WANT_WRITE:
						ec = make_error_code(socket_errc::try_again);
						break;
					case 0:
						break;
					default:
						ec.assign(last_error, std::generic_category());
				}

				if (ec || result <= 0)
				{
					return total;
				}

				total += result;
			}

			return total;
		}

		template<class Protocol>
		class basic_stream_ssl_socket : public basic_stream_socket<Protocol>
		{
			using base = basic_stream_socket<Protocol>;

			public:
				using native_handle_type = typename base::native_handle_type;
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				explicit basic_stream_ssl_socket(net::io_context& ctx)
					: base{ctx}, ssl{SSL_new(cmoon::net::use_service<cmoon::net::openssl_socket_service>(ctx).get_context().get_context())}
				{
				}

				basic_stream_ssl_socket(net::io_context& ctx, const protocol_type& procotol)
					: base{ctx, procotol}, ssl{SSL_new(cmoon::net::use_service<cmoon::net::openssl_socket_service>(ctx).get_context().get_context())}
				{
					SSL_set_fd(ssl, base::native_handle());
				}

				basic_stream_ssl_socket(net::io_context& ctx, const endpoint_type& endpoint)
					: base{ctx, endpoint}, ssl{SSL_new(cmoon::net::use_service<cmoon::net::openssl_socket_service>(ctx).get_context().get_context())}
				{
					SSL_set_fd(ssl, base::native_handle());
				}

				basic_stream_ssl_socket(net::io_context& ctx, const protocol_type& protocol, const native_handle_type& handle)
					: base{ctx, protocol, handle}, ssl{SSL_new(cmoon::net::use_service<cmoon::net::openssl_socket_service>(ctx).get_context().get_context())}
				{
					SSL_set_fd(ssl, base::native_handle());
				}

				basic_stream_ssl_socket(const basic_stream_ssl_socket&) = delete;
				basic_stream_ssl_socket(basic_stream_ssl_socket&& rhs) noexcept
					: base{std::move(static_cast<base&>(rhs))}, ssl{std::exchange(rhs.ssl, nullptr)} {}

				basic_stream_ssl_socket& operator=(const basic_stream_ssl_socket&) = delete;
				basic_stream_ssl_socket& operator=(basic_stream_ssl_socket&& rhs) noexcept
				{
					if (this != std::addressof(rhs))
					{
						base::operator=(std::move(static_cast<base&>(rhs)));
						ssl = std::exchange(rhs.ssl, nullptr);
					}

					return *this;
				}

				~basic_stream_ssl_socket()
				{
					if (ssl != nullptr)
					{
						SSL_shutdown(ssl);
						SSL_free(ssl);
					}
				}

				void open(const protocol_type& procotol, std::error_code& ec) noexcept
				{
					base::open(procotol, ec);
					if (!ec)
					{
						SSL_set_fd(ssl, static_cast<int>(base::native_handle()));
					}
				}

				void assign(const protocol_type& protocol, base::native_handle_type native_sockfd, std::error_code& ec) noexcept
				{
					base::assign(protocol, native_sockfd, ec);
					if (!ec)
					{
						SSL_set_fd(ssl, base::native_handle());
					}
				}

				void connect(const endpoint_type& endpoint, std::error_code& ec) noexcept
				{
					base::connect(endpoint, ec);
					if (!ec)
					{
						if (const auto status = SSL_connect(ssl); status != 1)
						{
							ec.assign(SSL_get_error(ssl, status), std::generic_category());
						}
					}
				}

				[[nodiscard]] native_handle_type release(std::error_code& ec) noexcept
				{
					const auto handle = base::release(ec);
					SSL_clear(ssl);
					return handle;
				}

				void connect(const endpoint_type& endpoint)
				{
					connect(endpoint, cmoon::throw_on_error{"basic_stream_ssl_socket::connect"});
				}

				void close(std::error_code& ec)
				{
					base::close(ec);
					ERR_clear_error();
					SSL_clear(ssl);
					if (const auto err = ERR_get_error(); err != 0)
					{
						ec.assign(err, std::generic_category());
					}
				}

				// async_connect() TODO

				template<class ElementType, std::size_t Extent>
				std::size_t receive(std::span<ElementType, Extent> buf, std::error_code& ec) noexcept
				{
					const auto result = socket_receive(ssl, buf, ec);
					if (ec)
					{
						close();
					}

					return result;
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_receive(std::span<ElementType, Extent> buf, std::error_code& ec, const std::launch l = std::launch::async)
				{
					return std::async(l, &basic_stream_ssl_socket::receive, buf, std::ref(ec));
				}

				template<class ElementType, std::size_t Extent>
				std::size_t send(std::span<const ElementType, Extent> buf, std::error_code& ec) noexcept
				{
					const auto result = socket_send(base::native_handle(), buf, ec);
					if (ec)
					{
						close();
					}
					return result;
				}

				template<class ElementType, std::size_t Extent>
				std::future<std::size_t> async_send(std::span<const ElementType, Extent> buf, std::error_code& ec, const std::launch l = std::launch::async) noexcept
				{
					return std::async(l, &basic_stream_ssl_socket::send, std::ref(ec));
				}

				ssl_socket_type ssl_handle() noexcept
				{
					return ssl;
				}
			private:
				ssl_socket_type ssl;
		};

		template<class Protocol>
		class basic_socket_ssl_streambuf : public std::basic_streambuf<char>
		{
			using base = std::basic_streambuf<char>;

			public:
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_socket_ssl_streambuf()
					: socket_{ctx()}
				{
					set_buffers();
				}

				explicit basic_socket_ssl_streambuf(basic_stream_ssl_socket<protocol_type>&& s) noexcept
					: socket_{std::move(s)}
				{
					set_buffers();
				}

				basic_socket_ssl_streambuf(const basic_socket_ssl_streambuf&) = delete;
				basic_socket_ssl_streambuf(basic_socket_ssl_streambuf&& rhs) noexcept
					: base{std::move(static_cast<base&>(rhs))}, socket_{std::move(rhs.socket_)}
				{
					set_buffers();
				}

				virtual ~basic_socket_ssl_streambuf()
				{
					if (pptr() != pbase())
					{
						overflow(std::istream::traits_type::eof());
					}
					close();
				}

				basic_socket_ssl_streambuf& operator=(const basic_socket_ssl_streambuf&) = delete;
				basic_socket_ssl_streambuf& operator=(basic_socket_ssl_streambuf&& rhs) noexcept
				{
					if (this != std::addressof(rhs))
					{
						close();
						socket_ = std::move(rhs.socket_);
						base::operator=(std::move(static_cast<base&>(rhs)));
						get_buf = std::exchange(rhs.get_buf, {});
						put_buf = std::exchange(rhs.put_buf, {});
						set_buffers();
					}

					return *this;
				}

				template<class... Args>
					requires (sizeof...(Args) == 1 ? !std::is_convertible_v<std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>, endpoint_type> : true)
				basic_socket_ssl_streambuf* connect(Args&&... args)
				{
					const auto results = protocol_type::resolver(ctx()).resolve(std::forward<Args>(args)...);
					for (const auto& result : results)
					{
						auto connection_result = connect(result.endpoint());
						if (connection_result != nullptr)
						{
							return connection_result;
						}
					}

					return nullptr;
				}

				basic_socket_ssl_streambuf* connect(const endpoint_type& e)
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

				basic_socket_ssl_streambuf* close()
				{
					ec_.clear();
					if (socket_.is_open())
					{
						overflow(std::istream::traits_type::eof());
						ec_.clear();
						socket_.close(ec_);
					}
					return ec_ ? nullptr : this;
				}

				basic_stream_ssl_socket<protocol_type>& socket() noexcept
				{
					return socket_;
				}

				std::error_code error() const noexcept
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

					std::span<char, buffer_size> buf{get_buf};

					const auto bytes_read = socket_receive(socket_.ssl_handle(), buf, ec_);

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
					auto begin = std::addressof(ch);
					auto size = sizeof(std::istream::traits_type::char_type);
					if (put_buf.front() != '\0')
					{
						begin = put_buf.data();
						size = (pptr() - pbase()) * sizeof(std::istream::traits_type::char_type);
					}

					const std::span<const decltype(ch), std::dynamic_extent> buf {begin, size};

					socket_send(socket_.ssl_handle(), buf, ec_);

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
				static constexpr std::size_t buffer_size {4096};

				static io_context& ctx() noexcept
				{
					static io_context ctx_;
					if constexpr (cmoon::win32_api)
					{
						make_service<windows_socket_service>(ctx_);
					}
					
					make_service<openssl_socket_service>(ctx_);
					return ctx_;
				}

				basic_stream_ssl_socket<protocol_type> socket_;
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
		class basic_socket_ssl_iostream : public std::basic_iostream<char>
		{
			using base = std::basic_iostream<char>;

			public:
				using protocol_type = Protocol;
				using endpoint_type = typename protocol_type::endpoint;

				basic_socket_ssl_iostream()
					: base{std::addressof(sb_)}
				{
					setf(std::ios_base::unitbuf);
				}

				explicit basic_socket_ssl_iostream(basic_stream_ssl_socket<protocol_type>&& s) noexcept
					: sb_{std::move(s)}, base{std::addressof(sb_)}
				{
					setf(std::ios_base::unitbuf);
				}

				basic_socket_ssl_iostream(const basic_socket_ssl_iostream&) = delete;
				basic_socket_ssl_iostream(basic_socket_ssl_iostream&& rhs) noexcept
					: sb_{std::move(rhs.sb_)}, base{std::addressof(sb_)} {}

				template<class... Args>
				explicit basic_socket_ssl_iostream(Args&&... args)
					: base{std::addressof(sb_)}
				{
					setf(std::ios_base::unitbuf);
					connect(std::forward<Args>(args)...);
				}

				basic_socket_ssl_iostream& operator=(const basic_socket_ssl_iostream&) = delete;
				basic_socket_ssl_iostream& operator=(basic_socket_ssl_iostream&& rhs) noexcept
				{
					if (this != std::addressof(rhs))
					{
						sb_ = std::move(rhs.sb_);
						base::operator=(std::move(rhs));
					}

					return *this;
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

				basic_socket_ssl_streambuf<protocol_type>* rdbuf() const
				{
					return std::addressof(sb_);
				}

				basic_stream_ssl_socket<protocol_type>& socket() noexcept
				{
					return rdbuf()->socket();
				}

				const basic_stream_ssl_socket<protocol_type>& socket() const noexcept
				{
					return rdbuf()->socket();
				}

				std::error_code error() const noexcept
				{
					return rdbuf()->error();
				}
			private:
				mutable basic_socket_ssl_streambuf<protocol_type> sb_ {};
		};
	}
}