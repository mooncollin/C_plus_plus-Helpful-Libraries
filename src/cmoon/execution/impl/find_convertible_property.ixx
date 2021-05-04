export module cmoon.execution.find_convertible_property;

import <type_traits>;
import <concepts>;

namespace cmoon::execution
{
	export
	template<class Property, class... Properties>
	struct find_convertible_property;

	export
	template<class Property, class Prop1, class... PropN>
	struct find_convertible_property<Property, Prop1, PropN...> : public std::conditional_t<std::convertible_to<Property, Prop1>,
																							std::type_identity<Prop1>,
																							find_convertible_property<Property, PropN...>> {};
}