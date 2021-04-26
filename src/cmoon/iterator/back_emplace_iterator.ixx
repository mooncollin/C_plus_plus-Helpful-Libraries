export module cmoon.iterator.back_emplace_iterator;

import <iterator>;
import <utility>;
import <functional>;

namespace cmoon
{
	template<class Container>
	class back_emplace_iterator
	{
		public:
			using difference_type = typename Container::difference_type;
			using value_type = typename Container::value_type;
			using pointer = typename Container::pointer;
			using reference = typename Container::reference;
			using iterator_category = std::output_iterator_tag;

			constexpr explicit back_emplace_iterator(Container& x) noexcept
				: container{x} {}

			template<class... Args>
			constexpr back_emplace_iterator& operator=(Args&&... args)
			{
				container.get().emplace_back(std::forward<Args>(args)...);
				return *this;
			}

			constexpr back_emplace_iterator& operator*()
			{
				return *this;
			}

			constexpr back_emplace_iterator& operator++()
			{
				return *this;
			}

			constexpr back_emplace_iterator& operator++(int)
			{
				return *this;
			}
		private:
			std::reference_wrapper<Container> container;
	};

	template<class Container>
	[[nodiscard]] constexpr back_emplace_iterator<Container> back_emplacer(Container& c) noexcept
	{
		return back_emplace_iterator<Container>{c};
	}
}