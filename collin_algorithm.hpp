#ifndef COLLIN_ALGORITHM
#define COLLIN_ALGORITHM

#include <utility>
#include <algorithm>
#include <functional>

namespace collin
{
	template<class InputIterator, class Function>
	Function for_each_n(InputIterator first, InputIterator last,
		std::size_t n, Function fn)
	{
		for(decltype(n) i {}; i < n && first != last; i++)
		{
			fn(i, *first);
			first++;
		}

		return std::move(fn);
	}

	template<class Container, class Function>
	Function for_each_n(Container& container, std::size_t n, Function fn)
	{
		return for_each_n(std::begin(container), std::end(container), n, fn);
	}

	template<class Container, class Function>
	Function for_each(Container& container, Function fn)
	{
		return std::for_each(std::begin(container), std::end(container), fn);
	}

	template<class Container, class UnaryPredicate>
	bool any_of(const Container& container, UnaryPredicate pred)
	{
		return std::any_of(std::cbegin(container), std::cend(container), pred);
	}

	template<class Container, class UnaryPredicate>
	bool all_of(const Container& container, UnaryPredicate pred)
	{
		return std::all_of(std::cbegin(container), std::cend(container), pred);
	}

	template<class Container, class T>
	typename Container::const_iterator find(const Container& container, const T& val)
	{
		return std::find(std::cbegin(container), std::cend(container), val);
	}

	template<class Container>
	void sort(Container& container)
	{
		return std::sort(std::begin(container), std::end(container));
	}

	template<class Container, class Compare>
	void sort(Container& container, Compare comp)
	{
		return std::sort(std::begin(container), std::end(container), comp);
	}

	template<class Function, class T>
	T& apply(Function func, T& val)
	{
		func(val);
		return val;
	}

	template<class Function, class T>
	bool test(Function func, T& val)
	{
		return func(val);
	}

	template<class InputIterator, class T>
	T& apply_range(InputIterator first, InputIterator last, T& val)
	{
		const auto apply_func = std::bind(apply<typename InputIterator::value_type, T>, std::placeholders::_1, std::ref(val));
		std::for_each(first, last, apply_func);

		return val;
	}

	template<class Container, class T>
	T& apply_range(const Container& container, T& val)
	{
		return apply_range(std::cbegin(container), std::cend(container), val);
	}

	template<class Container, class OutputIterator, class Function>
	OutputIterator transform(const Container& container, OutputIterator out, Function fn)
	{
		return std::transform(std::cbegin(container), std::cend(container), out, fn);
	}

	template<class Container, class Container2, class Function>
	auto transform(const Container& container, Container2& container2, Function fn)
	{
		return transform(container, std::begin(container2), fn);
	}

	template<class Container, class Function>
	auto transform(Container& container, Function fn)
	{
		return transform(container, std::begin(container), fn);
	}

	template<class Container, class UnaryPredicate>
	auto remove_if(Container& container, UnaryPredicate fn)
	{
		return std::remove_if(std::begin(container), std::end(container), fn);
	}
}

#endif