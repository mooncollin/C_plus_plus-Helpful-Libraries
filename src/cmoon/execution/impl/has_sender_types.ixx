export module cmoon.execution.impl.has_sender_types;

import <type_traits>;

namespace cmoon::execution
{
	export
	template<template<template<class...> class, template<class...> class> class>
	struct has_value_types {};

	export
	template<template<template<class...> class> class>
	struct has_error_types {};

	export
	template<class S>
	concept has_sender_types =
		requires
	{
		typename has_value_types<S::template value_types>;
		typename has_error_types<S::template error_types>;
		typename std::bool_constant<S::sends_done>;
	};
}