#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <iostream>
#include <vector>
#include <variant>
#include <iterator>
#include <stack>
#include <locale>
#include <stdexcept>
#include <map>

#include "collin/string.hpp"

namespace collin
{
	namespace json
	{
		class invalid_json : public std::runtime_error
		{
			public:
				invalid_json(const std::string& msg)
					: std::runtime_error(msg) {}
		};

		struct null {};

		template<class BooleanType, class StringType, class IntType, class FloatType>
		struct basic_json_value
		{
			using boolean_type = BooleanType;
			using string_type = StringType;
			using int_type = IntType;
			using float_type = FloatType;
			using null_type = null;
			using list_type = std::vector<basic_json_value>;
			using object_type = std::map<string_type, basic_json_value>;

			using type = std::variant<null_type, boolean_type, string_type, int_type, float_type, list_type, object_type>;

			constexpr operator const type& () const noexcept
			{
				return value;
			}

			constexpr operator type& () noexcept
			{
				return value;
			}

			constexpr basic_json_value(const type& v)
				: value{v} {}

			constexpr basic_json_value(type&& v) noexcept
				: value{std::forward<type>(v)} {}

			constexpr explicit basic_json_value() noexcept = default;

			constexpr basic_json_value(const basic_json_value&) = default;
			constexpr basic_json_value(basic_json_value&&) noexcept = default;

			constexpr basic_json_value& operator=(const basic_json_value&) = default;
			constexpr basic_json_value& operator=(basic_json_value&&) noexcept = default;

			template<class T>
			basic_json_value& operator=(T&& v)
			{
				value = std::forward<T>(v);
				return *this;
			}

			template<class T>
			constexpr basic_json_value(const T& v)
				: value{v} {}

			template<class T>
			constexpr basic_json_value(T&& v)
				: value{std::forward<T>(v)} {}

			constexpr basic_json_value& operator[](const string_type& key)
			{
				return as_object()[key];
			}

			constexpr basic_json_value& operator[](std::size_t key)
			{
				return as_list()[key];
			}

			constexpr const basic_json_value& operator[](std::size_t key) const
			{
				return as_list()[key];
			}

			constexpr basic_json_value& at(const string_type& key)
			{
				return as_object().at(key);
			}

			const basic_json_value& at(const string_type& key) const
			{
				return as_object().at(key);
			}

			constexpr basic_json_value& at(std::size_t key)
			{
				return as_list().at(key);
			}

			constexpr const basic_json_value& at(std::size_t key) const
			{
				return as_list().at(key);
			}

			constexpr boolean_type& as_boolean()
			{
				return std::get<boolean_type>(value);
			}

			constexpr const boolean_type& as_boolean() const
			{
				return std::get<boolean_type>(value);
			}

			constexpr string_type& as_string()
			{
				return std::get<string_type>(value);
			}

			constexpr const string_type& as_string() const
			{
				return std::get<string_type>(value);
			}

			constexpr int_type& as_integer()
			{
				return std::get<int_type>(value);
			}

			constexpr const int_type& as_integer() const
			{
				return std::get<int_type>(value);
			}

			constexpr float_type& as_floating()
			{
				return std::get<float_type>(value);
			}

			constexpr const float_type& as_floating() const
			{
				return std::get<float_type>(value);
			}

			constexpr list_type& as_list()
			{
				return std::get<list_type>(value);
			}

			constexpr const list_type& as_list() const
			{
				return std::get<list_type>(value);
			}

			constexpr object_type& as_object()
			{
				return std::get<object_type>(value);
			}

			constexpr const object_type& as_object() const
			{
				return std::get<object_type>(value);
			}

			constexpr bool is_boolean() const noexcept
			{
				return std::holds_alternative<boolean_type>(value);
			}

			constexpr bool is_string() const noexcept
			{
				return std::holds_alternative<string_type>(value);
			}

			constexpr bool is_integer() const noexcept
			{
				return std::holds_alternative<int_type>(value);
			}

			constexpr bool is_floating() const noexcept
			{
				return std::holds_alternative<float_type>(value);
			}

			constexpr bool is_null() const noexcept
			{
				return std::holds_alternative<null_type>(value);
			}

			constexpr bool is_list() const noexcept
			{
				return std::holds_alternative<list_type>(value);
			}

			constexpr bool is_object() const noexcept
			{
				return std::holds_alternative<object_type>(value);
			}

			type value {};
		};

		using json_value = basic_json_value<bool, std::string, std::intmax_t, double>;

		template<class CharT, class BooleanType, class StringType, class IntType, class FloatType>
		std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is, basic_json_value<BooleanType, StringType, IntType, FloatType>& json)
		{
			json_parser<basic_json_value<BooleanType, StringType, IntType, FloatType>> parser {is};
			json = std::move(parser.data());
			return is;
		}

		template<class CharT, class BooleanType, class StringType, class IntType, class FloatType>
		std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const basic_json_value<BooleanType, StringType, IntType, FloatType>& json)
		{
			using json_t = basic_json_value<BooleanType, StringType, IntType, FloatType>;
			json_outputter<json_t> outputter {os};
			outputter(json);
			return os;
		}

		template<class JsonType>
		class json_outputter
		{
			public:
				using json_value_type = JsonType;
				using json_boolean_type = typename json_value_type::boolean_type;
				using json_string_type = typename json_value_type::string_type;
				using json_int_type = typename json_value_type::int_type;
				using json_float_type = typename json_value_type::float_type;
				using json_null_type = typename json_value_type::null_type;
				using json_list_type = typename json_value_type::list_type;
				using json_object_type = typename json_value_type::object_type;

				using json_char_type = typename json_string_type::value_type;
				using json_variant_type = typename json_value_type::type;

				json_outputter(std::basic_ostream<json_char_type>& os)
					: os{os} {}

				inline void operator()(const json_int_type& value)
				{
					os.get() << value;
				}

				inline void operator()(const json_float_type& value)
				{
					os.get() << value;
				}

				inline void operator()(const json_boolean_type& value)
				{
					if (value)
					{
						os.get() << "true";
					}
					else
					{
						os.get() << "false";
					}
				}

				inline void operator()(const json_null_type& value)
				{
					os.get() << "null";
				}

				void operator()(const json_string_type& s)
				{
					os.get() << "\"";

					for (const auto ch : s)
					{
						switch (ch)
						{
							case '"':
								os.get() << "\\\"";
								break;
							case '\\':
								os.get() << "\\\\";
								break;
							case '/':
								os.get() << "\\/";
								break;
							case '\b':
								os.get() << "\\b";
								break;
							case '\f':
								os.get() << "\\f";
								break;
							case '\n':
								os.get() << "\\n";
								break;
							case '\r':
								os.get() << "\\r";
								break;
							case '\t':
								os.get() << "\\t";
								break;
							default:
								os.get() << ch;
								break;
						}
					}

					os.get() << "\"";
				}

				void operator()(const json_list_type& list)
				{
					os.get() << "[";
					for(auto it = std::cbegin(list); it != std::cend(list); ++it)
					{
						operator()(*it);
						if ((it + 1) != std::cend(list))
						{
							os.get() << ", ";
						}
					}
					os.get() << "]";
				}

				void operator()(const json_object_type& obj)
				{
					os.get() << "{";
					for(auto it = std::cbegin(obj); it != std::cend(obj); ++it)
					{
						operator()(it->first);
						os.get() << ": ";
						operator()(it->second);
						auto it2 = it;
						if (++it2 != std::cend(obj))
						{
							os.get() << ", ";
						}
					}
					os.get() << "}";
				}

				inline void operator()(const json_value_type& json)
				{
					operator()(json.value);
				}

				inline void operator()(const json_variant_type& json)
				{
					std::visit(*this, json);
				}
			private:
				std::reference_wrapper<std::basic_ostream<json_char_type>> os;
		};

		template<class JsonType = json_value>
		class json_parser
		{
			public:
				using json_value_type = JsonType;
				using json_boolean_type = typename json_value_type::boolean_type;
				using json_string_type = typename json_value_type::string_type;
				using json_int_type = typename json_value_type::int_type;
				using json_float_type = typename json_value_type::float_type;
				using json_null_type = typename json_value_type::null_type;
				using json_list_type = typename json_value_type::list_type;
				using json_object_type = typename json_value_type::object_type;

				using json_char_type = typename json_string_type::value_type;
				using json_variant_type = typename json_value_type::type;

				json_parser(std::basic_istream<json_char_type>& in)
				{
					parse_input(std::istreambuf_iterator<json_char_type>{in}, std::istreambuf_iterator<json_char_type>{});
				}

				json_parser(std::basic_string_view<json_char_type> in)
				{
					parse_input(std::begin(in), std::end(in));
				}

				const json_value_type& data() const noexcept
				{
					return data_;
				}

				json_value_type& data() noexcept
				{
					return data_;
				}

			private:
				static constexpr json_char_type value_delimiter {','};
				static constexpr json_char_type object_start {'{'};
				static constexpr json_char_type object_end {'}'};
				static constexpr json_char_type array_start {'['};
				static constexpr json_char_type array_end {']'};
				static constexpr json_char_type string_char {'"'};
				static constexpr json_char_type value_sep {':'};
				static constexpr json_char_type escape {'\\'};

				static constexpr std::basic_string_view<json_char_type> boolean_true {"true"};
				static constexpr std::basic_string_view<json_char_type> boolean_false {"false"};
				static constexpr std::basic_string_view<json_char_type> null {"null"};

				enum class parse_state
				{
					in_object,
					in_array,
					out
				};

				enum class token_type
				{
					integer,
					floating,
					string,
					name,
					value_delim,
					value_sep,
					boolean_true,
					boolean_false,
					object_start,
					object_end,
					array_start,
					array_end,
					null,
					none
				};

				json_value_type data_ {};

				template<class InputIterator>
				void parse_input(InputIterator begin, const InputIterator end)
				{
					std::stack<json_variant_type> state_stack;
					std::stack<json_string_type> name_stack;
					json_string_type token;
					json_string_type name;
					json_char_type constant_extra_char {'\0'};
					auto active_string = &token;
					token_type type;
					parse_state state {parse_state::out};

					while ((type = json_parser::get_token(begin, end, state, std::back_inserter(*active_string), constant_extra_char)) != token_type::none)
					{
						if (type == token_type::string && active_string == &name)
						{
							type = token_type::name;
						}

						switch (type)
						{
							case token_type::value_sep:
								break;
							case token_type::name:
								name_stack.push(std::move(name));
								name.clear();
								active_string = &token;
								break;
							case token_type::object_start:
								state = parse_state::in_object;
								state_stack.emplace(std::in_place_type_t<json_object_type>{});
								active_string = &name;
								break;
							case token_type::array_start:
								state_stack.emplace(std::in_place_type_t<json_list_type>{});
								state = parse_state::in_array;
								break;
							case token_type::object_end:
								active_string = &token;
							case token_type::array_end:
								state = end_container(state_stack, name_stack);
								break;
							case token_type::integer:
							{
								const auto integer = collin::strings::from_string<json_int_type>(token);
								if (state_stack.empty())
								{
									state_stack.emplace(integer);
								}
								else
								{
									insert_value(integer, state_stack, name_stack);
								}
								token.clear();
							}
								break;
							case token_type::floating:
							{
								const auto floating = collin::strings::from_string<json_float_type>(token);
								if (state_stack.empty())
								{
									state_stack.emplace(floating);
								}
								else
								{
									insert_value(floating, state_stack, name_stack);
								}
								token.clear();
							}
								break;
							case token_type::boolean_true:
								if (state_stack.empty())
								{
									state_stack.emplace(static_cast<json_boolean_type>(true));
								}
								else
								{
									insert_value(static_cast<json_boolean_type>(true), state_stack, name_stack);
								}
								break;
							case token_type::boolean_false:
								if (state_stack.empty())
								{
									state_stack.emplace(static_cast<json_boolean_type>(false));
								}
								else
								{
									insert_value(static_cast<json_boolean_type>(false), state_stack, name_stack);
								}
								break;
							case token_type::null:
								if (state_stack.empty())
								{
									state_stack.emplace(std::in_place_type_t<json_null_type>{});
								}
								else
								{
									insert_value(json_null_type{}, state_stack, name_stack);
								}
								break;
							case token_type::string:
								if (state_stack.empty())
								{
									state_stack.emplace(std::move(token));
								}
								else
								{
									insert_value(std::move(token), state_stack, name_stack);
								}
								token.clear();
								break;
							case token_type::value_delim:
								if (state == parse_state::in_object)
								{
									active_string = &name;
								}
								break;
						}

						if(constant_extra_char != '\0')
						{
							switch (type)
							{
								case token_type::integer:
								case token_type::floating:
									switch (state)
									{
										case parse_state::in_object:
											switch (constant_extra_char)
											{
												case object_end:
													active_string = &token;
													state = end_container(state_stack, name_stack);
													break;
												case value_delimiter:
													active_string = &name;
													break;
												default:
													if (!std::isspace(constant_extra_char, std::locale{}))
													{
														throw invalid_json {"Invalid number"};
													}
											}
											break;
										case parse_state::in_array:
											switch (constant_extra_char)
											{
												case array_end:
													state = end_container(state_stack, name_stack);
												case value_delimiter:
													break;
												default:
													if (!std::isspace(constant_extra_char, std::locale{}))
													{
														throw invalid_json {"Invalid number"};
													}
											}
											break;
										case parse_state::out:
											if (!std::isspace(constant_extra_char, std::locale{}))
											{
												throw invalid_json {"Invalid number"};
											}
									}
									break;
								default:
									break;
							}
						}

						constant_extra_char = '\0';
					}

					if (state_stack.empty())
					{
						throw invalid_json{"Invalid JSON structure"};
					}

					data_ = std::move(state_stack.top());
				}

				inline parse_state end_container(std::stack<json_variant_type>& state_stack, std::stack<json_string_type>& name_stack)
				{
					if (std::size(state_stack) > 1)
					{
						auto inner_container = std::move(state_stack.top());
						state_stack.pop();
						return insert_value(std::move(inner_container), state_stack, name_stack);
					}

					return parse_state::out;
				}

				template<class InputIterator, class OutputIterator>
				constexpr static token_type get_token(InputIterator& begin, const InputIterator end, const parse_state state, OutputIterator out, typename InputIterator::value_type& extra_char)
				{
					while (begin != end)
					{
						const auto ch = *begin++;
						if(!std::isspace(ch))
						{
							switch (ch)
							{
								case object_start:
									return token_type::object_start;
								case object_end:
									switch (state)
									{
										case parse_state::in_object:
											return token_type::object_end;
										default:
											throw invalid_json {"Invalid end of object"};
									}
								case array_start:
									return token_type::array_start;
								case array_end:
									switch (state)
									{
										case parse_state::in_array:
											return token_type::array_end;
										default:
											throw invalid_json {"Invalid end of array"};
									}
								case string_char:
									gather_string(begin, end, out);
									return token_type::string;
								case value_sep:
									return token_type::value_sep;
								case value_delimiter:
									return token_type::value_delim;
								default:
									return gather_constant(begin, end, ch, out, extra_char);
							}
						}
					}

					return token_type::none;
				}

				template<class InputIterator, class OutputIterator>
				constexpr static void gather_string(InputIterator& begin, const InputIterator end, OutputIterator out)
				{
					auto escaped {false};

					while (begin != end)
					{
						auto ch = *begin++;
						if (escaped)
						{
							switch (ch)
							{
								case '"':
								case '\\':
								case '/':
									*out++ = ch;
									break;
								case 'u':
								{
									auto i {0};
									while (begin != end && i++ < 4)
									{
										const auto hex_value = collin::strings::hex_char_to_number(*begin++);
										if (hex_value < 0 || hex_value > 15)
										{
											throw invalid_json {"Invalid unicode string"};
										}
										*out++ = static_cast<json_char_type>(hex_value);
									}
									if (i != 4)
									{
										throw invalid_json {"Unexpected ending of JSON string"};
									}
								}
									break;
								case 'b':
									*out++ = '\b';
									break;
								case 'f':
									*out++ = '\f';
									break;
								case 'n':
									*out++ = '\n';
									break;
								case 'r':
									*out++ = '\r';
									break;
								case 't':
									*out++ = '\t';
									break;
								default:
									throw invalid_json {"Invalid escape character"};
							}
							escaped = false;
							continue;
						}
						else
						{
							if (ch == escape)
							{
								escaped = true;
								continue;
							}
							else if (ch == string_char)
							{
								return;
							}
						}

						*out++ = ch;
					}

					throw invalid_json {"Unexpected end of JSON"};
				}

				template<class InputIterator, class OutputIterator>
				constexpr static token_type gather_constant(InputIterator& begin, const InputIterator end, const typename InputIterator::value_type first_char, OutputIterator out, typename InputIterator::value_type& extra_char)
				{
					std::basic_string_view<json_char_type>::iterator it;
					auto type {token_type::none};

					if (first_char == boolean_true.front())
					{
						it = std::begin(boolean_true) + 1;
						type = token_type::boolean_true;
					}
					else if (first_char == boolean_false.front())
					{
						it = std::begin(boolean_false) + 1;
						type = token_type::boolean_false;
					}
					else if (first_char == null.front())
					{
						it = std::begin(null) + 1;
						type = token_type::null;
					}
					else if (std::isdigit(first_char, std::locale{}) || first_char == '-')
					{
						*out++ = first_char;
						type = token_type::integer;
					}
					else if (first_char == '.')
					{
						*out++ = first_char;
						type = token_type::floating;
					}
					else
					{
						throw invalid_json{"Invalid JSON value"};
					}

					while (begin != end)
					{
						const auto ch = *begin++;
						switch (type)
						{
							case token_type::integer:
								if (std::isdigit(ch, std::locale{}))
								{
									*out++ = ch;
								}
								else if (ch == '.')
								{
									*out++ = ch;
									type = token_type::floating;
								}
								else
								{
									extra_char = ch;
									return type;
								}
								break;
							case token_type::floating:
								if (std::isdigit(ch, std::locale{}))
								{
									*out++ = ch;
								}
								else
								{
									extra_char = ch;
									return type;
								}
								break;
							case token_type::boolean_true:
								if (ch == *it)
								{
									if (++it == std::end(boolean_true))
									{
										return token_type::boolean_true;
									}
								}
								else
								{
									throw invalid_json {"Invalid boolean value"};
								}
								break;
							case token_type::boolean_false:
								if (ch == *it)
								{
									if (++it == std::end(boolean_false))
									{
										return token_type::boolean_false;
									}
								}
								else
								{
									throw invalid_json {"Invalid boolean value"};
								}
								break;
							case token_type::null:
								if (ch == *it)
								{
									if (++it == std::end(null))
									{
										return token_type::null;
									}
								}
								else
								{
									throw invalid_json {"Invalid null value"};
								}
						}
					}

					return type;
				}

				template<typename T>
				inline parse_state insert_value(T&& value, std::stack<json_variant_type>& state_stack, std::stack<json_string_type>& name_stack)
				{
					return std::visit(collin::overloaded {
						[&value, &name_stack](json_object_type& obj) {
							obj.try_emplace(name_stack.top(), std::forward<T>(value));
							name_stack.pop();
							return parse_state::in_object;
						},
						[&value](json_list_type& list) {
							list.emplace_back(std::forward<T>(value));
							return parse_state::in_array;
						},
						[](auto&) {
							throw invalid_json {"Cannot put a value into a non-container"};
							return parse_state::out; // The compiler complains unless this is here
						}
					}, state_stack.top());
				}
		};
	}
}