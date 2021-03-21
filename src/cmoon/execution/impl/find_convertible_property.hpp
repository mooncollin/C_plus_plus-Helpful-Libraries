#pragma once

#include <type_traits>

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class Property, class... Properties>
			struct find_convertible_property;

			template<class Property, class Prop1, class... PropN>
			struct find_convertible_property<Property, Prop1, PropN...> : public std::conditional_t<std::is_convertible_v<Property, Prop1>,
																										std::type_identity<Prop1>,
																										find_convertible_property<Property, PropN...>> {};
		}
	}
}