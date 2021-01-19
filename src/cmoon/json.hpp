#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <iostream>
#include <vector>
#include <variant>
#include <iterator>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <optional>
#include <utility>

#include "cmoon/string.hpp"

namespace cmoon
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

		template<class BooleanType, class StringType, class IntType, class FloatType, template<class, class> class ListType, template<class...> class ObjectType, class Allocator = std::allocator<void>>
		struct basic_json_value
		{
			using boolean_type = BooleanType;
			using string_type = StringType;
			using int_type = IntType;
			using float_type = FloatType;
			using null_type = null;
			using list_type = ListType<basic_json_value, typename std::allocator_traits<Allocator>::template rebind_alloc<basic_json_value>>;

			private:
				template<class T, class Alloc>
				struct object_picker;

				template<class Alloc>
				struct object_picker<std::map<string_type, basic_json_value, std::less<string_type>, std::allocator<std::pair<const string_type, basic_json_value>>>, Alloc>
				{
					using type = std::map<string_type, basic_json_value, std::less<string_type>, typename std::allocator_traits<Alloc>::template rebind_alloc<std::pair<const string_type, basic_json_value>>>;
				};

				template<class Alloc>
				struct object_picker<std::unordered_map<string_type, basic_json_value, std::hash<string_type>, std::equal_to<string_type>, std::allocator<std::pair<const string_type, basic_json_value>>>, Alloc>
				{
					using type = std::unordered_map<string_type, basic_json_value, std::hash<string_type>, std::equal_to<string_type>, typename std::allocator_traits<Alloc>::template rebind_alloc<std::pair<const string_type, basic_json_value>>>;
				};

				template<class T, class Alloc>
				using object_picker_t = typename object_picker<T, Alloc>::type;
			public:

			using object_type = object_picker_t<ObjectType<string_type, basic_json_value>, Allocator>;

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
				requires(requires(list_type& l) { l[0]; })
			{
				return as_list()[key];
			}

			constexpr const basic_json_value& operator[](std::size_t key) const
				requires(requires(const list_type& l) { l[0]; })
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

		template<class Allocator = std::allocator<void>>
		using json_value = basic_json_value<bool, std::string, std::intmax_t, long double, std::list, std::map, Allocator>;

		template<class JsonType = json_value<>>
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

		template<class JsonType = json_value<>>
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

				json_parser() = default;

				std::basic_istream<json_char_type>& operator()(std::basic_istream<json_char_type>& is, json_value_type& json)
				{
					parse_input(is, json);
					return is;
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

				static constexpr auto eof = std::basic_streambuf<json_char_type>::traits_type::eof();

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

				void parse_input(std::basic_istream<json_char_type>& in, json_value_type& json)
				{
					std::stack<json_variant_type> state_stack;
					std::stack<json_string_type> name_stack;
					json_string_type token;
					json_string_type name;
					auto active_string = std::addressof(token);
					token_type type;
					std::stack<parse_state> state;
					state.push(parse_state::out);

					while (in && (type = json_parser::get_token(in, state.top(), *active_string)) != token_type::none)
					{
						if (type == token_type::string && active_string == std::addressof(name))
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
								state.push(parse_state::in_object);
								state_stack.emplace(std::in_place_type_t<json_object_type>{});
								active_string = &name;
								break;
							case token_type::array_start:
								state.push(parse_state::in_array);
								state_stack.emplace(std::in_place_type_t<json_list_type>{});
								break;
							case token_type::object_end:
								active_string = &token;
							case token_type::array_end:
								state.pop();
								end_container(state_stack, name_stack, state.top());
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
									insert_value(integer, state_stack, name_stack, state.top());
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
									insert_value(floating, state_stack, name_stack, state.top());
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
									insert_value(static_cast<json_boolean_type>(true), state_stack, name_stack, state.top());
								}
								break;
							case token_type::boolean_false:
								if (state_stack.empty())
								{
									state_stack.emplace(static_cast<json_boolean_type>(false));
								}
								else
								{
									insert_value(static_cast<json_boolean_type>(false), state_stack, name_stack, state.top());
								}
								break;
							case token_type::null:
								if (state_stack.empty())
								{
									state_stack.emplace(std::in_place_type_t<json_null_type>{});
								}
								else
								{
									insert_value(json_null_type{}, state_stack, name_stack, state.top());
								}
								break;
							case token_type::string:
								if (state_stack.empty())
								{
									state_stack.emplace(std::move(token));
								}
								else
								{
									insert_value(std::move(token), state_stack, name_stack, state.top());
								}
								token.clear();
								break;
							case token_type::value_delim:
								if (state.top() == parse_state::in_object)
								{
									active_string = &name;
								}
								break;
						}

						if (state.size() == 1)
						{
							break;
						}
					}

					if (state_stack.empty())
					{
						in.setstate(std::ios::failbit);
					}
					else if(!in.fail())
					{
						json = std::move(state_stack.top());
					}
				}

				static inline void end_container(std::stack<json_variant_type>& state_stack, std::stack<json_string_type>& name_stack, const parse_state current_state)
				{
					if (std::size(state_stack) > 1)
					{
						auto inner_container = std::move(state_stack.top());
						state_stack.pop();
						insert_value(std::move(inner_container), state_stack, name_stack, current_state);
					}
				}

				static token_type get_token(std::basic_istream<json_char_type>& is, const parse_state state, json_string_type& out)
				{
					std::basic_istream<json_char_type>::sentry se(is);
					auto sb = is.rdbuf();

					while(true)
					{
						const auto ch = sb->sbumpc();
						switch (ch)
						{
							case eof:
								return token_type::none;
							case object_start:
								return token_type::object_start;
							case object_end:
								switch (state)
								{
									case parse_state::in_object:
										return token_type::object_end;
									default:
										is.setstate(std::ios::failbit);
										return token_type::none;
								}
							case array_start:
								return token_type::array_start;
							case array_end:
								switch (state)
								{
									case parse_state::in_array:
										return token_type::array_end;
									default:
										is.setstate(std::ios::failbit);
										return token_type::none;
								}
							case string_char:
								gather_string(is, sb, out);
								return token_type::string;
							case value_sep:
								return token_type::value_sep;
							case value_delimiter:
								return token_type::value_delim;
							default:
								return gather_constant(is, sb, static_cast<json_char_type>(ch), out);
						}
					}

					return token_type::none;
				}

				static void gather_string(std::basic_istream<json_char_type>& is, std::basic_streambuf<json_char_type>* sb, json_string_type& out)
				{
					auto escaped {false};
					int ch;
					while ((ch = sb->sbumpc()) != eof)
					{
						if (escaped)
						{
							switch (ch)
							{
								case '"':
								case '\\':
								case '/':
									out += static_cast<json_char_type>(ch);
									break;
								case 'u':
								{
									auto i {0};
									while (((ch = sb->sbumpc()) != eof) && i++ < 4)
									{
										const auto hex_value = collin::strings::hex_to_base10(ch);
										if (hex_value < 0 || hex_value > 15)
										{
											is.setstate(std::ios::failbit);
											return;
										}
										out += static_cast<json_char_type>(hex_value);
									}

									if (i != 4)
									{
										is.setstate(std::ios::failbit);
										return;
									}
								}
									break;
								case 'b':
									out += '\b';
									break;
								case 'f':
									out += '\f';
									break;
								case 'n':
									out += '\n';
									break;
								case 'r':
									out += '\r';
									break;
								case 't':
									out += '\t';
									break;
								default:
									is.setstate(std::ios::failbit);
									return;
							}
							escaped = false;
							continue;
						}
						else
						{
							switch (ch)
							{
								case escape:
									escaped = true;
									continue;
								case string_char:
									return;
								default:
									break;
							}
						}

						out += static_cast<json_char_type>(ch);
					}

					is.setstate(std::ios::failbit);
				}

				static token_type gather_constant(std::basic_istream<json_char_type>& is, std::basic_streambuf<json_char_type>* sb, const json_char_type first_char, json_string_type& out)
				{
					typename std::basic_string_view<json_char_type>::iterator it;
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
					else if (std::isdigit(first_char) || first_char == '-')
					{
						out += first_char;
						type = token_type::integer;
					}
					else if (first_char == '.')
					{
						out += first_char;
						type = token_type::floating;
					}
					else
					{
						throw invalid_json{"Invalid JSON value"};
					}

					while (true)
					{
						const auto ch = sb->sbumpc();
						switch (type)
						{
							case token_type::integer:
								if (std::isdigit(ch))
								{
									out += static_cast<json_char_type>(ch);
								}
								else
								{
									switch (ch)
									{
										case '.':
											out += static_cast<json_char_type>(ch);
											type = token_type::floating;
											break;
										default:
											sb->sungetc();
										case eof:
											return type;
									}
								}
								break;
							case token_type::floating:
								if (std::isdigit(ch))
								{
									out += static_cast<json_char_type>(ch);
								}
								else
								{
									switch (ch)
									{
										default:
											sb->sungetc();
										case eof:
											return type;
									}
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
									is.setstate(std::ios::failbit);
									return token_type::none;
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
									is.setstate(std::ios::failbit);
									return token_type::none;
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
									is.setstate(std::ios::failbit);
									return token_type::none;
								}
						}
					}

					return type;
				}

				template<typename T>
				static inline void insert_value(T&& value, std::stack<json_variant_type>& state_stack, std::stack<json_string_type>& name_stack, const parse_state current_state)
				{
					switch (current_state)
					{
						case parse_state::in_object:
							std::get<json_object_type>(state_stack.top()).try_emplace(name_stack.top(), std::forward<T>(value));
							name_stack.pop();
							break;
						case parse_state::in_array:
							std::get<json_list_type>(state_stack.top()).emplace_back(std::forward<T>(value));
							break;
						default:
							break;
					}
				}
		};

		template<class BooleanType, class StringType, class IntType, class FloatType, template<class, class> class ListType, template<class...> class ObjectType>
		std::istream& operator>>(std::istream& is, basic_json_value<BooleanType, StringType, IntType, FloatType, ListType, ObjectType>& json)
		{
			using json_t = basic_json_value<BooleanType, StringType, IntType, FloatType, ListType, ObjectType>;
			json_parser<json_t> parser;
			return parser(is, json);
		}

		template<class BooleanType, class StringType, class IntType, class FloatType, template<class, class> class ListType, template<class...> class ObjectType>
		std::ostream& operator<<(std::ostream& os, const basic_json_value<BooleanType, StringType, IntType, FloatType, ListType, ObjectType>& json)
		{
			using json_t = basic_json_value<BooleanType, StringType, IntType, FloatType, ListType, ObjectType>;
			json_outputter<json_t> outputter {os};
			outputter(json);
			return os;
		}
	}
}