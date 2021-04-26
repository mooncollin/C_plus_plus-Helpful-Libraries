export module cmoon.type_traits.conditional_value;

namespace cmoon
{
	export
	template<bool B, class T, class F, T t, F f>
	struct conditional_value;

	export
	template<class T, class F, T t, F f>
	struct conditional_value<true, T, F, t, f>
	{
		static constexpr auto value = t;
	};

	export
	template<class T, class F, T t, F f>
	struct conditional_value<false, T, F, t, f>
	{
		static constexpr auto value = f;
	};

	export
	template<class T, T t, T t2>
	struct conditional_value<true, T, T, t, t2>
	{
		static constexpr auto value = t;
	};

	export
	template<class T, T t, T t2>
	struct conditional_value<false, T, T, t, t2>
	{
		static constexpr auto value = t2;
	};

	export
	template<bool B, class T, class F, T t, F f>
	constexpr auto conditional_value_v = conditional_value<B, T, F, t, f>::value;

	export
	template<bool B, class T, T t, T t2>
	constexpr auto conditional_value2_v = conditional_value<B, T, T, t, t2>::value;
}