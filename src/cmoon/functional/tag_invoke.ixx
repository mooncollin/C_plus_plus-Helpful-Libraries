export module cmoon.functional.tag_invoke;

import <type_traits>;
import <concepts>;

import cmoon.utility;

namespace cmoon
{
	namespace tag_invoke_ns
	{
		void tag_invoke();

		struct tag_invoke_t
		{
			template<typename Tag, typename... Args>
				requires(requires(Tag tag, Args&&... args) {
					tag_invoke(cmoon::decay_copy(tag), std::forward<Args>(args)...);
				})
			constexpr decltype(auto) operator()(Tag tag, Args&&... args) const noexcept(noexcept(tag_invoke(cmoon::decay_copy(std::declval<Tag>()), std::forward<Args>(std::declval<Args>())...)))
			{
				return tag_invoke(cmoon::decay_copy(tag), std::forward<Args>(args)...);
			}
		};
	}

	export
	inline constexpr tag_invoke_ns::tag_invoke_t tag_invoke {};

	export
	template<auto& Tag>
	using tag_t = std::decay_t<decltype(Tag)>;

	export
	template<class Tag, class... Args>
	concept tag_invocable = std::invocable<decltype(tag_invoke), Tag, Args...>;

	export
	template<class Tag, class... Args>
	concept nothrow_tag_invocable = tag_invocable<Tag, Args...> &&
									std::is_nothrow_invocable_v<decltype(tag_invoke), Tag, Args...>;


	export
	template<class Tag, class... Args>
	using tag_invoke_result = std::invoke_result<decltype(tag_invoke), Tag, Args...>;

	export
	template<class Tag, class... Args>
	using tag_invoke_result_t = typename tag_invoke_result<Tag, Args...>::type;

	export
	template<class T, class Tag, class... Args>
	concept tag_invocable_with_result = tag_invocable<Tag, Args...> &&
										std::same_as<tag_invoke_result_t<Tag, Args...>, T>;

	export
	template<class T, class Tag, class... Args>
	concept nothrow_tag_invocable_with_result = nothrow_tag_invocable<Tag, Args...> &&
												std::same_as<tag_invoke_result_t<Tag, Args...>, T>;
}