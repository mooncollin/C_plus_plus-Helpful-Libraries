module;
#include <iterator>
#include <exception>
#include <utility>
#include <array>
#include <tuple>
#include <system_error>
#include <type_traits>
#include <limits>
#include <span>
#include <chrono>
#include <iostream>

export module cmoon.utility;

namespace cmoon
{
	export
	template<class Container>
	constexpr auto& last_of(Container& container, typename Container::size_type n = 1)
	{
		auto end = std::end(container);
		std::advance(end, -n);
		return *end;
	}

	export
	template<class Container>
	constexpr const auto& last_of(const Container& container, typename Container::size_type n = 1)
	{
		auto end = std::end(container);
		std::advance(end, -n);
		return *end;
	}

	export
	template<class Container>
	constexpr auto& first_of(Container& container, typename Container::size_type n = 0)
	{
		auto begin = std::begin(container);
		std::advance(begin, n);
		return *begin;
	}

	export
	template<class Container>
	constexpr const auto& first_of(const Container& container, typename Container::size_type n = 0)
	{
		auto begin = std::begin(container);
		std::advance(begin, n);
		return *begin;
	}

	export
	template<class Container, class... Args>
	constexpr auto construct_value(Args&&... args) noexcept(std::is_nothrow_constructible_v<typename Container::value_type, Args...>)
	{
		return typename Container::value_type {std::forward<Args...>(args)...};
	}

	export
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

	export
	template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
	constexpr auto array_to_tuple(const std::array<T, N>& a) noexcept(noexcept(a2t_impl(a, Indices{})))
	{
		return a2t_impl(a, Indices{});
	}

	template<std::size_t... I>
	constexpr auto index_array_helper(std::index_sequence<I...>) noexcept(noexcept(std::array<std::size_t, sizeof...(I)>{I...}))
	{
		return std::array<std::size_t, sizeof...(I)>{I...};
	}

	export
	template<std::size_t N>
	constexpr auto index_array() noexcept(noexcept(index_array_helper(std::make_index_sequence<N>{})))
	{
		return index_array_helper(std::make_index_sequence<N>{});
	}

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

	export
	template<class T>
	constexpr std::decay_t<T> decay_copy(T&& v) noexcept(noexcept(static_cast<std::decay_t<T>>(std::forward<T>(v))))
	{
		return std::forward<T>(v);
	}

	

	export
	template<class Required, std::size_t Min, std::size_t Max, class... T>
	struct basic_static_initializer_list : public std::bool_constant<
				 sizeof...(T) >= Min &&
				 sizeof...(T) <= Max &&
				 (... && std::is_convertible_v<T, Required>)> {};

	export
	template<class Required, class... T>
	using unbounded_static_initializer_list = basic_static_initializer_list<Required, 0, std::numeric_limits<std::size_t>::max(), T...>;

	export
	template<class Required, std::size_t Min, class... T>
	using minimum_static_initializer_list = basic_static_initializer_list<Required, Min, std::numeric_limits<std::size_t>::max(), T...>;

	export
	template<class Required, std::size_t Max, class... T>
	using maximum_static_initializer_list = basic_static_initializer_list<Required, 0, Max, T...>;

	export
	template<std::size_t Ext>
	class extent_type
	{
		public:
			using size_type = std::size_t;

			constexpr extent_type() noexcept = default;

			template<size_type Other>
			constexpr extent_type(extent_type<Other> ext) noexcept
			{
				static_assert(Other == Ext);
			}

			constexpr extent_type(size_type) noexcept
			{
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return Ext;
			}

			constexpr void swap(extent_type& other) noexcept {}
	};

	export
	template<>
	class extent_type<std::dynamic_extent>
	{
		public:
			using size_type = std::size_t;

			template<size_type Other>
			explicit constexpr extent_type(extent_type<Other> ext) noexcept : size_(ext.size()) {}
			explicit constexpr extent_type(size_type size) noexcept : size_(size) {}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return size_;
			}

			constexpr void swap(extent_type& other) noexcept
			{
				std::swap(size_, other.size_);
			}
		private:
			size_type size_;
	};

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