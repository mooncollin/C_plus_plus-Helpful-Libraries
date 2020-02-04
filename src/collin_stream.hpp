#ifndef COLLIN_STREAM
#define COLLIN_STREAM

#include "collin_type_traits.hpp"
#include <list>
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <optional>

namespace collin
{
	template<class T, typename Implementation=std::list<T>>
	class stream
	{
		public:
			using impl = Implementation;
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const pointer;

			using iterator = typename impl::iterator;
			using const_iterator = typename impl::const_iterator;

			template<class C, typename Implementation1=std::list<C>>
			static stream<C> concat(const stream<C>& stream1, const stream<C>& stream2)
			{
				Implementation1 elements;
				elements.insert(elements.end(), stream1.current(), stream1.end());
				elements.insert(elements.end(), stream2.current(), stream2.end());

				return stream(std::move(elements));
			}

			template<class C, typename Implementation1=std::list<C>>
			static stream<C> concat(stream<C>&& stream1, stream<C>&& stream2)
			{
				Implementation1 elements(std::move(stream1._container));

				if constexpr(std::is_same_v<Implementation, std::list<C>>)
				{
					elements.splice(std::end(elements), stream2._container);
				}
				else
				{
					auto inserter = std::inserter(elements, std::end(elements));
					std::move(std::begin(stream2._container), std::end(stream2._container), inserter);
					stream2.clear();
				}

				return stream(std::move(elements));
			}

			stream(Implementation&& container={})
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
			stream& operator=(stream&&) = default;

			auto begin() const
			{
				return std::begin(_container);
			}

			auto cbegin() const
			{
				return std::cbegin(_container);
			}

			auto cend() const
			{
				return std::cend(_container);
			}

			auto end() const
			{
				return std::end(_container);
			}

			stream& operator++()
			{
				_container.erase(begin());
				return *this;
			}

			stream& operator++(int)
			{
				stream s = *this;
				_container.erase(begin());
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
				const auto result = std::all_of(cbegin(), cend(), p);
				clear();

				return result;
			}

			template<class UnaryPredicate>
			bool any_match(UnaryPredicate p)
			{
				const auto result = std::any_of(cbegin(), cend(), p);
				clear();

				return result;
			}

			template<class UnaryPredicate>
			bool none_match(UnaryPredicate p)
			{
				const auto result = std::none_of(cbegin(), cend(), p);
				clear();

				return true;
			}

			stream<T>& skip(std::size_t n)
			{
				auto end_iter = begin();
				std::advance(end_iter, n);
				_container.erase(begin(), end_iter);

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
				if constexpr(std::is_same_v<Other_Collection, Implementation>)
				{
					return std::move(_container);
				}
				else
				{
					Other_Collection c;
					auto insert = std::inserter(c, std::end(c));
					std::move(begin(), end(), insert);
					clear();

					return c;
				}
			}

			bool empty() const
			{
				return _container.empty();
			}

			auto count()
			{
				if constexpr(has_size_v<Implementation>)
				{
					return std::size(_container);
				}
				else
				{
					return std::distance(begin(), end());
				}
			}

			template<class Consumer>
			void for_each(Consumer c)
			{
				while(!empty())
				{
					c(operator*());
					operator++();
				}
			}

			template<class Function>
			auto map(Function func)
			{
				using R = decltype(func(std::declval<T>()));

				if constexpr(std::is_same_v<R, T>)
				{
					std::transform(begin(), end(), begin(), func);
					return *this;
				}
				else
				{
					std::list<R> mapping;

					auto inserter = std::inserter(mapping, std::end(mapping));

					std::transform(begin(), end(), inserter, func);

					stream<R> s(std::move(mapping));

					return s;
				}
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
				auto begin_it = begin();

				while(!empty() && pred(*begin_it))
				{
					begin_it++;
				}

				_container.erase(begin_it, end());

				return *this;
			}

			template<class UnaryPredicate>
			stream<T>& filter(UnaryPredicate pred)
			{
				_container.erase(std::remove_if(begin(), end(),
					[&](const auto& val) { return !pred(val); }), end());
				return *this;
			}

			stream<T>& limit(std::size_t max_size)
			{
				if(max_size < count())
				{
                    auto begin_iter = begin();
                    std::advance(begin_iter, max_size);
					_container.erase(begin_iter, end());
				}

				return *this;
			}

			std::optional<T> max()
			{
				if(empty())
				{
					return {};
				}

				const auto result = *std::max_element(cbegin(), cend());
				clear();

				return result;
			}

			template<class Comparator>
			std::optional<T> max(Comparator comp)
			{
				if(empty())
				{
					return {};
				}

				const auto result = *std::max_element(cbegin(), cend(), comp);
				clear();

				return result;
			}

			std::optional<T> min()
			{
				if(empty())
				{
					return {};
				}

				const auto result = *std::min_element(cbegin(), cend());
				clear();

				return result;
			}

			template<class Comparator>
			std::optional<T> min(Comparator comp)
			{
				if(empty())
				{
					return {};
				}

				const auto result = *std::min_element(cbegin(), cend(), comp);
				clear();

				return result;
			}

			stream<T>& clear()
			{
				_container.clear();
				return *this;
			}

		private:
			Implementation _container;
	};

	template<class T, typename Implementation = std::list<T>>
	static stream<T> to_stream(std::initializer_list<T> items)
	{
		return stream<T, Implementation>(items);
	}

	template<class Container, typename Implementation = std::list<typename Container::value_type>>
	static stream<typename Container::value_type> to_stream(Container c)
	{
		return stream<typename Container::value_type, Implementation>(std::move(c));
	}
}

#endif