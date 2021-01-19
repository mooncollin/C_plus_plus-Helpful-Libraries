#pragma once

#include <string>
#include <map>
#include <optional>
#include <string_view>
#include <iostream>

#include "cmoon/string.hpp"
#include "cmoon/net/internet.hpp"

namespace cmoon
{
	namespace net
	{
		namespace web
		{
			template<class CharT>
			[[nodiscard]] constexpr std::basic_string_view<CharT> url_encode_char(const CharT& c) noexcept
			{
				switch (c)
				{
					case CharT(' '):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("+"));
					case CharT('+'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%2B"));
					case CharT('~'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%7E"));
					case CharT('!'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%21"));
					case CharT('#'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%23"));
					case CharT('$'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%24"));
					case CharT('%'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%25"));
					case CharT('&'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%26"));
					case CharT('\''):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%27"));
					case CharT('('):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%28"));
					case CharT(')'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%29"));
					case CharT('*'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%2A"));
					case CharT(','):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%2C"));
					case CharT('/'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%2F"));
					case CharT(':'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%3A"));
					case CharT(';'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%3B"));
					case CharT('='):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%3D"));
					case CharT('?'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%3F"));
					case CharT('@'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%40"));
					case CharT('['):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%5B"));
					case CharT(']'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%5D"));
					case CharT('\n'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%0A"));
					case CharT('"'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%22"));
					case CharT('-'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%2D"));
					case CharT('.'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%2E"));
					case CharT('<'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%3C"));
					case CharT('>'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%3E"));
					case CharT('\\'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%5C"));
					case CharT('^'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%5E"));
					case CharT('`'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%60"));
					case CharT('{'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%7B"));
					case CharT('}'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%7C"));
					case CharT('|'):
						return cmoon::choose_str_literal<CharT>(STR_LITERALS("%7C"));
					default:
						return std::basic_string_view<CharT>{std::addressof(c), 1};
				}
			}

			std::string url_encode_string(std::string_view str)
			{
				std::string result;
				result.reserve(std::size(str));

				for (auto ch : str)
				{
					result += url_encode_char(ch);
				}

				return result;
			}

			template<class Map>
			[[nodiscard]] std::string create_parameter_string(const Map& map)
			{
				std::string result;

				if (!map.empty())
				{
					result += '?';
				}

				for (const auto& [key, value] : map)
				{
					result += url_encode_string(key) + '=' + url_encode_string(value) + '&';
				}

				if (!map.empty())
				{
					// Get rid of the last '&'
					result.erase(std::end(result) - 1);
				}

				return result;
			}

			class url
			{
				public:
					url(std::optional<std::string> protocol = {}, std::string_view host = "",
						std::optional<cmoon::net::ip::port_type> port = {},
						std::string_view resource = "/",
						const std::map<std::string, std::string>& parameters = {})
						: protocol_{std::move(protocol)}, host_{host}, port_{port},
						  resource_{resource}, parameters_{parameters} {}

					[[nodiscard]] std::string to_string() const
					{
						std::string result;
						create_pre_parameter_string(result);

						if (!parameters_.empty())
						{
							result += '?';
						}

						for (const auto& [key, value] : parameters_)
						{
							result += key + '=' + value + '&';
						}

						if (!parameters_.empty())
						{
							// Get rid of the last '&'
							result.erase(std::end(result) - 1);
						}

						return result;
					}

					[[nodiscard]] std::string encode_string() const
					{
						std::string result;
						create_pre_parameter_string(result);
						result += create_parameter_string(parameters_);
						return result;
					}

					void protocol(std::string_view p)
					{
						protocol_ = p;
					}

					void protocol(const std::optional<std::string>& p)
					{
						protocol_ = p;
					}

					const std::optional<std::string>& protocol() const
					{
						return protocol_;
					}

					std::string& host()
					{
						return host_;
					}

					const std::string& host() const
					{
						return host_;
					}

					std::string& resource()
					{
						return resource_;
					}

					const std::string& resource() const
					{
						return resource_;
					}

					void port(const std::optional<cmoon::net::ip::port_type>& p)
					{
						port_ = p;
					}

					void port(cmoon::net::ip::port_type p)
					{
						port_ = p;
					}

					const std::optional<cmoon::net::ip::port_type>& port() const
					{
						return port_;
					}

					std::map<std::string, std::string>& parameters()
					{
						return parameters_;
					}

					const std::map<std::string, std::string>& parameters() const
					{
						return parameters_;
					}
				private:
					std::optional<std::string> protocol_;
					std::string host_;
					std::optional<cmoon::net::ip::port_type> port_;
					std::string resource_;
					std::map<std::string, std::string> parameters_;

					void create_pre_parameter_string(std::string& str) const
					{
						if (protocol_)
						{
							str += protocol_.value() + "://";
						}

						str += host_;

						if (port_)
						{
							str += ':' + std::to_string(port_.value());
						}

						str += resource_;
					}
			};

			std::ostream& operator<<(std::ostream& os, const url& u)
			{
				if (u.protocol())
				{
					os << u.protocol().value() << "://";
				}

				os << u.host();

				if (u.port())
				{
					os << ':' << u.port().value();
				}

				os << u.resource();

				if (!u.parameters().empty())
				{
					os << '?';
				}

				const auto end = u.parameters().cend();
				for (auto it = u.parameters().cbegin(); it != end; ++it)
				{
					os << url_encode_string(it->first) << '=' << url_encode_string(it->second);

					if (std::next(it) != end)
					{
						os << '&';
					}
				}

				return os;
			}
		}
	}
}