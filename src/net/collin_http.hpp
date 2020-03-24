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

#include "collin_socket.hpp"
#include "collin_internet.hpp"
#include "collin_iocontext.hpp"

namespace collin
{
	namespace http
	{
		using namespace std::literals;
		inline constexpr net::ip::port_type default_port = 80;
		inline constexpr auto host_header = "Host";

		enum class HttpVersion
		{
			HTTP_1,
			HTTP_1_1,
			HTTP_2,
			HTTP_3,
			INVALID
		};

		constexpr std::string_view http_1 = "HTTP/1.0";
		constexpr std::string_view http_1_1 = "HTTP/1.1";
		constexpr std::string_view http_2 = "HTTP/2";
		constexpr std::string_view http_3 = "HTTP/3";

		constexpr std::string_view get = "GET";
		constexpr std::string_view put = "PUT";
		constexpr std::string_view post = "POST";

		constexpr std::string_view line_terminator = "\r\n";

		static constexpr std::string_view http_version_string(HttpVersion v)
		{
			switch (v)
			{
				case HttpVersion::HTTP_1:
					return http_1;
				case HttpVersion::HTTP_1_1:
					return http_1_1;
				case HttpVersion::HTTP_2:
					return http_2;
				case HttpVersion::HTTP_3:
					return http_3;
				default:
					return std::string_view(nullptr, 0);
			}
		}

		static constexpr HttpVersion http_version(std::string_view s)
		{
			if (s == http_1)
			{
				return HttpVersion::HTTP_1;
			}
			else if (s == http_1_1)
			{
				return HttpVersion::HTTP_1_1;
			}
			else if (s == http_2)
			{
				return HttpVersion::HTTP_2;
			}
			else if (s == http_3)
			{
				return HttpVersion::HTTP_3;
			}

			return HttpVersion::INVALID;
		}

		template<class BodyContent>
		class HttpRequest
		{
			public:
				using protocol = net::ip::tcp;
				using endpoint_t = protocol::endpoint;

				HttpRequest() = default;

				HttpRequest(std::string_view r)
					: resource_(r.data()) {}

				void endpoint(const endpoint_t& ep)
				{
					endpoint_ = ep;
					std::ostringstream ss;
					ss << ep;
					header(collin::http::host_header, ss.str());
				}

				std::size_t host(net::io_context& ctx, std::string_view str, net::ip::port_type port = default_port)
				{
					net::ip::basic_resolver<protocol> r(ctx);
					std::string service = std::to_string(port);
					const auto results = r.resolve(str, service);

					for(const auto& result : results)
					{
						auto ep = result.endpoint();
						ep.port(port);
						endpoint(ep);
						break;
					}

					return results.size();
				}

				endpoint_t endpoint() const
				{
					return endpoint_;
				}

				void resource(std::string_view str)
				{
					resource_ = str.data();
				}

				const std::string& resource() const
				{
					return resource_;
				}

				void method(std::string_view str)
				{
					method_ = str.data();
				}

				const std::string& method() const
				{
					return method_;
				}

				void contentProvider(BodyContent& c) noexcept
				{
					contentProvider_ = &c;
				}

				void contentProvider(BodyContent&& c) = delete;

				BodyContent* contentProvider() const noexcept
				{
					return contentProvider_;
				}

				void header(std::string_view name, std::string_view value)
				{
					headers_[name.data()] = value.data();
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

				std::chrono::milliseconds timeout() const
				{
					return timeout_;
				}

				void version(HttpVersion v)
				{
					version_ = v;
				}

				HttpVersion version() const
				{
					return version_;
				}

				std::string message() const
				{
				
					std::string msg;
					const auto httpversion_ = http_version_string(version_);
					msg.reserve(std::size(resource_) + std::size(method_) + std::size(httpversion_));

					msg += method_;
					msg += " ";
					msg += resource_;
					msg += " ";
					msg += httpversion_;
					msg += line_terminator;

					for (const auto& [name, value] : headers_)
					{
						msg += name;
						msg += ": ";
						msg += value;
						msg += "\r\n";
					}
				
					msg += line_terminator;

					if (contentProvider_ != nullptr)
					{
						std::copy(std::begin(*contentProvider_), std::end(*contentProvider_), std::back_inserter(msg));
					}

					return msg;
				}
			
			private:
				std::string resource_ = "/";
				std::string method_ = get.data();
				endpoint_t endpoint_;
				HttpVersion version_ = HttpVersion::HTTP_1_1;
				BodyContent* contentProvider_ = nullptr;
				std::unordered_map<std::string, std::string> headers_;
				std::chrono::milliseconds timeout_ = 1000ms;
		};

		template<>
		class HttpRequest<void>
		{
			public:
				HttpRequest() = default;

				HttpRequest(std::string_view r)
					: resource_(r.data()) {}

				void resource(std::string_view str)
				{
					resource_ = str.data();
				}

				const std::string& resource() const
				{
					return resource_;
				}

				void method(std::string_view str)
				{
					method_ = str.data();
				}

				const std::string& method() const
				{
					return method_;
				}

				void header(std::string_view name, std::string_view value)
				{
					headers_[name.data()] = value.data();
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

				std::chrono::milliseconds timeout() const
				{
					return timeout_;
				}

				void version(HttpVersion v)
				{
					version_ = v;
				}

				HttpVersion version() const
				{
					return version_;
				}

				std::string message() const
				{

					std::string msg;
					const auto httpversion_ = http_version_string(version_);
					msg.reserve(std::size(resource_) + std::size(method_) + std::size(httpversion_));

					msg += method_;
					msg += " ";
					msg += resource_;
					msg += " ";
					msg += httpversion_;
					msg += line_terminator;

					for (const auto& [name, value] : headers_)
					{
						msg += name;
						msg += ": ";
						msg += value;
						msg += "\r\n";
					}

					msg += line_terminator;

					return msg;
				}

			private:
				std::string resource_ = "/";
				std::string method_ = get.data();
				HttpVersion version_ = HttpVersion::HTTP_1_1;
				std::unordered_map<std::string, std::string> headers_;
				std::chrono::milliseconds timeout_ = 1000ms;
		};

		class HttpResponse
		{
			public:
				HttpResponse(std::size_t _response_code, HttpVersion version_, std::unordered_map<std::string, std::string>&& headers_)
					: _response_code(_response_code), version_(version_), headers_(headers_) {}

				std::size_t response_code() const noexcept
				{
					return _response_code;
				}

				HttpVersion version() const noexcept
				{
					return version_;
				}

				const std::unordered_map<std::string, std::string>& headers() const noexcept
				{
					return headers_;
				}

			private:
				std::size_t _response_code;
				HttpVersion version_;
				std::unordered_map<std::string, std::string> headers_;
		};

		template<std::size_t Buffer_Size = 8192>
		class HttpClient
		{
			public:
				using protocol = net::ip::tcp;
				using endpoint = protocol::endpoint;
				using socket = net::basic_stream_socket<net::ip::tcp>;

				HttpClient(net::io_context& ctx) noexcept
					: ctx_(ctx) {}
				~HttpClient() noexcept {}

				template<class RequestBody>
				std::optional<HttpResponse> send(const HttpRequest<RequestBody>& req, std::error_code& ec)
				{
					net::basic_stream_socket<net::ip::tcp> s;
					sendHelper(s, req, ec);
					if (ec)
					{
						return {};
					}
					std::array<char, Buffer_Size> buf;
					auto offset = std::begin(buf);

					return parseResponse(s, buf, offset);
				}

				template<class RequestBody, class ResponseBody>
				std::optional<HttpResponse> send(const HttpRequest<RequestBody>& req, ResponseBody& body, std::error_code& ec)
				{
					socket s(ctx_);
					sendHelper(s, req, ec);
					if (ec)
					{
						return {};
					}

					std::array<char, Buffer_Size> buf;
					auto offset = std::begin(buf);
					std::size_t last_read {0};

					auto response = parseResponse(s, last_read, buf, offset);
					if (!response)
					{
						return {};
					}

					parseBody(s, last_read, body, buf, offset);
					return response;
				}

				template<class RequestBody, class ResponseBody>
				std::future<std::optional<HttpResponse>> sendAsync(const HttpRequest<RequestBody>& req, RequestBody& buf, std::launch policy=std::launch::async)
				{
					return std::async(policy, &HttpClient::send<RequestBody, ResponseBody>, this, std::ref(req), std::ref(buf));
				}

				template<class RequestBody>
				std::future<std::optional<HttpResponse>> sendAsync(const HttpRequest<RequestBody>& req, std::launch policy = std::launch::async)
				{
					return std::async(policy, &HttpClient::send<RequestBody>, this, std::ref(req));
				}

			private:
				net::io_context& ctx_;

				template<class RequestBody>
				void sendHelper(socket& s, const HttpRequest<RequestBody>& req, std::error_code& ec)
				{
					s.connect(req.endpoint(), ec);
					s.set_option(net::socket_base::timeout(req.timeout()));
					if (ec)
					{
						return;
					}
					s.send(req.message());
				}

				template<class ArrayType, std::size_t N, class ForwardIterator>
				std::optional<HttpResponse> parseResponse(socket& s, std::size_t& last_read, std::array<ArrayType, N>& buf, ForwardIterator& out_offset)
				{
					buf.fill('\0');
					last_read = s.receive(buf);
					if (last_read <= 0)
					{
						return {};
					}

					const auto fill_until = [&s, &buf, &last_read](std::string_view ch, auto& offset)
					{
						std::string result;
						auto current_location = std::search(offset, std::begin(buf) + last_read, std::begin(ch), std::end(ch));
						while (current_location == std::end(buf))
						{
							result += buf.data();
							last_read = s.receive(buf);
							offset = std::begin(buf);
							current_location = std::search(std::begin(buf), std::begin(buf) + last_read, std::begin(ch), std::end(ch));
						}

						std::copy(offset, current_location, std::back_inserter(result));
						offset = current_location;
						return std::move(result);
					};

					out_offset = std::begin(buf);
					const auto version = http_version(fill_until(" ", out_offset));
					out_offset += 1;

					const auto status_code = std::stoull(fill_until(" ", out_offset));
					out_offset += 1;

					fill_until(line_terminator, out_offset);
					out_offset += line_terminator.length();

					auto next_fill_result = fill_until(line_terminator, out_offset);

					std::unordered_map<std::string, std::string> headers;
					while (true)
					{
						const auto colon_loc = next_fill_result.find(": ");
						if (colon_loc == std::string::npos)
						{
							break;
						}
						headers[next_fill_result.substr(0, colon_loc)] = next_fill_result.substr(colon_loc + 2);
						out_offset += line_terminator.length();
						next_fill_result = fill_until(line_terminator, out_offset);
					}

					out_offset += line_terminator.length();
					return HttpResponse(status_code, version, std::move(headers));
				}

				template<class ResponseBody, std::size_t N, class ForwardIterator>
				void parseBody(socket& s, std::size_t last_read, ResponseBody& body, std::array<char, N>& buf, ForwardIterator starting_offset)
				{
					if(s)
					{
						std::copy(starting_offset, std::begin(buf) + last_read, std::back_inserter(body));
						while (s.is_open() && last_read > 0)
						{
							last_read = s.receive(buf);
							std::copy(std::begin(buf), std::begin(buf) + last_read, std::back_inserter(body));
						}
					}
				}
		};
	}
}