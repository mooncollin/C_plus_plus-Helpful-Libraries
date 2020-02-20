#ifndef COLLIN_HTTP
#define COLLIN_HTTP

#include <string>
#include <unordered_map>
#include <chrono>
#include <stdexcept>
#include <optional>
#include <utility>
#include <cstring>
#include <future>
#include <type_traits>

#include "collin_socket.hpp"

using namespace std::literals;

namespace collin
{
	namespace http
	{
		inline constexpr auto default_port = "80";
		inline constexpr auto host_header = "Host";

		enum class HttpVersion
		{
			HTTP_1,
			HTTP_1_1,
			HTTP_2,
			HTTP_3
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

			throw std::invalid_argument(s.data() + std::string(" is not a valid http version string"));
		}

		template<class BodyContent>
		class HttpRequest
		{
			public:
				HttpRequest() = default;

				HttpRequest(std::string_view r)
					: _resource(r.data()) {}

				void resource(std::string_view str)
				{
					_resource = str.data();
				}

				const std::string& resource() const
				{
					return _resource;
				}

				void method(std::string_view str)
				{
					_method = str.data();
				}

				const std::string& method() const
				{
					return _method;
				}

				void contentProvider(BodyContent& c) noexcept
				{
					_contentProvider = &c;
				}

				void contentProvider(BodyContent&& c) = delete;

				BodyContent* contentProvider() const noexcept
				{
					return _contentProvider;
				}

				void header(std::string_view name, std::string_view value)
				{
					_headers[name.data()] = value.data();
				}

				const std::string& header(std::string_view name) const
				{
					return _headers.at(name.data());
				}

				const std::unordered_map<std::string, std::string>& headers() const noexcept
				{
					return _headers;
				}

				template<class Rep, class Period>
				void timeout(std::chrono::duration<Rep, Period> d) noexcept
				{
					_timeout = d;
				}

				std::chrono::milliseconds timeout() const
				{
					return _timeout;
				}

				void version(HttpVersion v)
				{
					_version = v;
				}

				HttpVersion version() const
				{
					return _version;
				}

				std::string message() const
				{
				
					std::string msg;
					const auto http_version = http_version_string(_version);
					msg.reserve(std::size(_resource) + std::size(_method) + std::size(http_version));

					msg += _method;
					msg += " ";
					msg += _resource;
					msg += " ";
					msg += http_version;
					msg += line_terminator;

					for (const auto& [name, value] : _headers)
					{
						msg += name;
						msg += ": ";
						msg += value;
						msg += "\r\n";
					}
				
					msg += line_terminator;

					if (_contentProvider != nullptr)
					{
						std::copy(std::begin(*_contentProvider), std::end(*_contentProvider), std::back_inserter(msg));
					}

					return msg;
				}
			
			private:
				std::string _resource = "/";
				std::string _method = get.data();
				HttpVersion _version = HttpVersion::HTTP_1_1;
				BodyContent* _contentProvider = nullptr;
				std::unordered_map<std::string, std::string> _headers;
				std::chrono::milliseconds _timeout = 1000ms;
		};

		template<>
		class HttpRequest<void>
		{
			public:
				HttpRequest() = default;

				HttpRequest(std::string_view r)
					: _resource(r.data()) {}

				void resource(std::string_view str)
				{
					_resource = str.data();
				}

				const std::string& resource() const
				{
					return _resource;
				}

				void method(std::string_view str)
				{
					_method = str.data();
				}

				const std::string& method() const
				{
					return _method;
				}

				void header(std::string_view name, std::string_view value)
				{
					_headers[name.data()] = value.data();
				}

				const std::string& header(std::string_view name) const
				{
					return _headers.at(name.data());
				}

				const std::unordered_map<std::string, std::string>& headers() const noexcept
				{
					return _headers;
				}

				template<class Rep, class Period>
				void timeout(std::chrono::duration<Rep, Period> d) noexcept
				{
					_timeout = d;
				}

				std::chrono::milliseconds timeout() const
				{
					return _timeout;
				}

				void version(HttpVersion v)
				{
					_version = v;
				}

				HttpVersion version() const
				{
					return _version;
				}

				std::string message() const
				{

					std::string msg;
					const auto http_version = http_version_string(_version);
					msg.reserve(std::size(_resource) + std::size(_method) + std::size(http_version));

					msg += _method;
					msg += " ";
					msg += _resource;
					msg += " ";
					msg += http_version;
					msg += line_terminator;

					for (const auto& [name, value] : _headers)
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
				std::string _resource = "/";
				std::string _method = get.data();
				HttpVersion _version = HttpVersion::HTTP_1_1;
				std::unordered_map<std::string, std::string> _headers;
				std::chrono::milliseconds _timeout = 1000ms;
		};

		class HttpResponse
		{
			public:
				HttpResponse(std::size_t _response_code, HttpVersion _version, std::unordered_map<std::string, std::string>&& _headers)
					: _response_code(_response_code), _version(_version), _headers(_headers) {}

				std::size_t response_code() const noexcept
				{
					return _response_code;
				}

				HttpVersion version() const noexcept
				{
					return _version;
				}

				const std::unordered_map<std::string, std::string>& headers() const noexcept
				{
					return _headers;
				}

			private:
				std::size_t _response_code;
				HttpVersion _version;
				std::unordered_map<std::string, std::string> _headers;
		};

		template<std::size_t Buffer_Size = 8192>
		class HttpClient
		{
			public:

				HttpClient() noexcept
				{
					Socket::init();
				}

				~HttpClient() noexcept
				{
					Socket::cleanup();
				}

				template<class RequestBody>
				std::optional<HttpResponse> send(const HttpRequest<RequestBody>& req)
				{
					Socket s;
					if (!sendHelper(s, req))
					{
						return {};
					}
					std::array<char, Buffer_Size> buf;
					auto offset = std::begin(buf);

					return parseResponse(s, buf, offset);
				}

				template<class RequestBody, class ResponseBody>
				std::optional<HttpResponse> send(const HttpRequest<RequestBody>& req, ResponseBody& body)
				{
					Socket s;
					if (!sendHelper(s, req))
					{
						return {};
					}

					std::array<char, Buffer_Size> buf;
					auto offset = std::begin(buf);

					auto response = parseResponse(s, buf, offset);
					if (!response)
					{
						return {};
					}

					parseBody(s, body, buf, offset);
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
				template<class RequestBody>
				bool sendHelper(Socket& s, const HttpRequest<RequestBody>& req)
				{
					auto address = gather_address_information(req);
					if (!address)
					{
						return false;
					}

					s.setOption(SO_RCVTIMEO, req.timeout().count());
					if (s.connect(address.value()) == SOCKET_ERROR)
					{
						return false;
					}

					s << req.message();
					if (!s)
					{
						return false;
					}

					return true;
				}

				template<class ArrayType, std::size_t N, class ForwardIterator>
				std::optional<HttpResponse> parseResponse(Socket& s, std::array<ArrayType, N>& buf, ForwardIterator& out_offset)
				{
					buf.fill('\0');
					s >> buf;
					if (s.lastRead() <= 0)
					{
						return {};
					}

					const auto fill_until = [&s, &buf](std::string_view ch, auto& offset)
					{
						std::string result;
						auto current_location = std::search(offset, std::begin(buf) + s.lastRead(), std::begin(ch), std::end(ch));
						while (current_location == std::end(buf))
						{
							result += buf.data();
							s >> buf;
							offset = std::begin(buf);
							current_location = std::search(std::begin(buf), std::begin(buf) + s.lastRead(), std::begin(ch), std::end(ch));
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
				void parseBody(Socket& s, ResponseBody& body, std::array<char, N>& buf, ForwardIterator starting_offset)
				{
					if(s)
					{
						std::copy(starting_offset, std::begin(buf) + s.lastRead(), std::back_inserter(body));
						while (s && s.lastRead() > 0)
						{
							s >> buf;
							std::copy(std::begin(buf), std::begin(buf) + s.lastRead(), std::back_inserter(body));
						}
					}
				}

				template<class RequestBody>
				std::optional<sockaddr_in> gather_address_information(const HttpRequest<RequestBody>& req)
				{
					const auto headers = req.headers();
					const auto location = headers.find(host_header);
					if (location == std::end(headers))
					{
						return {};
					}

					auto host = headers.at(location->first);
					const auto colon_loc = host.find(':');
					auto port = default_port;
					if (colon_loc != std::string::npos)
					{
						port = host.data() + colon_loc + 1;
						host.erase(std::begin(host) + colon_loc, std::end(host));
					}

					return resolveHostName(host, port);
				}
		};
	}
}

#endif