#ifndef COLLIN_UTILITY
#define COLLIN_UTILITY

#include <iterator>
#include <exception>
#include <utility>
#include <array>
#include <tuple>

namespace collin
{
	template<class Function, class... Args>
	class finally
	{
		public:
			using return_type = std::result_of_t<Function(Args...)>;

			finally(Function&& f, Args&&... args) noexcept
				: f(f), args(args...) {}

			~finally() noexcept
			{
				static_cast<void>(operator()());
			}

			return_type operator()() const noexcept
			{
				return std::apply(f, args);
			}

			Function function() const noexcept
			{
				return f;
			}

		private:
			Function f;
			std::tuple<Args...> args;
	};

	template<class Container>
	constexpr auto& last_of(Container& container, typename Container::size_type n = 1)
	{
		return *(std::end(container) - n);
	}

	template<class Container>
	constexpr auto& first_of(Container& container, typename Container::size_type n = 1)
	{
		return *(std::begin(container) + n);
	}

	template<class Container, class... Args>
	constexpr auto construct_value(Args&&... args)
	{
		return typename Container::value_type {std::forward<Args...>(args)...};
	}

	template<class Container, class... Args>
	constexpr auto construct_value(const Container&, Args&&... args)
	{
		return construct_value<Container>(args...);
	}

	template<class Array, std::size_t... I>
	constexpr auto a2t_impl(const Array& a, std::index_sequence<I...>)
	{
		return std::make_tuple(a[I]...);
	}

	template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
	constexpr auto array_to_tuple(const std::array<T, N>& a)
	{
		return a2t_impl(a, Indices{});
	}

	template<class... Types>
	struct parameter_count
	{
		static constexpr std::size_t value {sizeof...(Types)};	
	};

	template<class... Types>
	constexpr auto parameter_count_v = parameter_count<Types...>::value;
}

#endif