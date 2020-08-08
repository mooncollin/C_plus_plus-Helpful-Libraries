#pragma once

#include <string>
#include <map>
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
#include <iterator>

#include "collin/net/socket.hpp"
#include "collin/net/internet.hpp"
#include "collin/net/iocontext.hpp"
#include "collin/net/url.hpp"
#include "collin/iostream.hpp"
#include "collin/iterator.hpp"
#include "collin/string.hpp"

namespace collin
{
	namespace http
	{
		constexpr net::ip::port_type default_port {80};

		namespace headers
		{
			constexpr std::string_view www_authenticate{"WWW-Authenticate"};
			constexpr std::string_view authorization{"Authorization"};
			constexpr std::string_view proxy_authenticate{"Proxy-Authenticate"};
			constexpr std::string_view proxy_authorization{"Proxy-Authorization"};
			constexpr std::string_view age{"Age"};
			constexpr std::string_view cache_control{"Cache-Control"};
			constexpr std::string_view clear_site_data{"Clear-Site-Data"};
			constexpr std::string_view expires{"Expires"};
			constexpr std::string_view pragma{"Pragma"};
			constexpr std::string_view warning{"Warning"};
			constexpr std::string_view accept_ch{"Accept-CH"};
			constexpr std::string_view accept_ch_lifetime{"Accept-CH-Lifetime"};
			constexpr std::string_view early_data{"Early-Data"};
			constexpr std::string_view dpr{"DPR"};
			constexpr std::string_view device_memory{"Device-Memory"};
			constexpr std::string_view save_data{"Save-Data"};
			constexpr std::string_view last_modified{"Last-Modified"};
			constexpr std::string_view etag{"ETag"};
			constexpr std::string_view if_match{"If-Match"};
			constexpr std::string_view if_none_match{"If-None-Match"};
			constexpr std::string_view if_modified_since{"If-Modified-Since"};
			constexpr std::string_view if_unmodified_since{"If-Unmodified-Since"};
			constexpr std::string_view vary{"Vary"};
			constexpr std::string_view connection{"Connection"};
			constexpr std::string_view keep_alive{"Keep-Alive"};
			constexpr std::string_view accept{"Accept"};
			constexpr std::string_view accept_charset{"Accept-Charset"};
			constexpr std::string_view accept_encoding{"Accept-Encoding"};
			constexpr std::string_view accept_language{"Accept-Language"};
			constexpr std::string_view expect{"Expect"};
			constexpr std::string_view cookie{"Cookie"};
			constexpr std::string_view set_cookie{"Set-Cookie"};
			constexpr std::string_view access_control_allow_origin{"Access-Control-Allow-Origin"};
			constexpr std::string_view access_control_allow_credentials{"Access-Control-Allow-Credentials"};
			constexpr std::string_view access_control_allow_headers{"Access-Control-Allow-Headers"};
			constexpr std::string_view access_control_allow_methods{"Access-Control-Allow-Methods"};
			constexpr std::string_view access_control_expose_headers{"Access-Control-Expose-Headers"};
			constexpr std::string_view access_control_max_age{"Access-Control-Max-Age"};
			constexpr std::string_view access_control_request_headers{"Access-Control-Request-Headers"};
			constexpr std::string_view access_control_request_method{"Access-Control-Request-Method"};
			constexpr std::string_view origin{"Origin"};
			constexpr std::string_view timing_allow_origin{"Timing-Allow-Origin"};
			constexpr std::string_view dnt{"DNT"};
			constexpr std::string_view tk{"Tk"};
			constexpr std::string_view content_disposition{"Content-Disposition"};
			constexpr std::string_view content_length{"Content-Length"};
			constexpr std::string_view content_type{"Content-Type"};
			constexpr std::string_view content_encoding{"Content-Encoding"};
			constexpr std::string_view content_language{"Content-Language"};
			constexpr std::string_view content_location{"Content-Location"};
			constexpr std::string_view forwarded{"Forwarded"};
			constexpr std::string_view via{"Via"};
			constexpr std::string_view location{"Location"};
			constexpr std::string_view from{"From"};
			constexpr std::string_view host{"Host"};
			constexpr std::string_view referer{"referer"};
			constexpr std::string_view referer_policy{"Referer-Policy"};
			constexpr std::string_view user_agent{"User-Agent"};
			constexpr std::string_view allow{"Allow"};
			constexpr std::string_view server{"Server"};
			constexpr std::string_view accept_ranges{"Accept-Ranges"};
			constexpr std::string_view range{"Range"};
			constexpr std::string_view if_range{"If-Range"};
			constexpr std::string_view content_range{"Content-Range"};
			constexpr std::string_view cross_origin_embedder_policy{"Cross-Origin-Embedder-Policy"};
			constexpr std::string_view cross_origin_opener_policy{"Cross-Origin-Opener-Policy"};
			constexpr std::string_view cross_origin_resource_policy{"Cross-Origin-Resource-Policy"};
			constexpr std::string_view content_security_policy{"Content-Security-Policy"};
			constexpr std::string_view content_security_policy_report_only{"Content-Security-Policy-Report-Only"};
			constexpr std::string_view expect_ct{"Expect-CT"};
			constexpr std::string_view feature_policy{"Feature-Policy"};
			constexpr std::string_view strict_transport_security{"Strict-Transport-Security"};
			constexpr std::string_view upgrade_insecure_requests{"Upgrade-Insecure-Requests"};
			constexpr std::string_view x_content_type_options{"X-Content-Type-Options"};
			constexpr std::string_view x_download_options{"X-Download-Options"};
			constexpr std::string_view x_frame_options{"X-Frame-Options"};
			constexpr std::string_view x_permitted_cross_domain_policies{"X-Permitted-Cross-Domain-Policies"};
			constexpr std::string_view x_powered_by{"X-Powered-By"};
			constexpr std::string_view x_xss_protection{"X-XSS-Protection"};
			constexpr std::string_view sec_fetch_site{"Sec-Fetch-Site"};
			constexpr std::string_view sec_fetch_mode{"Sec-Fetch-Mode"};
			constexpr std::string_view sec_fetch_user{"Sec-Fetch-User"};
			constexpr std::string_view sec_fetch_dest{"Sec-Fetch-Dest"};
			constexpr std::string_view transfer_encoding{"Transfer-Encoding"};
			constexpr std::string_view te{"TE"};
			constexpr std::string_view trailer{"Trailer"};
			constexpr std::string_view sec_websocket_key{"Sec-WebSocket-Key"};
			constexpr std::string_view sec_websocket_extensions{"Sec-WebSocket-Extensions"};
			constexpr std::string_view sec_websocket_accept{"Sec-WebSocket-Accept"};
			constexpr std::string_view sec_websocket_protocol{"Sec-WebSocket-Protocol"};
			constexpr std::string_view sec_websocket_version{"Sec-WebSocket-Version"};
			constexpr std::string_view alt_svc{"Alt-Svc"};
			constexpr std::string_view date{"Date"};
			constexpr std::string_view large_allocation{"Large-Allocation"};
			constexpr std::string_view link{"Link"};
			constexpr std::string_view retry_after{"Retry-After"};
			constexpr std::string_view server_timing{"Server-Timing"};
			constexpr std::string_view sourcemap{"SourceMap"};
			constexpr std::string_view x_dns_prefetch_control{"X-DNS-Prefetch-Control"};
		}

		namespace status
		{
			constexpr std::string_view continue_message{"Continue"};
			constexpr std::string_view switching_protocols_message{"Switching Protocols"};
			constexpr std::string_view processing_message{"Processing"};
			constexpr std::string_view early_hints_message{"Early Hints"};
			constexpr std::string_view ok_message{"OK"};
			constexpr std::string_view created_message{"Created"};
			constexpr std::string_view accepted_message{"Accepted"};
			constexpr std::string_view non_authoritative_information_message{"Non-Authoritative Information"};
			constexpr std::string_view no_content_message{"No Content"};
			constexpr std::string_view reset_content_message{"Reset Content"};
			constexpr std::string_view partial_content_message{"Partial Content"};
			constexpr std::string_view multi_status_message{"Multi-Status"};
			constexpr std::string_view already_reported_message{"Already Reported"};
			constexpr std::string_view im_used_message{"IM Used"};
			constexpr std::string_view multiple_choices_message{"Multiple Choices"};
			constexpr std::string_view moved_permanently_message{"Moved Permanently"};
			constexpr std::string_view found_message{"Found"};
			constexpr std::string_view see_other_message{"See Other"};
			constexpr std::string_view not_modified_message{"Not Modified"};
			constexpr std::string_view use_proxy_message{"Use Proxy"};
			constexpr std::string_view switch_proxy_message{"Switch Proxy"};
			constexpr std::string_view temporary_redirect_message{"Temporary Redirect"};
			constexpr std::string_view permanent_redirect_message{"Permanent Redirect"};
			constexpr std::string_view bad_request_message{"Bad Request"};
			constexpr std::string_view unauthorized_message{"Unauthorized"};
			constexpr std::string_view payment_required_message{"Payment Required"};
			constexpr std::string_view forbidden_message{"Forbidden"};
			constexpr std::string_view not_found_message{"Not Found"};
			constexpr std::string_view method_not_allowed_message{"METHOD_NOT_ALLOWED"};
			constexpr std::string_view not_acceptable_message{"Not Acceptable"};
			constexpr std::string_view proxy_authentication_required_message{"Proxy Authentication Required"};
			constexpr std::string_view request_timeout_message{"Request Timeout"};
			constexpr std::string_view conflict_message{"Conflict"};
			constexpr std::string_view gone_message{"Gone"};
			constexpr std::string_view length_required_message{"Length Required"};
			constexpr std::string_view precondition_failed_message{"Precondition Failed"};
			constexpr std::string_view payload_too_large_message{"Payload Too Large"};
			constexpr std::string_view uri_too_long_message{"URI Too Long"};
			constexpr std::string_view unsupported_media_type_message{"Unsupported Media Type"};
			constexpr std::string_view range_not_satisfiable_message{"Range Not Satisfiable"};
			constexpr std::string_view expectation_failed_message{"Expectation Failed"};
			constexpr std::string_view im_a_teapot_message{"I'm a teapot"};
			constexpr std::string_view misdirected_request_message{"Misdirected Request"};
			constexpr std::string_view unprocessable_entity_message{"Unprocessable Entity"};
			constexpr std::string_view locked_message{"Locked"};
			constexpr std::string_view failed_dependency_message{"Failed Dependency"};
			constexpr std::string_view too_early_message{"Too Early"};
			constexpr std::string_view upgrade_required_message{"Upgrade Required"};
			constexpr std::string_view precondition_required_message{"Precondition Required"};
			constexpr std::string_view too_many_requests_message{"Too Many Requests"};
			constexpr std::string_view request_header_fields_too_large_message{"Request Header Fields Too Large"};
			constexpr std::string_view unavailable_for_legal_reasons_message{"Unavailable For Legal Reasons"};
			constexpr std::string_view internal_server_error_message{"Internal Server Error"};
			constexpr std::string_view not_implemented_message{"Not Implemented"};
			constexpr std::string_view bad_gateway_message{"Bad Gateway"};
			constexpr std::string_view service_unavailable_message{"Service Unavailable"};
			constexpr std::string_view gateway_timeout_message{"Gateway Timeout"};
			constexpr std::string_view http_version_not_supported_message{"HTTP Version Not Supported"};
			constexpr std::string_view variant_also_negotiates_message{"Variant Also Negotiates"};
			constexpr std::string_view insufficient_storage_message{"Insufficient Storage"};
			constexpr std::string_view loop_detected_message{"Loop Detected"};
			constexpr std::string_view not_extended_message{"Not Extended"};
			constexpr std::string_view network_authentication_required_message{"Network Authentication Required"};

			constexpr std::string_view http_status_code_to_message(unsigned int code)
			{
				switch (code)
				{
					case 100:
						return status::continue_message;
					case 101:
						return status::switching_protocols_message;
					case 102:
						return status::processing_message;
					case 103:
						return status::early_hints_message;
					case 200:
						return status::ok_message;
					case 201:
						return status::created_message;
					case 202:
						return status::accepted_message;
					case 203:
						return status::non_authoritative_information_message;
					case 204:
						return status::no_content_message;
					case 205:
						return status::reset_content_message;
					case 206:
						return status::partial_content_message;
					case 207:
						return status::multi_status_message;
					case 208:
						return status::already_reported_message;
					case 226:
						return status::im_used_message;
					case 300:
						return status::multiple_choices_message;
					case 301:
						return status::moved_permanently_message;
					case 302:
						return status::found_message;
					case 303:
						return status::see_other_message;
					case 304:
						return status::not_modified_message;
					case 305:
						return status::use_proxy_message;
					case 306:
						return status::switch_proxy_message;
					case 307:
						return status::temporary_redirect_message;
					case 308:
						return status::permanent_redirect_message;
					case 400:
						return status::bad_request_message;
					case 401:
						return status::unauthorized_message;
					case 402:
						return status::payment_required_message;
					case 403:
						return status::forbidden_message;
					case 404:
						return status::not_found_message;
					case 405:
						return status::method_not_allowed_message;
					case 406:
						return status::not_acceptable_message;
					case 407:
						return status::proxy_authentication_required_message;
					case 408:
						return status::request_timeout_message;
					case 409:
						return status::conflict_message;
					case 410:
						return status::gone_message;
					case 411:
						return status::length_required_message;
					case 412:
						return status::precondition_failed_message;
					case 413:
						return status::payload_too_large_message;
					case 414:
						return status::uri_too_long_message;
					case 415:
						return status::unsupported_media_type_message;
					case 416:
						return status::range_not_satisfiable_message;
					case 417:
						return status::expectation_failed_message;
					case 418:
						return status::im_a_teapot_message;
					case 421:
						return status::misdirected_request_message;
					case 422:
						return status::unprocessable_entity_message;
					case 423:
						return status::locked_message;
					case 424:
						return status::failed_dependency_message;
					case 425:
						return status::too_early_message;
					case 426:
						return status::upgrade_required_message;
					case 428:
						return status::precondition_required_message;
					case 429:
						return status::too_many_requests_message;
					case 431:
						return status::request_header_fields_too_large_message;
					case 451:
						return status::unavailable_for_legal_reasons_message;
					case 500:
						return status::internal_server_error_message;
					case 501:
						return status::not_implemented_message;
					case 502:
						return status::bad_gateway_message;
					case 503:
						return status::service_unavailable_message;
					case 504:
						return status::gateway_timeout_message;
					case 505:
						return status::http_version_not_supported_message;
					case 506:
						return status::variant_also_negotiates_message;
					case 507:
						return status::insufficient_storage_message;
					case 508:
						return status::loop_detected_message;
					case 510:
						return status::not_extended_message;
					case 511:
						return status::network_authentication_required_message;
					default:
						throw std::invalid_argument{"Http status code is invalid"};
				}
			}

			struct http_status
			{
				http_status() = default;

				http_status(unsigned int code)
					: code{code}, message{http_status_code_to_message(code)} {}

				http_status(unsigned int code, std::string_view message="")
					: code{code}, message{message} {}

				http_status(unsigned int code, std::string&& message)
					: code{code}, message{std::move(message)} {}

				unsigned int code {};
				std::string message {};

				friend bool operator==(const http_status& lhs, const http_status& rhs) noexcept
				{
					return lhs.code == rhs.code;
				}

				friend bool operator!=(const http_status& lhs, const http_status& rhs) noexcept
				{
					return !(lhs == rhs);
				}
			};
		}

		

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
			malformed_response = 1,
			malformed_request = 2
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
						case http_errc::malformed_request:
							return "http malformed request";
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
		class scoped_http_socket
		{
			public:
				using socket_type = net::basic_socket_iostream<typename Owner::protocol_type>;

				scoped_http_socket(Owner& own)
					: owner_(own), s(std::move(own.socket())) {}

				scoped_http_socket(const scoped_http_socket&) = delete;
				scoped_http_socket& operator=(const scoped_http_socket&) = delete;

				net::basic_stream_socket<typename Owner::protocol_type>&& move_socket() noexcept
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

				~scoped_http_socket() noexcept
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

				basic_http_request(net::io_context& ctx)
					: socket_{ctx} {}

				const collin::net::web::url& url() const noexcept
				{
					return url_;
				}

				collin::net::web::url& url() noexcept
				{
					return url_;
				}

				void host(std::string_view str)
				{
					if(str != url_.host())
					{
						url_.host() = str;
						update_host_header();
						socket_.close();
					}
				}

				void port(net::ip::port_type p)
				{
					if(p != url_.port())
					{
						url_.port() = p;
						update_host_header();
						socket_.close();
					}
				}

				const std::string& method() const noexcept
				{
					return method_;
				}

				std::string& method() noexcept
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

				const std::map<std::string, std::string>& headers() const noexcept
				{
					return headers_;
				}

				std::chrono::milliseconds timeout() const noexcept
				{
					return timeout_;
				}

				std::chrono::milliseconds& timeout() noexcept
				{
					return timeout_;
				}

				http_version& version() noexcept
				{
					return version_;
				}

				http_version version() const noexcept
				{
					return version_;
				}

				socket_type& socket()
				{
					return socket_;
				}

				void socket(net::basic_socket<protocol_type>&& socket)
				{
					socket_.assign(endpoint_type{}.protocol(), socket.release());
				}

				const std::istream_iterator<char>& body() const
				{
					return body_;
				}

				std::istream_iterator<char>& body()
				{
					return body_;
				}

				void connect(std::error_code& ec) noexcept
				{
					ec.clear();
					std::string port_string;
					if (url_.port())
					{
						port_string = std::to_string(url_.port().value());
					}

					const auto results = protocol_type::resolver(socket_.context()).resolve(url_.host(), port_string);
					for (const auto& result : results)
					{
						socket_.open(result.endpoint().protocol(), ec);
						if (ec)
						{
							continue;
						}

						socket_.set_option(net::socket_base::timeout(timeout_), ec);
						if (ec)
						{
							continue;
						}

						socket_.connect(result.endpoint(), ec);
						if (!ec)
						{
							break;
						}
						
						ec.clear();
						socket_.close(ec);
						if (ec)
						{
							break;
						}
					}
				}

				bool is_open() const noexcept
				{
					return socket_.is_open();
				}

			private:
				collin::net::web::url url_ {"http", "", default_port};
				std::string method_ {get};
				http_version version_ {http_version::HTTP_1_1};
				std::map<std::string, std::string> headers_;
				std::istream_iterator<char> body_;
				std::chrono::milliseconds timeout_ {std::chrono::seconds(1)};
				socket_type socket_;

				void update_host_header()
				{
					std::string host_info = url_.host();
					if (url_.port())
					{
						host_info += ':' + std::to_string(url_.port().value());
					}

					headers_[headers::host.data()] = std::move(host_info);
				}
		};

		std::ostream& operator<<(std::ostream& stream, basic_http_request& req)
		{
			stream << req.method();
			stream << " ";
			stream << req.url().resource();
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

			std::copy(req.body(), std::istream_iterator<char>(), std::ostream_iterator<char>(stream));

			return stream;
		}

		class basic_http_response
		{
			public:
				using protocol_type = net::ip::tcp;
				using endpoint_type = typename protocol_type::endpoint;
				using socket_type = net::basic_stream_socket<protocol_type>;

				basic_http_response(socket_type&& sock, status::http_status&& status, http_version version, std::map<std::string, std::string>&& headers)
					: socket_(std::move(sock)), status_{std::move(status)}, version_(version), headers_(std::move(headers)) {}

				const status::http_status& status() const noexcept
				{
					return status_;
				}

				http_version version() const noexcept
				{
					return version_;
				}

				const std::map<std::string, std::string>& headers() const noexcept
				{
					return headers_;
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

				socket_type socket_;
				status::http_status status_;
				http_version version_;
				std::map<std::string, std::string> headers_;
				std::error_code ec_;
		};

		std::ostream& operator<<(std::ostream& os, basic_http_response& resp)
		{
			resp.ec_.clear();
			if(resp.socket().is_open())
			{
				scoped_http_socket s(resp);
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

				template<class HttpRequest>
				std::optional<basic_http_response> send(HttpRequest& req, std::error_code& ec) const
				{
					ec.clear();
					if (!req.is_open())
					{
						req.connect(ec);
					}
					if (ec)
					{
						return {};
					}

					scoped_http_socket s{req};
					s.socket() << req;
					s->sync();

					return parseResponse(s, ec);
				}

				template<class HttpRequest>
				basic_http_response send(HttpRequest& req) const
				{
					collin::throw_on_error e{"http_client::send"};
					try
					{
						return send(req, e).value();
					}
					catch (const std::bad_optional_access&)
					{
						throw std::runtime_error{"http_client::send did not properly throw an exception"};
					}
				}

				template<class HttpRequest>
				std::future<std::optional<basic_http_response>> async_send(HttpRequest& req, std::error_code& ec, const std::launch l = std::launch::async) const
				{
					using func_t = std::optional<basic_http_response>(http_client::*)(HttpRequest&, std::error_code&);
					auto f = static_cast<func_t>(&http_client::send);
					return std::async(l, f, std::ref(*this), std::ref(req), std::ref(ec));
				}

				template<class HttpRequest>
				std::future<basic_http_response> async_send(HttpRequest& req, const std::launch l = std::launch::async) const
				{
					using func_t = basic_http_response(http_client::*)(HttpRequest&, std::error_code&);
					auto f = static_cast<func_t>(&http_client::send);
					return std::async(l, f, std::ref(*this), std::ref(req));
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
				std::optional<basic_http_response> parseResponse(scoped_http_socket<HttpRequest>& s, std::error_code& ec) const
				{
					ec.clear();

					iterators::istream_iterator_sep space_stream {s.socket(), " "};

					if(space_stream->empty())
					{
						ec = make_error_code(http_errc::malformed_response);
						return {};
					}

					http_version version;
					try
					{
						version = http_version_type(*space_stream);
					}
					catch (const std::invalid_argument&)
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

					const unsigned int status_code = strings::from_string<unsigned int>(*space_stream, ec);
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

					const std::string status_code_message = std::move(*line_stream);

					std::map<std::string, std::string> headers;

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

					return std::make_optional<basic_http_response>(std::move(s.move_socket()), status::http_status{status_code, std::move(status_code_message)}, version, std::move(headers));
				}
		};
	}
}