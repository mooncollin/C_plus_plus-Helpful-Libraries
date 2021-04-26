export module cmoon.format.write_string_view;

import <string>;
import <string_view>;
import <algorithm>

import cmoon.format.basic_format_context;
import cmoon.format.format_parser;
import cmoon.format.format_specs;

namespace cmoon
{
	export
	template<class T, class OutputIt, class CharT, class Traits = std::char_traits<CharT>>
	typename basic_format_context<OutputIt, CharT>::iterator write_string_view(std::basic_string_view<CharT, Traits> str, basic_format_context<OutputIt, CharT>& ctx, const format_parser<T, CharT>& spec_info)
	{
		const auto width = spec_info.get_width(ctx);
		const auto alignment = spec_info.specifications().align == align_t::none ? align_t::left : spec_info.specifications().align;
		const auto fill = spec_info.specifications().fill;

		const auto fill_amount = width == 0 ? 0 : width - std::size(str);

		if (alignment == align_t::left)
		{
			ctx.advance_to(std::copy(std::begin(str), std::end(str), ctx.out()));
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount, fill));
		}
		else if (alignment == align_t::center)
		{
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount / 2, fill));
			ctx.advance_to(std::copy(std::begin(str), std::end(str), ctx.out()));
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount - fill_amount / 2, fill));
		}
		else if (alignment == align_t::right)
		{
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount, fill));
			ctx.advance_to(std::copy(std::begin(str), std::end(str), ctx.out()));
		}

		return ctx.out();
	}
}