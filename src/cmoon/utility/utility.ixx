export module cmoon.utility;
export import cmoon.utility.owner;
export import cmoon.utility.not_null;
export import cmoon.utility.throw_on_error;
export import cmoon.utility.extent_type;
export import cmoon.utility.expected;
export import cmoon.utility.stopwatch;
export import cmoon.utility.as_unsigned;

import <iterator>;
import <utility>;
import <array>;
import <tuple>;
import <type_traits>;
import <limits>;
import <ranges>;

namespace cmoon
{
	export
	template<std::ranges::range R>
	constexpr decltype(auto) last_of(R&& r, std::ranges::range_size_t<R> n = 1)
	{
		auto end = std::ranges::end(r);
		std::advance(end, -n);
		return *end;
	}

	export
	template<std::ranges::range R>
	constexpr decltype(auto) first_of(R&& r, std::ranges::range_size_t<R> n = 0)
	{
		auto begin = std::begin(r);
		std::advance(begin, n);
		return *begin;
	}

	export
	template<class Container, class... Args>
		requires(requires { typename Container::value_type; })
	constexpr auto construct_value(Args&&... args) noexcept(std::is_nothrow_constructible_v<typename Container::value_type, Args...>)
	{
		return typename Container::value_type {std::forward<Args...>(args)...};
	}

	export
	template<class Container, class... Args>
		requires(requires { typename Container::value_type; })
	constexpr auto construct_value(const Container&, Args&&... args) noexcept(noexcept(construct_value<Container>(args...)))
	{
		return construct_value<Container>(args...);
	}

	//template<class Array, std::size_t... I>
	//constexpr auto a2t_impl(const Array& a, std::index_sequence<I...>) noexcept(noexcept(std::make_tuple(a[I]...)))
	//{
	//	return std::make_tuple(a[I]...);
	//}

	//export
	//template<typename T, std::size_t N>
	//constexpr auto array_to_tuple(const std::array<T, N>& a) noexcept(noexcept(a2t_impl(a, Indices{})))
	//{
	//	return a2t_impl(a, std::make_index_sequence<N>{});
	//}

	//template<std::size_t... I>
	//constexpr auto index_array_helper(std::index_sequence<I...>) noexcept(noexcept(std::array<std::size_t, sizeof...(I)>{I...}))
	//{
	//	return std::array<std::size_t, sizeof...(I)>{I...};
	//}

	//export
	//template<std::size_t N>
	//constexpr auto index_array() noexcept(noexcept(index_array_helper(std::make_index_sequence<N>{})))
	//{
	//	return index_array_helper(std::make_index_sequence<N>{});
	//}

	template<class T, std::size_t... I>
	constexpr std::array<T, sizeof...(I)> initializer_list_to_array_helper(std::initializer_list<T> values, std::index_sequence<I...>) noexcept(noexcept(std::array<T, sizeof...(I)>{{*(std::begin(values) + I)...}}))
	{
		return {{*(std::begin(values) + I)...}};
	}

	export
	template<std::size_t Size, class T>
	constexpr std::array<T, Size> initializer_list_to_array(std::initializer_list<T> values) noexcept(noexcept(initializer_list_to_array_helper(values, std::make_index_sequence<Size>{})))
	{
		return initializer_list_to_array_helper(values, std::make_index_sequence<Size>{});
	}

	export
	inline std::size_t unaligned_load(const char* p) noexcept
	{
		std::size_t result;
		std::memcpy(&result, p, sizeof(result));
		return result;
	}

	export
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

	export
	template<class T>
	constexpr std::decay_t<T> decay_copy(T&& v) noexcept(noexcept(static_cast<std::decay_t<T>>(std::forward<T>(v))))
	{
		return static_cast<std::decay_t<T>>(std::forward<T>(v));
	}

	template<class T>
	constexpr auto choose_on_type_helper() noexcept
	{
		static_assert(false, "No argument matching the given type could be found");
	}

	template<class T, class First, class... Args>
	constexpr auto choose_on_type_helper([[maybe_unused]] First&& f, [[maybe_unused]] Args&&... args) noexcept
	{
		if constexpr (std::same_as<T, std::decay_t<First>>)
		{
			return std::forward<First>(f);
		}
		else
		{
			return choose_on_type_helper<T>(std::forward<Args>(args)...);
		}
	}

	export
	template<class T, class... Args>
	constexpr auto choose_on_type(Args&&... args) noexcept
	{
		return choose_on_type_helper<T>(std::forward<Args>(args)...);
	}
}