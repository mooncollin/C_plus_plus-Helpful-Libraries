#ifndef COLLIN_UTILITY
#define COLLIN_UTILITY

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

			~finally() noexcept
			{
				operator()();
			}

			return_type operator()() const noexcept
			{
				return std::apply(f, args);
			}

			function_type function() const noexcept
			{
				return f;
			}

		private:
			function_type f;
			std::tuple<Args...> args;
	};

	template<class Container>
	constexpr auto& last_of(Container& container, typename Container::size_type n = 1)
	{
		return *(std::end(container) - n);
	}

	template<class Container>
	constexpr auto& first_of(Container& container, typename Container::size_type n = 0)
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

	inline std::size_t unaligned_load(const char* p)
	{
		std::size_t result;
		std::memcpy(&result, p, sizeof(result));
		return result;
	}

	std::size_t hash_bytes(const void* ptr, std::size_t len, std::size_t seed)
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
				hash ^= static_cast<unsigned char>(buf[2]) << 16;
			case 2:
				hash ^= static_cast<unsigned char>(buf[1]) << 8;
			case 1:
				hash ^= static_cast<unsigned char>(buf[0]);
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
		constexpr std::invoke_result_t<Function, Args...> invoke(Function&& f, std::tuple<Args...>&& tup, std::index_sequence<I...>)
		{
			return std::forward<Function>(f)(std::forward<Args>(std::get<I>(tup))...);
		}
	}

	template<class Function, class... Args>
	std::invoke_result_t<Function, Args...> invoke(Function&& f, std::tuple<Args...>&& tup)
	{
		return details::invoke_helper(std::forward<Function>(f), std::forward<std::tuple<Args...>>(tup), std::make_index_sequence<sizeof...(Args)>{});
	}

	template<class... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;
}

#endif