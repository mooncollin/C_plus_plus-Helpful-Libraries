export module cmoon.type_traits.function_traits;
import <functional>;

namespace cmoon
{
	export
	template<typename T>
	struct function_traits;

	export
	template<typename R, typename... Args>
	struct function_traits<std::function<R(Args...)>>
	{
		static constexpr auto nargs = sizeof...(Args);
		using result_type = R;

		template<std::size_t i>
		struct arg
		{
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};
}