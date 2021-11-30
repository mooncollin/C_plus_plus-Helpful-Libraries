export module cmoon.type_traits.detector;

import <type_traits>;

namespace cmoon
{
	export
	struct nonesuch
	{
		~nonesuch() = delete;
		nonesuch(const nonesuch&) = delete;
		nonesuch& operator=(const nonesuch&) = delete;
	};

	template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
	struct detector
	{
		using value_t = std::false_type;
		using type = Default;
	};

	template<class Default, template<class...> class Op, class... Args>
	struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
	{
		using value_t = std::true_type;
		using type = Op<Args...>;
	};

	export
	template<template<class...> class Op, class... Args>
	using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

	export
	template<template<class...> class Op, class... Args>
	using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

	export
	template<class Default, template<class...> class Op, class... Args>
	using detected_or = detector<Default, void, Op, Args...>;

	export
	template<template<class...> class Op, class... Args>
	constexpr inline auto is_detected_v = is_detected<Op, Args...>::value;

	export
	template<class Default, template<class...> class Op, class... Args>
	using detected_or_t = typename detected_or<Default, Op, Args...>::type;

	export
	template<class Expected, template<class...> class Op, class... Args>
	using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

	export
	template<class Expected, template<class...> class Op, class... Args>
	constexpr inline auto is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

	export
	template<class To, template<class...> class Op, class... Args>
	using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

	export
	template<class To, template<class...> class Op, class... Args>
	constexpr inline auto is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;
}