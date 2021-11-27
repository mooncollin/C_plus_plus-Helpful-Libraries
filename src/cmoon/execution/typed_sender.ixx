export module cmoon.execution.typed_sender;

import <type_traits>;
import <tuple>;
import <variant>;

import cmoon.execution.sender;
import cmoon.execution.sender_traits;
import cmoon.execution.impl.has_sender_types;

namespace cmoon::execution
{
	export
	template<class S>
	concept typed_sender = sender<S> &&
						   has_sender_types<sender_traits<std::remove_cvref_t<S>>>;

	struct empty_variant
	{
		empty_variant() = delete;
	};

	template<class... Ts>
	struct variant_or_empty_impl : std::type_identity<std::variant<Ts...>> {};

	template<>
	struct variant_or_empty_impl<> : std::type_identity<empty_variant> {};

	template<class... Ts>
	using variant_or_empty = typename variant_or_empty_impl<Ts...>::type;

	export
	template<typed_sender S,
			 template<class...> class Tuple = std::tuple,
			 template<class...> class Variant = variant_or_empty>
	using value_types_of_t = typename sender_traits<std::remove_cvref_t<S>>::template value_types<Tuple, Variant>;

	export
	template<typed_sender S,
			 template<class...> class Variant = variant_or_empty>
	using error_types_of_t = typename sender_traits<std::remove_cvref_t<S>>::template error_types<Variant>;
}