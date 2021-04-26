module;

#include <variant>
#include <concepts>
#include <string_view>

#include "cmoon/string_literals.hpp"

export module cmoon.format.format_parser;
import cmoon.format.format_error;
import cmoon.format.basic_format_context;
import cmoon.format.basic_format_parse_context;
import cmoon.format.format_specs;
import cmoon.utility;
import cmoon.string;

namespace cmoon
{
	export
	template<typename T, typename CharT>
	class format_parser
	{
		using context_t = basic_format_parse_context<CharT>;

		public:
			constexpr format_parser() noexcept = default;
			constexpr format_parser(context_t& parse_ctx)
			{
				parse(parse_ctx);
			}

			constexpr void parse(context_t& parse_ctx)
			{
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_align(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_sign(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_hash(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_zero_flag(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_width(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_precision(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_type(parse_ctx);
			}

			[[nodiscard]] constexpr const basic_format_specs<CharT>& specifications() const noexcept
			{
				return specs;
			}

			[[nodiscard]] constexpr const dynamic_format_specs<CharT>& dynamic_specifications() const noexcept
			{
				return dynamic_specs;
			}

			[[nodiscard]] constexpr bool is_width_dynamic() const noexcept
			{
				return dynamic_specs.width.has_value();
			}

			[[nodiscard]] constexpr bool is_precision_dynamic() const noexcept
			{
				return dynamic_specs.precision.has_value();
			}

			template<class OutputIt>
			[[nodiscard]] constexpr int get_width(const basic_format_context<OutputIt, CharT>& ctx) const noexcept
			{
				struct f
				{
					int operator()(int v) const noexcept
					{
						return v;
					}

					int operator()(const auto&) const noexcept
					{
						return 0;
					}
				};

				if (is_width_dynamic())
				{
					std::visit(f{}, ctx.arg(dynamic_specs.width.value()).value);
					//return visit_format_arg(cmoon::overloaded {
					//	[](int v) { return v; },
					//	[](const auto&) { return 0; },
					//}, ctx.arg(dynamic_specs.width.value()));
				}

				return specs.width;
			}

			template<class OutputIt>
			[[nodiscard]] constexpr int get_precision(const basic_format_context<OutputIt, CharT>& ctx) const noexcept
			{
				struct f
				{
					int operator()(int v) const noexcept
					{
						return v;
					}

					int operator()(const auto&) const noexcept
					{
						return -1;
					}
				};

				if (is_precision_dynamic())
				{
					return visit_format_arg(f{}, ctx.arg(dynamic_specs.precision.value()));
				}

				return specs.precision;
			}
		private:
			basic_format_specs<CharT> specs;
			dynamic_format_specs<CharT> dynamic_specs;

			static constexpr auto open_brace_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('{'));
			static constexpr auto close_brace_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('}'));
			static constexpr auto left_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('<'));
			static constexpr auto right_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('>'));
			static constexpr auto numeric_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('='));
			static constexpr auto center_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('^'));
			static constexpr auto plus_sign_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('+'));
			static constexpr auto minus_sign_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('-'));
			static constexpr auto space_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS(' '));
			static constexpr auto hash_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('#'));
			static constexpr auto zero_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('0'));
			static constexpr auto precision_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('.'));

			constexpr void parse_align(context_t& parse_ctx)
			{
				auto align = align_t::none;

				auto p = parse_ctx.begin() + cmoon::code_point_length(get_char(parse_ctx));
				if (p >= parse_ctx.end())
				{
					p = parse_ctx.begin();
				}

				while (true)
				{
					switch (*p)
					{
						case left_align_char:
							align = align_t::left;
							break;
						case right_align_char:
							align = align_t::right;
							break;
						case numeric_align_char:
							align = align_t::numeric;
							break;
						case center_align_char:
							align = align_t::center;
							break;
					}

					if (align != align_t::none)
					{
						if (p != parse_ctx.begin())
						{
							if (get_char(parse_ctx) == open_brace_char)
							{
								throw format_error{"invalid fill character '{'"};
							}

							specs.fill = get_char(parse_ctx);
							parse_ctx.advance_to(p + 1);
						}
						else
						{
							advance_one(parse_ctx);
						}

						specs.align = align;
						break;
					}
					else if (p == parse_ctx.begin())
					{
						break;
					}

					p = parse_ctx.begin();
				}
			}

			constexpr void parse_sign(context_t& parse_ctx) noexcept
			{
				switch (get_char(parse_ctx))
				{
					case plus_sign_char:
						specs.sign = sign_t::plus;
						advance_one(parse_ctx);
						break;
					case minus_sign_char:
						specs.sign = sign_t::minus;
						advance_one(parse_ctx);
						break;
					case space_char:
						specs.sign = sign_t::space;
						advance_one(parse_ctx);
						break;
				}
			}

			constexpr void parse_hash(context_t& parse_ctx) noexcept
			{
				if (get_char(parse_ctx) == hash_char)
				{
					specs.alt = true;
					advance_one(parse_ctx);
				}
			}

			constexpr void parse_zero_flag(context_t& parse_ctx) noexcept
			{
				if (get_char(parse_ctx) == zero_char)
				{
					specs.align = align_t::numeric;
					specs.fill = zero_char;
					advance_one(parse_ctx);
				}
			}

			constexpr void parse_width(context_t& parse_ctx)
			{
				if (cmoon::is_digit(get_char(parse_ctx)))
				{
					specs.width = parse_nonnegative_int(parse_ctx);
				}
				else if (get_char(parse_ctx) == open_brace_char)
				{
					advance_one(parse_ctx);
					if (!at_end(parse_ctx))
					{
						dynamic_specs.width = parse_arg_id(parse_ctx);
					}

					if (at_end(parse_ctx) || get_char(parse_ctx) != close_brace_char)
					{
						throw format_error{"invalid format string"};
					}

					advance_one(parse_ctx);
				}
			}

			constexpr void parse_precision(context_t& parse_ctx)
			{
				if (get_char(parse_ctx) == precision_char)
				{
					advance_one(parse_ctx);
					auto c = !at_end(parse_ctx) ? get_char(parse_ctx) : CharT{};
					if (cmoon::is_digit(c))
					{
						specs.precision = parse_nonnegative_int(parse_ctx);
					}
					else if (c == open_brace_char)
					{
						advance_one(parse_ctx);
						if (!at_end(parse_ctx))
						{
							dynamic_specs.precision = parse_arg_id(parse_ctx);
						}

						if (at_end(parse_ctx) || get_char(parse_ctx) != close_brace_char)
						{
							throw format_error{"invalid format string"};
						}
						else
						{
							throw format_error{"missing precision specifier"};
						}
					}
				}
			}

			constexpr void parse_type(context_t& parse_ctx)
			{
				// Can compile out type parsing step if the type that is being
				// formatted is not expecting a type.
				if constexpr (std::integral<T> || 
								std::floating_point<T> || 
								std::same_as<std::decay_t<T>, std::basic_string_view<CharT>> ||
								std::same_as<std::decay_t<T>, void*>)
				{
					if (!at_end(parse_ctx) && get_char(parse_ctx) != close_brace_char)
					{
						const auto ch = get_char(parse_ctx);
						if constexpr (std::same_as<T, CharT> || std::same_as<T, char>)
						{
							switch (ch)
							{
								case CharT('b'):
								case CharT('B'):
								case CharT('d'):
								case CharT('o'):
								case CharT('x'):
								case CharT('X'):
								case CharT('L'):
								case CharT('c'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::same_as<std::decay_t<T>, std::basic_string_view<CharT>>)
						{
							switch (ch)
							{
								case CharT('s'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::integral<T> /* and boolean */)
						{
							switch (ch)
							{
								case CharT('b'):
								case CharT('B'):
								case CharT('d'):
								case CharT('o'):
								case CharT('x'):
								case CharT('X'):
								case CharT('L'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::floating_point<T>)
						{
							switch (ch)
							{
								case CharT('a'):
								case CharT('A'):
								case CharT('e'):
								case CharT('E'):
								case CharT('f'):
								case CharT('F'):
								case CharT('g'):
								case CharT('G'):
								case CharT('L'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::same_as<std::decay_t<T>, void*>)
						{
							switch (ch)
							{
								case CharT('p'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
					}
				}
			}
	};
}