#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <stdexcept>
#include <optional>
#include <utility>
#include <cstring>
#include <future>
#include <type_traits>
#include <algorithm>
#include <sstream>
#include <array>

#include "collin/net/socket.hpp"
#include "collin/net/internet.hpp"
#include "collin/net/iocontext.hpp"
#include "collin/iostream.hpp"
#include "collin/iterator.hpp"
#include "collin/string.hpp"

namespace collin
{
	namespace http
	{
		inline constexpr net::ip::port_type http_default_port {80};
		inline constexpr std::string_view host_header {"Host"};
		inline constexpr std::string_view content_length_header {"Content-Length"};
		inline constexpr std::string_view connection_header {"Connection"};
		inline constexpr std::string_view connection_keep_alive {"keep-alive"};

		enum class http_version
		{
			HTTP_1,
			HTTP_1_1,
			HTTP_2,
			HTTP_3
		};

		enum class http_method
		{
			GET,
			PUT,
			POST
		};

		constexpr std::string_view http_1 {"HTTP/1.0"};
		constexpr std::string_view http_1_1 {"HTTP/1.1"};
		constexpr std::string_view http_2 {"HTTP/2"};
		constexpr std::string_view http_3 {"HTTP/3"};

		constexpr std::string_view get {"GET"};
		constexpr std::string_view put {"PUT"};
		constexpr std::string_view post {"POST"};

		constexpr std::string_view line_terminator = "\r\n";

		constexpr std::string_view http_version_string(http_version v)
		{
			switch (v)
			{
				case http_version::HTTP_1:
					return http_1;
				case http_version::HTTP_1_1:
					return http_1_1;
				case http_version::HTTP_2:
					return http_2;
				case http_version::HTTP_3:
					return http_3;
				default:
					throw std::invalid_argument("http_version_string");
			}
		}

		constexpr http_version http_version_type(std::string_view s)
		{
			if (s == http_1)
			{
				return http_version::HTTP_1;
			}
			else if (s == http_1_1)
			{
				return http_version::HTTP_1_1;
			}
			else if (s == http_2)
			{
				return http_version::HTTP_2;
			}
			else if (s == http_3)
			{
				return http_version::HTTP_3;
			}

			throw std::invalid_argument("http_version_type");
		}

		constexpr std::string_view http_method_string(http_method m)
		{
			switch (m)
			{
				case http_method::GET:
					return get;
				case http_method::POST:
					return put;
				case http_method::PUT:
					return post;
				default:
					throw std::invalid_argument("http_method_string");
			}
		}

		constexpr http_method http_method_type(std::string_view s)
		{
			if (s == get)
			{
				return http_method::GET;
			}
			else if (s == post)
			{
				return http_method::POST;
			}
			else if (s == put)
			{
				return http_method::PUT;
			}

			throw std::invalid_argument("http_method_type");
		}

		enum class http_errc
		{
			malformed_response = 1
		};

		inline const std::error_category& http_category() noexcept
		{
			struct http_category_impl : std::error_category
			{
				virtual const char* name() const noexcept override
				{
					return "http";
				}

				virtual std::string message(int val) const override
				{
					switch (static_cast<http_errc>(val))
					{
						case http_errc::malformed_response:
							return "http malformed response";
					}

					return "http unknown error";
				}
			};

			static http_category_impl hc;

			return hc;
		}

		std::error_code make_error_code(http_errc e) noexcept
		{
			return std::error_code(static_cast<int>(e), http_category());
		}

		std::error_condition make_error_condition(http_errc e) noexcept
		{
			return std::error_condition(static_cast<int>(e), http_category());
		}

		template<class Owner>
		class scoped_socket
		{
			public:
				using socket_type = net::basic_socket_iostream<typename Owner::protocol_type>;

				scoped_socket(Owner& own)
					: owner_(own), s(std::move(own.socket())) {}

				scoped_socket(const scoped_socket&) = delete;
				scoped_socket& operator=(const scoped_socket&) = delete;

				net::basic_stream_socket<typename Owner::protocol_type>&& move_socket()
				{
					moved = true;
					return std::move(s.socket());
				}

				socket_type& socket()
				{
					return s;
				}

				socket_type* operator->()
				{
					return &s;
				}

				~scoped_socket()
				{
					if(!moved)
					{
						owner_.socket(std::move(s.socket()));
					}
				}

			private:
				Owner& owner_;
				socket_type s;
				bool moved {false};
		};

		class basic_http_request
		{
			public:
				using protocol_type = net::ip::tcp;
				using endpoint_type = typename protocol_type::endpoint;
				using socket_type = net::basic_stream_socket<protocol_type>;

				basic_http_request(const basic_http_request&) = default;
				basic_http_request(basic_http_request&&) = default;
				basic_http_request& operator=(const basic_http_request&) = default;
				basic_http_request& operator=(basic_http_request&&) = default;
				virtual ~basic_http_request() = default;

				basic_http_request(net::io_context& ctx = net::io_context())
					: socket_{ctx} {}

				void resource(std::string_view str)
				{
					resource_ = str;
				}

				const std::string& resource() const noexcept
				{
					return resource_;
				}

				void host(std::string_view str)
				{
					host_ = str;
					update_host_header();
				}

				const std::string& host() const noexcept
				{
					return host_;
				}

				net::ip::port_type port() const noexcept
				{
					return port_;
				}

				void port(net::ip::port_type p)
				{
					port_ = p;
					update_host_header();
				}

				void method(std::string_view str)
				{
					method_ = str;
				}

				const std::string& method() const noexcept
				{
					return method_;
				}

				void header(std::string_view name, std::string_view value)
				{
					headers_[name.data()] = value;
				}

				const std::string& header(std::string_view name) const
				{
					return headers_.at(name.data());
				}

				const std::unordered_map<std::string, std::string>& headers() const noexcept
				{
					return headers_;
				}

				template<class Rep, class Period>
				void timeout(std::chrono::duration<Rep, Period> d) noexcept
				{
					timeout_ = d;
				}

				std::chrono::milliseconds timeout() const noexcept
				{
					return timeout_;
				}

				void version(http_version v) noexcept
				{
					version_ = v;
				}

				http_version version() const noexcept
				{
					return version_;
				}

				std::string full_host() const noexcept
				{
					return host_ + ":" + std::to_string(port_);
				}

				socket_type& socket()
				{
					return socket_;
				}

				void socket(net::basic_socket<protocol_type>&& socket)
				{
					socket_.assign(endpoint_type{}.protocol(), socket.release());
				}

			private:
				std::string resource_ {"/"};
				std::string method_ {get};
				std::string host_;
				net::ip::port_type port_ {http_default_port};
				http_version version_ {http_version::HTTP_1_1};
				std::unordered_map<std::string, std::string> headers_;
				std::chrono::milliseconds timeout_ {std::chrono::milliseconds(200)};
				socket_type socket_;

				void update_host_header()
				{
					headers_[host_header.data()] = full_host();
				}
		};

		std::ostream& operator<<(std::ostream& stream, const basic_http_request& req)
		{
			stream << req.method();
			stream << " ";
			stream << req.resource();
			stream << " ";
			stream << http_version_string(req.version());
			stream << line_terminator;

			for (const auto& [name, value] : req.headers())
			{
				stream << name;
				stream << ": ";
				stream << value;
				stream << "\r\n";
			}

			stream << line_terminator;

			return stream;
		}

		class basic_http_response
		{
			public:
				using protocol_type = net::ip::tcp;
				using endpoint_type = typename protocol_type::endpoint;
				using socket_type = net::basic_stream_socket<protocol_type>;

				basic_http_response(socket_type&& sock, std::size_t response_code, std::string_view response_code_message, http_version version, std::unordered_map<std::string, std::string>&& headers)
					: socket_(std::move(sock)), response_code_(response_code), response_code_message_(response_code_message), version_(version), headers_(std::move(headers)) {}

				basic_http_response(const basic_http_response&) = default;
				basic_http_response(basic_http_response&&) noexcept = default;
				basic_http_response& operator=(const basic_http_response&) = default;
				basic_http_response& operator=(basic_http_response&&) = default;

				std::size_t response_code() const noexcept
				{
					return response_code_;
				}

				http_version version() const noexcept
				{
					return version_;
				}

				const std::unordered_map<std::string, std::string>& headers() const noexcept
				{
					return headers_;
				}

				const std::string& response_code_message() const noexcept
				{
					return response_code_message_;
				}

				socket_type& socket()
				{
					return socket_;
				}

				void socket(socket_type&& socket)
				{
					socket_ = std::move(socket);
				}

				const std::error_code& error() const
				{
					return ec_;
				}

			private:
				friend std::ostream& operator<<(std::ostream& os, basic_http_response& resp);

				std::size_t response_code_;
				std::string response_code_message_;
				http_version version_;
				std::unordered_map<std::string, std::string> headers_;
				socket_type socket_;
				std::error_code ec_;
		};

		std::ostream& operator<<(std::ostream& os, basic_http_response& resp)
		{
			resp.ec_.clear();
			if(resp.socket().is_open())
			{
				scoped_socket s(resp);
				os << s->rdbuf();
				resp.ec_ = s->error();
			}

			return os;
		}

		class http_client
		{
			public:
				using protocol_type = net::ip::tcp;
				using endpoint_type = protocol_type::endpoint;
				using socket_type = net::basic_socket_iostream<protocol_type>;

				http_client() = default;
				~http_client() noexcept = default;

				template<class HttpRequest>
				std::optional<basic_http_response> send(HttpRequest& req, std::error_code& ec)
				{
					ec.clear();
					scoped_socket s(req);
					s->connect(req.host(), std::to_string(req.port()));
					if (s->error())
					{
						ec = s->error();
						return {};
					}

					s->socket().set_option(net::socket_base::timeout(req.timeout()));

					s.socket() << req;
					s->sync();

					return parseResponse(s, ec);
				}

			private:

				/*
				 * We are attempting to parse an HTTP response message. This method is responsible for parsing the headers.
				 * The message can look like the following:
				 *
				 *	HTTP/1.1 200 OK
				 *	Date: Sun, 08 Feb xxxx 01:11:12 GMT
				 *	Server: Apache/1.3.29 (Win32)
				 *	Last-Modified: Sat, 07 Feb xxxx
				 *	ETag: "0-23-4024c3a5"
				 *	Accept-Ranges: bytes
				 *	Content-Length: 35
				 *	Connection: close
				 *	Content-Type: text/html
				 *
				 *	<h1>My Home page</h1>
				 */
				template<class HttpRequest>
				std::optional<basic_http_response> parseResponse(scoped_socket<HttpRequest>& s, std::error_code& ec)
				{
					ec.clear();
					http_version version;
					std::size_t status_code;
					std::string status_code_message;

					iterators::istream_iterator_sep space_stream {s.socket(), " "};

					if(space_stream->empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					try
					{
						version = http_version_type(*space_stream);
					}
					catch (const std::invalid_argument& e)
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					++space_stream;
					if(space_stream->empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					status_code = strings::from_string<std::size_t>(*space_stream, ec);
					if (ec)
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					iterators::istream_iterator_sep line_stream {s.socket(), "\r\n"};

					if(line_stream->empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					status_code_message = std::move(*line_stream);

					std::unordered_map<std::string, std::string> headers;

					while (true)
					{
						++line_stream;
						if(line_stream->empty())
						{
							break;
						}

						const auto colon_loc = line_stream->find(':');
						if (colon_loc == std::string::npos)
						{
							ec = make_error_code(http_errc::malformed_response);
							return {};
						}

						auto key = line_stream->substr(0, colon_loc);
						auto value = colon_loc + 1 > line_stream->size() ? "" : line_stream->substr(colon_loc + 1);
						strings::trim_front(value, " ");

						headers[std::move(key)] = std::move(value);
					}

					return std::make_optional<basic_http_response>(std::move(s.move_socket()), status_code, status_code_message, version, std::move(headers));
				}
		};

		//template<class Protocol = net::ip::tcp>
		//class http_acceptor : public net::basic_socket_acceptor<Protocol>
		//{
		//	public:
		//		using protocol_type = Protocol;
		//		using endpoint_type = typename protocol_type::endpoint;
		//		using socket_type = typename protocol_type::socket;
		//	private:
		//		using base = net::basic_socket_acceptor<protocol_type>;
		//	public:
		//		http_acceptor(net::io_context& ctx = net::io_context())
		//			: base{ctx} {}

		//		http_acceptor(net::io_context& ctx, const protocol_type& protocol)
		//			: base{ctx, protocol} {}

		//		http_acceptor(net::io_context& ctx, const endpoint_type& endpoint, bool reuse_addr = true)
		//			: base{ctx, endpoint, reuse_addr} {}

		//		http_acceptor(net::io_context& ctx, const protocol_type& protocol, const base::native_handle_type& handle)
		//			: base{ctx, protocol, handle} {}

		//		~http_acceptor() = default;
		//		
		//		http_acceptor(const http_acceptor&) = delete;
		//		http_acceptor(http_acceptor&&) = default;

		//		http_acceptor& operator=(const http_acceptor&) = delete;
		//		http_acceptor& operator=(http_acceptor&&) = delete;
		//	private:
		//		std::unordered_map<std::string, 
		//};
	}
}