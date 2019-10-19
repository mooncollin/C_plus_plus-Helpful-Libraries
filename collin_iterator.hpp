#ifndef COLLIN_ITERATOR
#define COLLIN_ITERATOR

#include <iterator>

namespace collin
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
				container->emplace_back(std::forward<Args>(args));
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
}

#endif