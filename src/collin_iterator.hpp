#pragma once

#include <iterator>
#include <tuple>

namespace collin
{
	namespace iterator
	{
		template<class Container>
		class back_emplace_iterator : public std::iterator<std::output_iterator_tag, typename Container::value_type, typename Container::difference_type, typename Container::pointer, typename Container::reference>
		{
			public:
				explicit back_emplace_iterator(Container& x)
					: container(&x)
				{
				}

				template<class... Args>
				back_emplace_iterator<Container>& operator=(Args&&... args)
				{
					container->emplace_back(std::forward<Args>(args)...);
					return *this;
				}

				back_emplace_iterator& operator*()
				{
					return *this;
				}

				back_emplace_iterator& operator++()
				{
					return *this;
				}

				back_emplace_iterator& operator++(int)
				{
					return *this;
				}
			private:
				Container* container;
		};

		template<class Container>
		back_emplace_iterator<Container> back_emplacer(Container& c)
		{
			return back_emplace_iterator<Container>(c);
		}

		template<class Container>
		class Enumerate : public std::iterator<std::forward_iterator_tag, typename Container::value_type, typename Container::difference_type, typename Container::pointer, typename Container::reference>
		{
			public:
				explicit Enumerate(Container& container)
					: Enumerate(container, std::begin(container))
				{
				}

				Enumerate(Container& container, int index)
					: Enumerate(container, std::begin(container), index)
				{
				}

				Enumerate(Container& container, typename Container::const_iterator place, int start=0)
					: container(container), it(place), index(start)
				{
				}

				Enumerate(const Enumerate& other) = default;
				Enumerate& operator=(const Enumerate& other) = default;

				Enumerate(Enumerate&&) = delete;
				Enumerate& operator=(Enumerate&&) = delete;


				Enumerate begin() const
				{
					return *this;
				}

				Enumerate end() const
				{
					return Enumerate(container, std::cend(container));
				}

				Enumerate& operator++()
				{
					++it;
					++index;

					return *this;
				}

				Enumerate operator++(int)
				{
					Enumerate e(*this);
					operator++();
					return e;
				}

				auto operator*() const
				{
					return std::make_tuple(index, it);
				}

				bool operator==(const Enumerate& rhs) const
				{
					return it == rhs.it;
				}

				bool operator!=(const Enumerate& rhs) const
				{
					return !operator==(rhs);
				}

			private:
				Container& container;
				typename Container::const_iterator it;
				int index;
		};
	}
}