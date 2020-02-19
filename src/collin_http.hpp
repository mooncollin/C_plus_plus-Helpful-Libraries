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

#include "collin_socket.hpp"

using namespace std::literals;

namespace collin
{
	namespace http
	{
		constexpr auto default_port = "80";

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
				using content_type = typename BodyContent::value_type;

				HttpRequest() = default;

				HttpRequest(std::string_view url)
					: url(url.data()) {}

				void url(std::string_view str)
				{
					_url = str.data();
				}

				const std::string& url() const
				{
					return _url;
				}

				void method(std::string_view str)
				{
					_method = str.data();
				}

				const std::string& method() const
				{
					return _method;
				}

				void contentProvider(const BodyContent& c)
				{
					_contentProvider = &c;
				}

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
					return _headers.at(name);
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
					msg.reserve(std::size(_url) + std::size(_method) + std::size(http_version));

					msg += _method;
					msg += " ";
					msg += _url;
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
				std::string _url = "/";
				std::string _method = get.data();
				HttpVersion _version = HttpVersion::HTTP_1_1;
				BodyContent* _contentProvider = nullptr;
				std::unordered_map<std::string, std::string> _headers;
				std::chrono::milliseconds _timeout = 1000ms;
		};

		template<class BodyContent>
		struct HttpResponse
		{
			public:
				HttpResponse(std::size_t _response_code, HttpVersion _version, std::unordered_map<std::string, std::string>&& _headers, BodyContent&& _body={})
					: _response_code(_response_code), _version(_version), _headers(_headers), _body(_body) {}

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

				const BodyContent& body() const noexcept
				{
					return _body;
				}

			private:
				std::size_t _response_code;
				HttpVersion _version;
				std::unordered_map<std::string, std::string> _headers;
				BodyContent _body;
		};

		template<>
		struct HttpResponse<void>
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

				template<class ResponseBody, class RequestBody>
				std::optional<HttpResponse<ResponseBody>> send(const HttpRequest<RequestBody>& req)
				{
					auto address = gather_address_information(req);
					if (!address)
					{
						return {};
					}

					Socket s;
					s.setOption(SO_RCVTIMEO, req.timeout().count());
					if (s.connect(address.value()) == SOCKET_ERROR)
					{
						return {};
					}

					s << req.message();
					if (!s)
					{
						return {};
					}

					return parseResponse<ResponseBody>(s);
				}

				template<class ResponseBody, class RequestBody>
				std::future<std::optional<HttpResponse<ResponseBody>>> sendAsync(const HttpRequest<RequestBody>& req, std::launch policy=std::launch::async)
				{
					return std::async(policy, &HttpClient::send<ResponseBody, RequestBody>, this, std::ref(req));
				}

			private:
				inline static constexpr auto host_header = "Host";

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

				template<class ResponseBody>
				std::optional<HttpResponse<ResponseBody>> parseResponse(Socket& s)
				{
					std::array<char, 4096> buf;
					buf.fill('\0');
					s >> buf;

					const auto fill_until = [&s, &buf](std::string_view ch, std::size_t offset)
					{
						std::string result;
						auto current_location = std::strstr(buf.data() + offset, ch.data());
						while (current_location == nullptr)
						{
							result += buf.data();
							buf.fill('\0');
							s >> buf;
							current_location = std::strstr(buf.data(), ch.data());
						}

						std::copy_n(std::begin(buf) + offset, current_location - (buf.data() + offset), std::back_inserter(result));
						return std::make_pair(std::move(result), current_location - buf.data());
					};

					auto fill_result = fill_until(" ", 0);
					const auto version_string = std::move(fill_result.first);
					const auto version = http_version(version_string);
					auto offset = fill_result.second;

					fill_result = fill_until(" ", offset + 1);
					const auto status_string = std::move(fill_result.first);
					const auto status_code = std::atoll(status_string.data());
					offset = fill_result.second;

					auto previous_fill_result = fill_until(line_terminator, offset + 1);
					auto next_fill_result = fill_until(line_terminator, previous_fill_result.second + line_terminator.length());

					std::unordered_map<std::string, std::string> headers;
					while(previous_fill_result.second != next_fill_result.second)
					{
						const auto colon_loc = next_fill_result.first.find(": ");
						if (colon_loc == std::string::npos)
						{
							break;
						}
						headers[next_fill_result.first.substr(0, colon_loc)] = next_fill_result.first.substr(colon_loc + 2);
						previous_fill_result = std::move(next_fill_result);
						next_fill_result = fill_until(line_terminator, previous_fill_result.second + line_terminator.length());
					}

					if constexpr (std::is_same_v<ResponseBody, void>)
					{
						return HttpResponse<ResponseBody>(status_code, version, std::move(headers));
					}
					else
					{
						std::string body = buf.data() + next_fill_result.second + line_terminator.length();
						while (s && buf.at(0) != '\0')
						{
							buf.fill('\0');
							s >> buf;
							body += buf.data();
						}
						return HttpResponse<ResponseBody>(status_code, version, std::move(headers), std::move(body));
					}
				}
		};
	}
}

#endif