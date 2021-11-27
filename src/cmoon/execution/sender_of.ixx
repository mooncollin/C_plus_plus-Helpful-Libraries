export module cmoon.execution.sender_of;

import <concepts>;
import <type_traits>;

import cmoon.meta;

import cmoon.execution.typed_sender;

namespace cmoon::execution
{
	export
	template<class S, class... Ts>
	concept sender_of = typed_sender<S> &&
						std::same_as<cmoon::meta::type_list<Ts...>,
									 value_types_of_t<S, cmoon::meta::type_list, std::type_identity_t>>;
}