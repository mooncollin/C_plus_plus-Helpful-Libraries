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
#include "collin/flat_map.hpp"

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

			using code_t = unsigned int;

			constexpr auto code_to_message = collin::make_fixed_flat_map(
				std::pair{100u, status::continue_message},
				std::pair{101u, status::switching_protocols_message},
				std::pair{102u, status::processing_message},
				std::pair{103u, status::early_hints_message},
				std::pair{200u, status::ok_message},
				std::pair{201u, status::created_message},
				std::pair{202u, status::accepted_message},
				std::pair{203u, status::non_authoritative_information_message},
				std::pair{204u, status::no_content_message},
				std::pair{205u, status::reset_content_message},
				std::pair{206u, status::partial_content_message},
				std::pair{207u, status::multi_status_message},
				std::pair{208u, status::already_reported_message},
				std::pair{226u, status::im_used_message},
				std::pair{300u, status::multiple_choices_message},
				std::pair{301u, status::moved_permanently_message},
				std::pair{302u, status::found_message},
				std::pair{303u, status::see_other_message},
				std::pair{304u, status::not_modified_message},
				std::pair{305u, status::use_proxy_message},
				std::pair{306u, status::switch_proxy_message},
				std::pair{307u, status::temporary_redirect_message},
				std::pair{308u, status::permanent_redirect_message},
				std::pair{400u, status::bad_request_message},
				std::pair{401u, status::unauthorized_message},
				std::pair{402u, status::payment_required_message},
				std::pair{403u, status::forbidden_message},
				std::pair{404u, status::not_found_message},
				std::pair{405u, status::method_not_allowed_message},
				std::pair{406u, status::not_acceptable_message},
				std::pair{407u, status::proxy_authentication_required_message},
				std::pair{408u, status::request_timeout_message},
				std::pair{409u, status::conflict_message},
				std::pair{410u, status::gone_message},
				std::pair{411u, status::length_required_message},
				std::pair{412u, status::precondition_failed_message},
				std::pair{413u, status::payload_too_large_message},
				std::pair{414u, status::uri_too_long_message},
				std::pair{415u, status::unsupported_media_type_message},
				std::pair{416u, status::range_not_satisfiable_message},
				std::pair{417u, status::expectation_failed_message},
				std::pair{418u, status::im_a_teapot_message},
				std::pair{421u, status::misdirected_request_message},
				std::pair{422u, status::unprocessable_entity_message},
				std::pair{423u, status::locked_message},
				std::pair{424u, status::failed_dependency_message},
				std::pair{425u, status::too_early_message},
				std::pair{426u, status::upgrade_required_message},
				std::pair{428u, status::precondition_required_message},
				std::pair{429u, status::too_many_requests_message},
				std::pair{431u, status::request_header_fields_too_large_message},
				std::pair{451u, status::unavailable_for_legal_reasons_message},
				std::pair{500u, status::internal_server_error_message},
				std::pair{501u, status::not_implemented_message},
				std::pair{502u, status::bad_gateway_message},
				std::pair{503u, status::service_unavailable_message},
				std::pair{504u, status::gateway_timeout_message},
				std::pair{505u, status::http_version_not_supported_message},
				std::pair{506u, status::variant_also_negotiates_message},
				std::pair{507u, status::insufficient_storage_message},
				std::pair{508u, status::loop_detected_message},
				std::pair{510u, status::not_extended_message}
			);

			struct http_status
			{
				http_status() = default;

				http_status(unsigned int code)
					: code{code}, message{code_to_message.at(code)} {}

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
						url_.port(p);
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

				void add_header(std::string_view name, std::string_view value)
				{
					headers_[name.data()] = value;
				}

				const std::string& get_header(std::string_view name) const
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
					std::string service_string;
					if (url_.port())
					{
						service_string = std::to_string(url_.port().value());
					}
					else if (url_.protocol())
					{
						service_string = url_.protocol().value();
					}

					const auto results = protocol_type::resolver(socket_.context()).resolve(url_.host(), service_string);
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
			stream << req.url().resource() << collin::net::web::create_parameter_string(req.url().parameters());
			stream << " ";
			stream << http_version_string(req.version());

			for (const auto& [name, value] : req.headers())
			{
				stream << "\r\n";
				stream << name;
				stream << ": ";
				stream << value;
			}

			stream << line_terminator << line_terminator;

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

				http_client(collin::net::io_context& ctx)
					: ctx_{ctx}
				{
					collin::net::use_service<collin::net::windows_socket_service>(ctx_.get());
				}

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
					std::error_code ec;
					auto op = send(req, ec);
					if (ec)
					{
						throw std::system_error{ec};
					}

					return std::move(op).value();
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
				std::reference_wrapper<collin::net::io_context> ctx_;
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
					catch (...)
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

					status::code_t status_code;
					try
					{
						status_code = strings::from_string<status::code_t>(std::string_view{*space_stream});
					}
					catch (...)
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

					auto status_code_message = std::move(*line_stream);

					std::map<std::string, std::string> headers;

					while (true)
					{
						++line_stream;
						if(line_stream->empty())
						{
							break;
						}
						
						const std::string_view line{*line_stream};

						const auto colon_loc = line.find(':');
						if (colon_loc == std::string_view::npos)
						{
							ec = make_error_code(http_errc::malformed_response);
							return {};
						}


						const auto key = line.substr(0, colon_loc);
						const auto value = strings::trim_front(colon_loc + 1 > std::size(line) ? std::string_view{""} : line.substr(colon_loc + 1), std::string_view{" "});

						headers[std::string{key}] = value;
					}

					return std::make_optional<basic_http_response>(std::move(s.move_socket()), status::http_status{status_code, std::move(status_code_message)}, version, std::move(headers));
				}
		};
	}
}