export module cmoon.iterator.reverse;

import <ranges>;

namespace cmoon
{
	export
	template<std::ranges::range T>
		requires(requires(T t) { std::rbegin(t); std::rend(t); })
	class reverse
	{
		public:
			constexpr explicit reverse(T& iterable)
				: iterable_{iterable} {}

			[[nodiscard]] constexpr auto begin() const
			{
				return std::rbegin(iterable_);
			}

			[[nodiscard]] constexpr auto end() const
			{
				return std::rend(iterable_);
			}

			[[nodiscard]] constexpr auto rbegin() const
			{
				return std::begin(iterable_);
			}

			[[nodiscard]] constexpr auto rend() const
			{
				return std::end(iterable_);
			}
		private:
			T& iterable_;
	};
}