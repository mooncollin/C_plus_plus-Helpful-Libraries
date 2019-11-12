#ifndef COLLIN_STREAM
#define COLLIN_STREAM

#include <list>
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <algorithm>

namespace collin
{
	template<class T>
	class stream
	{
		public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const pointer;

			using iterator = typename std::list<T>::iterator;
			using const_iterator = typename std::list<T>::const_iterator;

			template<class C>
			static stream<C> concat(stream<C>& stream1, stream<C>& stream2)
			{
				std::list<C> elements;
				elements.insert(elements.end(), stream1.current(), stream1.end());
				elements.insert(elements.end(), stream2.current(), stream2.end());

				return stream(std::move(elements));
			}

			stream(std::list<T>&& container={})
				: _container(std::move(container))
			{
			}

			template<class Container>
			stream(const Container& container)
				: _container(std::cbegin(container), std::cend(container))
			{
			}

			template<class Container>
			stream(Container&& container)
				: _container(std::begin(container), std::end(container))
			{
			}

			stream(stream&&) = default;
			stream(const stream&) = default;
			stream& operator=(const stream&) = default;

			iterator begin() const
			{
				return std::begin(_container);
			}

			const_iterator cbegin() const
			{
				return std::cbegin(_container);
			}

			const_iterator cend() const
			{
				return std::cend(_container);
			}

			iterator end() const
			{
				return std::end(_container);
			}

			stream& operator++()
			{
				_container.pop_front();
				return *this;
			}

			stream& operator++(int)
			{
				stream s = *this;
				_container.pop_front();
				return s;
			}

			reference operator*()
			{
				return _container.front();
			}

			pointer operator->()
			{
				return _container.front().operator->();
			}

			template<class UnaryPredicate>
			bool all_match(UnaryPredicate p)
			{
				while(!empty())
				{
					if(!p(operator*()))
					{
						operator++();
						return false;
					}

					operator++();
				}

				return true;
			}

			template<class UnaryPredicate>
			bool any_match(UnaryPredicate p)
			{
				while(!empty())
				{
					if(p(operator*()))
					{
						operator++();
						return true;
					}

					operator++();
				}

				return false;
			}

			template<class UnaryPredicate>
			bool none_match(UnaryPredicate p)
			{
				while(!empty())
				{
					if(p(operator*()))
					{
						operator++();
						return false;
					}

					operator++();
				}

				return true;
			}

			stream<T>& skip(std::size_t n)
			{
				for(std::size_t i = 0; i < n; i++)
				{
					operator++();
				}

				return *this;
			}

			stream<T>& sorted()
			{
				_container.sort();
				return *this;
			}

			template<class Comparator>
			stream<T>& sorted(Comparator comp)
			{
				_container.sort(comp);

				return *this;
			}

			template<class Other_Collection>
			Other_Collection collect()
			{
				Other_Collection c {};
				auto inserter = std::inserter(c, std::end(c));

				while(!empty())
				{
					*inserter = std::move(operator*());
					inserter++;
					operator++();
				}

				return c;
			}

			bool empty() const
			{
				return _container.empty();
			}

			auto count()
			{
				const auto d = std::distance(begin(), end());
				return d;
			}

			template<class Consumer>
			void for_each(Consumer c)
			{
				while(!empty())
				{
					c(std::move(operator*()));
					operator++();
				}
			}

			template<class Function>
			auto map(Function func)
			{
				using R = decltype(func(std::declval<T>()));
				if constexpr(std::is_same_v<R, T>)
				{
					std::transform(std::begin(_container), std::end(_container), std::begin(_container), func);
					return *this;
				}

				std::list<R> mapping;

				while(!empty())
				{
					mapping.push_back(std::move(func(std::move(operator*()))));
					operator++();
				}

				stream<R> s(std::move(mapping));

				return s;
			}

			template<class UnaryPredicate>
			stream<T>& dropWhile(UnaryPredicate pred)
			{
				while(!empty() && pred(operator*()))
				{
					operator++();
				}

				return *this;
			}

			template<class UnaryPredicate>
			stream<T> takeWhile(UnaryPredicate pred)
			{
				std::list<T> elements;

				while(!empty() && pred(operator*()))
				{
					elements.push_back(std::move(operator*()));
					elements++();
				}

				_container = std::move(elements);

				return *this;;
			}

			template<class UnaryPredicate>
			stream<T>& filter(UnaryPredicate pred)
			{
				std::list<T> left;

				while(!empty())
				{
					if(pred(operator*()))
					{
						left.push_back(std::move(operator*()));
					}

					operator++();
				}

				_container = std::move(left);

				return *this;
			}

			stream<T>& limit(std::size_t max_size)
			{
				std::list<T> elements;

				for(std::size_t i = 0; i < max_size; i++)
				{
					elements.push_back(std::move(operator*()));
					operator++();
				}

				_container = std::move(elements);

				return *this;
			}
		private:
			std::list<T> _container;
	};

	template<class T>
	static stream<T> to_stream(std::initializer_list<T> items)
	{
		return stream<T>(items);
	}

	template<class Container>
	static stream<typename Container::value_type> to_stream(const Container& c)
	{
		return stream<typename Container::value_type>(c);
	}

	template<class Container>
	static stream<typename Container::value_type> to_stream(Container&& c)
	{
		return stream<typename Container::value_type>(std::move(c));
	}
}

#endif