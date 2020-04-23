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

#include "socket.hpp"
#include "internet.hpp"
#include "iocontext.hpp"
#include "../iostream.hpp"
#include "../string.hpp"
#include "../future.hpp"

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
			HTTP_3,
			INVALID
		};

		constexpr std::string_view http_1 {"HTTP/1.0"};
		constexpr std::string_view http_1_1 {"HTTP/1.1"};
		constexpr std::string_view http_2 {"HTTP/2"};
		constexpr std::string_view http_3 {"HTTP/3"};

		constexpr std::string_view get {"GET"};
		constexpr std::string_view put {"PUT"};
		constexpr std::string_view post {"POST"};

		constexpr std::string_view line_terminator = "\r\n";

		static constexpr std::string_view http_version_string(http_version v)
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
					return std::string_view(nullptr, 0);
			}
		}

		static constexpr http_version http_version_type(std::string_view s)
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

			return http_version::INVALID;
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

		class basic_http_request
		{
			public:
				basic_http_request() = default;
				basic_http_request(const basic_http_request&) = default;
				basic_http_request(basic_http_request&&) = default;
				basic_http_request& operator=(const basic_http_request&) = default;
				basic_http_request& operator=(basic_http_request&&) = default;
				virtual ~basic_http_request() = default;

				basic_http_request(std::string_view r)
					: resource_(r) {}

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

			private:
				std::string resource_ {"/"};
				std::string method_ {get};
				std::string host_;
				net::ip::port_type port_ {http_default_port};
				http_version version_ {http_version::HTTP_1_1};
				std::unordered_map<std::string, std::string> headers_;
				std::chrono::milliseconds timeout_ {std::chrono::milliseconds(100)};

				void update_host_header()
				{
					headers_[host_header.data()] = full_host();
				}
		};

		template<class BodyContent, typename = std::enable_if_t<
/* requires */	!std::is_void_v<BodyContent>
		>>
		class http_request_body : public basic_http_request
		{
			public:
				using body_type = BodyContent;

				http_request_body() = default;
				http_request_body(const http_request_body&) = default;
				http_request_body(http_request_body&&) = default;
				http_request_body& operator=(const http_request_body&) = default;
				http_request_body& operator=(http_request_body&&) = default;

				void body(const body_type& c) noexcept
				{
					body_ = &c;
				}

				const body_type* body() const noexcept
				{
					return body_;
				}

			private:
				body_type* body_ {nullptr};
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

		template<class T>
		std::ostream& operator<<(std::ostream& stream, const http_request_body<T>& req)
		{
			stream << static_cast<const basic_http_request&>(req);

			if (req.body() != nullptr)
			{
				stream << *(req.body());
			}

			return stream;
		}

		class basic_http_response
		{
			public:
				basic_http_response(std::size_t response_code, std::string_view response_code_message, http_version version, std::unordered_map<std::string, std::string>&& headers)
					: response_code_(response_code), response_code_message_(response_code_message), version_(version), headers_(std::move(headers)) {}

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

			private:
				std::size_t response_code_;
				std::string response_code_message_;
				http_version version_;
				std::unordered_map<std::string, std::string> headers_;
		};

		class http_client
		{
			public:
				using protocol = net::ip::tcp;
				using endpoint = protocol::endpoint;
				using socket = net::basic_socket_iostream<net::ip::tcp>;

				http_client(net::io_context& ctx) noexcept
					: ctx_(ctx) {}
				~http_client() noexcept {}

				template<class HttpRequest>
				std::optional<basic_http_response> send(const HttpRequest& req, std::error_code& ec)
				{
					auto s = get_socket(req, ec);
					if (ec)
					{
						return {};
					}

					s << req;
					s.sync();

					return parseResponse(s, req, ec);
				}

				template<class HttpRequest>
				std::optional<basic_http_response> send(const HttpRequest& req, std::string& body, std::error_code& ec)
				{
					auto s = get_socket(req, ec);
					if (ec)
					{
						return {};
					}

					s << req;
					s.sync();

					auto response = parseResponse(s, req, ec);
					if(!response)
					{
						return {};
					}

					parseBody(s, response.value(), body);
					return response;
				}

				template<class HttpRequest, class ResponseBody>
				std::future<std::optional<basic_http_response>> sendAsync(const HttpRequest& req, ResponseBody& buf, std::error_code& ec, std::launch policy = std::launch::async)
				{
					return std::async(policy, &http_client::send<HttpRequest, ResponseBody>, this, std::ref(req), std::ref(buf), std::ref(ec));
				}

				template<class HttpRequest>
				std::future<std::optional<basic_http_response>> sendAsync(const HttpRequest& req, std::error_code& ec, std::launch policy = std::launch::async)
				{
					return std::async(policy, &http_client::send<HttpRequest>, this, std::cref(req), std::ref(ec));
				}

			private:
				net::io_context& ctx_;

				template<class HttpRequest>
				socket get_socket(const HttpRequest& req, std::error_code& ec)
				{
					socket s(ctx_);
					ec.clear();
					s.socket().set_option(net::socket_base::timeout(req.timeout()));
					s.connect(req.host(), std::to_string(req.port()));
					if (s.error())
					{
						ec = s.error();
					}

					return s;
				}

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
				std::optional<basic_http_response> parseResponse(socket& s, const HttpRequest& req, std::error_code& ec)
				{
					ec.clear();
					std::string line;
					std::getline(s, line, ' ');
					if (line.empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					const auto version = http_version_type(line);
					line.clear();
					
					std::getline(s, line, ' ');
					if (line.empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					const auto status_code = strings::from_string<std::size_t>(line, ec);
					line.clear();
					if (ec)
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					iostream::getline(s, line, line_terminator);
					if (line.empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					const auto status_code_message = std::move(line);

					std::unordered_map<std::string, std::string> headers;

					while (true)
					{
						line.clear();
						iostream::getline(s, line, line_terminator);
						if (line.empty())
						{
							break;
						}

						const auto colon_loc = line.find(':');
						if (colon_loc == std::string::npos)
						{
							ec = make_error_code(http_errc::malformed_response);
							return {};
						}

						auto key = line.substr(0, colon_loc);
						auto value = colon_loc + 1 > line.size() ? "" : line.substr(colon_loc + 1);
						strings::trim_front(value, " ");

						headers[std::move(key)] = std::move(value);
					}

					return std::make_optional<basic_http_response>(status_code, status_code_message, version, std::move(headers));
				}

				/*
				 * We are attempting to parse an HTTP response message. This method is responsible for parsing the body.
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
				void parseBody(socket& s, const basic_http_response& resp, std::string& body)
				{
					const auto content_length_str = resp.headers().find(content_length_header.data());
					if (content_length_str != std::end(resp.headers()))
					{
						std::error_code ec;
						const auto content_length = strings::from_string<std::size_t>(content_length_str->second, ec);
						if(!ec)
						{
							body.resize(content_length);
							s.get(body.data(), content_length);
							return;
						}
					}

					body = std::string((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());
				}
		};
	}
}