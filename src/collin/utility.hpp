#pragma once

#include <iterator>
#include <exception>
#include <utility>
#include <array>
#include <tuple>
#include <system_error>
#include <type_traits>
#include <limits>

namespace collin
{
	template<class Function, class... Args>
	class finally
	{
		public:
			using function_type = Function;
			using return_type = std::invoke_result_t<function_type(Args...)>;
			using arguments = std::tuple<Args...>;

			finally(function_type&& f, Args&&... args) noexcept
				: f(f), args(std::forward<Args>(args...)) {}

			~finally()
			{
				operator()();
			}

			return_type operator()() noexcept
			{
				return std::apply(f, args);
			}

			[[nodiscard]] const function_type& function() const noexcept
			{
				return f;
			}

		private:
			function_type f;
			std::tuple<Args...> args;
	};

	template<class Container>
	constexpr auto& last_of(Container& container, typename Container::size_type n = 1) noexcept(noexcept(*(std::end(container) - n)))
	{
		return *(std::end(container) - n);
	}

	template<class Container>
	constexpr auto& first_of(Container& container, typename Container::size_type n = 0) noexcept(noexcept(*(std::begin(container) + n)))
	{
		return *(std::begin(container) + n);
	}

	template<class Container, class... Args>
	constexpr auto construct_value(Args&&... args) noexcept(std::is_nothrow_constructible_v<typename Container::value_type, Args...>)
	{
		return typename Container::value_type {std::forward<Args...>(args)...};
	}

	template<class Container, class... Args>
	constexpr auto construct_value(const Container&, Args&&... args) noexcept(noexcept(construct_value<Container>(args...)))
	{
		return construct_value<Container>(args...);
	}

	template<class Array, std::size_t... I>
	constexpr auto a2t_impl(const Array& a, std::index_sequence<I...>) noexcept(noexcept(std::make_tuple(a[I]...)))
	{
		return std::make_tuple(a[I]...);
	}

	template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
	constexpr auto array_to_tuple(const std::array<T, N>& a) noexcept(noexcept(a2t_impl(a, Indices{})))
	{
		return a2t_impl(a, Indices{});
	}

	namespace details
	{
		template<std::size_t... I>
		constexpr auto index_array_helper(std::index_sequence<I...>) noexcept(noexcept(std::array<std::size_t, sizeof...(I)>{I...}))
		{
			return std::array<std::size_t, sizeof...(I)>{I...};
		}
	}

	template<std::size_t N>
	constexpr auto index_array() noexcept(noexcept(details::index_array_helper(std::make_index_sequence<N>{})))
	{
		return details::index_array_helper(std::make_index_sequence<N>{});
	}

	namespace details
	{
		template<class T, std::size_t... I>
		constexpr std::array<T, sizeof...(I)> initializer_list_to_array_helper(std::initializer_list<T> values, std::index_sequence<I...>) noexcept(noexcept(std::array<T, sizeof...(I)>{{*(std::begin(values) + I)...}}))
		{
			return {{*(std::begin(values) + I)...}};
		}
	}

	template<std::size_t Size, class T>
	constexpr std::array<T, Size> initializer_list_to_array(std::initializer_list<T> values) noexcept(noexcept(details::initializer_list_to_array_helper(values, std::make_index_sequence<Size>{})))
	{
		return details::initializer_list_to_array_helper(values, std::make_index_sequence<Size>{});
	}

	inline std::size_t unaligned_load(const char* p) noexcept
	{
		std::size_t result;
		std::memcpy(&result, p, sizeof(result));
		return result;
	}

	std::size_t hash_bytes(const void* ptr, std::size_t len, std::size_t seed) noexcept
	{
		constexpr std::size_t m = 0x5bd1e995;
		std::size_t hash = seed ^ len;
		auto buf = static_cast<const char*>(ptr);

		while (len >= 4)
		{
			std::size_t k = unaligned_load(buf);
			k *= m;
			k ^= k >> 24;
			k *= m;
			hash *= m;
			hash ^= k;
			buf += 4;
			len -= 4;
		}

		switch (len)
		{
			case 3:
				hash ^= static_cast<std::size_t>(buf[2]) << 16;
				[[fallthrough]];
			case 2:
				hash ^= static_cast<std::size_t>(buf[1]) << 8;
				[[fallthrough]];
			case 1:
				hash ^= static_cast<std::size_t>(buf[0]);
				hash *= m;
		}

		hash ^= hash >> 13;
		hash *= m;
		hash ^= hash >> 15;
		return hash;
	}

	struct throw_on_error
	{
		explicit throw_on_error(std::string_view msg)
			: msg(msg) {}

		~throw_on_error() noexcept(false)
		{
			if (ec)
			{
				throw std::system_error(ec, msg.c_str());
			}
		}

		throw_on_error(const throw_on_error&) = delete;
		throw_on_error& operator=(const throw_on_error&) = delete;

		throw_on_error(throw_on_error&&) noexcept = default;
		throw_on_error& operator=(throw_on_error&&) noexcept = default;

		operator std::error_code& () noexcept
		{
			return ec;
		}

		std::string msg;
		std::error_code ec;
	};

	namespace details
	{
		template<class Function, std::size_t... I, class... Args>
		constexpr std::invoke_result_t<Function, Args...> invoke(Function&& f, std::tuple<Args...>&& tup, std::index_sequence<I...>) noexcept(noexcept(std::forward<Function>(f)(std::forward<Args>(std::get<I>(tup))...)))
		{
			return std::forward<Function>(f)(std::forward<Args>(std::get<I>(tup))...);
		}
	}

	template<class Function, class... Args>
	std::invoke_result_t<Function, Args...> invoke(Function&& f, std::tuple<Args...>&& tup) noexcept(noexcept(details::invoke(std::forward<Function>(f), std::forward<std::tuple<Args...>>(tup), std::make_index_sequence<sizeof...(Args)>{})))
	{
		return details::invoke(std::forward<Function>(f), std::forward<std::tuple<Args...>>(tup), std::make_index_sequence<sizeof...(Args)>{});
	}

	template<class T>
	constexpr std::decay_t<T> decay_copy(T&& v) noexcept
	{
		return std::forward<T>(v);
	}

	template<class... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;

	template<class Required, std::size_t Min, std::size_t Max, class... T>
	struct basic_static_initializer_list : public std::bool_constant<
				 sizeof...(T) >= Min &&
				 sizeof...(T) <= Max &&
				 (... && std::is_convertible_v<T, Required>)> {};

	template<class Required, class... T>
	using unbounded_static_initializer_list = basic_static_initializer_list<Required, 0, std::numeric_limits<std::size_t>::max(), T...>;

	template<class Required, std::size_t Min, class... T>
	using minimum_static_initializer_list = basic_static_initializer_list<Required, Min, std::numeric_limits<std::size_t>::max(), T...>;

	template<class Required, std::size_t Max, class... T>
	using maximum_static_initializer_list = basic_static_initializer_list<Required, 0, Max, T...>;
}