export module cmoon.execution.typed_sender;

import <type_traits>;

import cmoon.execution.sender;
import cmoon.execution.sender_traits;
import cmoon.execution.impl.has_sender_types;

namespace cmoon::execution
{
	export
	template<class S>
	concept typed_sender = sender<S> &&
						   has_sender_types<sender_traits<std::remove_cvref_t<S>>>;
}