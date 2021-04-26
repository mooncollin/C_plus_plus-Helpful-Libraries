module;

#include "cmoon/string/literals.hpp"

export module cmoon.format.write_boolean;

import <string>;
import <string_view>;
import <locale>;

import cmoon.string;

import cmoon.format.basic_format_context;
import cmoon.format.format_parser;
import cmoon.format.format_specs;
import cmoon.format.write_integer;
import cmoon.format.write_string_view;

namespace cmoon
{
	export
	template<class OutputIt, class CharT>
	typename basic_format_context<OutputIt, CharT>::iterator write_boolean(bool value, basic_format_context<OutputIt, CharT>& ctx, const format_parser<bool, CharT>& spec_info)
	{
		const auto type = spec_info.specifications().type;
			
		switch (type)
		{
			case CharT('b'):
			case CharT('B'):
			case CharT('d'):
			case CharT('o'):
			case CharT('x'):
			case CharT('X'):
				return write_integer(static_cast<int>(value), ctx, spec_info);
		}

		std::basic_string<CharT> dyn_boolean_string;
		std::basic_string_view<CharT> boolean_string;

		if (type == CharT('L'))
		{
			if (value)
			{
				dyn_boolean_string = std::use_facet<std::numpunct<CharT>>(ctx.locale()).truename();
			}
			else
			{
				dyn_boolean_string = std::use_facet<std::numpunct<CharT>>(ctx.locale()).falsename();
			}

			boolean_string = dyn_boolean_string;
		}
		else
		{
			if (value)
			{
				boolean_string = cmoon::choose_str_literal<CharT>(STR_LITERALS("true"));
			}
			else
			{
				boolean_string = cmoon::choose_str_literal<CharT>(STR_LITERALS("false"));
			}
		}

		return write_string_view(boolean_string, ctx, spec_info);
	}
}