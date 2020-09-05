#pragma once

#include <string>
#include <map>
#include <optional>
#include <string_view>

#include "collin/net/internet.hpp"

namespace collin
{
	namespace net
	{
		namespace web
		{
			template<class CharT>
			std::basic_string<CharT> url_encode_char(const CharT& c)
			{
				switch (c)
				{
					case static_cast<CharT>(' '):
						return "+";
					case static_cast<CharT>('+'):
						return "%2B";
					case static_cast<CharT>('~'):
						return "%7E";
					case static_cast<CharT>('!'):
						return "%21";
					case static_cast<CharT>('#'):
						return "%23";
					case static_cast<CharT>('$'):
						return "%24";
					case static_cast<CharT>('%'):
						return "%25";
					case static_cast<CharT>('&'):
						return "%26";
					case static_cast<CharT>('\''):
						return "%27";
					case static_cast<CharT>('('):
						return "%28";
					case static_cast<CharT>(')'):
						return "%29";
					case static_cast<CharT>('*'):
						return "%2A";
					case static_cast<CharT>(','):
						return "%2C";
					case static_cast<CharT>('/'):
						return "%2F";
					case static_cast<CharT>(':'):
						return "%3A";
					case static_cast<CharT>(';'):
						return "%3B";
					case static_cast<CharT>('='):
						return "%3D";
					case static_cast<CharT>('?'):
						return "%3F";
					case static_cast<CharT>('@'):
						return "%40";
					case static_cast<CharT>('['):
						return "%5B";
					case static_cast<CharT>(']'):
						return "%5D";
					case static_cast<CharT>('\n'):
						return "%0A";
					case static_cast<CharT>('"'):
						return "%22";
					case static_cast<CharT>('-'):
						return "%2D";
					case static_cast<CharT>('.'):
						return "%2E";
					case static_cast<CharT>('<'):
						return "%3C";
					case static_cast<CharT>('>'):
						return "%3E";
					case static_cast<CharT>('\\'):
						return "%5C";
					case static_cast<CharT>('^'):
						return "%5E";
					case static_cast<CharT>('_'):
						return "%5F";
					case static_cast<CharT>('`'):
						return "%60";
					case static_cast<CharT>('{'):
						return "%7B";
					case static_cast<CharT>('}'):
						return "%7C";
					case static_cast<CharT>('|'):
						return "%7C";
					default:
						return std::basic_string<CharT>{c};
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
			std::string create_parameter_string(const Map& map)
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
						std::optional<collin::net::ip::port_type> port = {},
						std::string_view resource = "/",
						const std::map<std::string, std::string>& parameters = {})
						: protocol_{std::move(protocol)}, host_{host}, port_{port},
						  resource_{resource}, parameters_{parameters} {}

					std::string to_string() const
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

					std::string encode_string() const
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

					void port(const std::optional<collin::net::ip::port_type>& p)
					{
						port_ = p;
					}

					void port(collin::net::ip::port_type p)
					{
						port_ = p;
					}

					const std::optional<collin::net::ip::port_type>& port() const
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
					std::optional<collin::net::ip::port_type> port_;
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
		}
	}
}