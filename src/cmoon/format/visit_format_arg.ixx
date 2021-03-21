module;

#include <variant>

#include "cmoon/format/forward_declarations.hpp"

export module cmoon.format.visit_format_arg;

namespace cmoon
{
	export
	template<class Visitor, class Ctx>
	decltype(auto) visit_format_arg(Visitor&& vis, basic_format_arg<Ctx> arg)
	{
		return std::visit(std::forward<Visitor>(vis), arg.value);
	}
}