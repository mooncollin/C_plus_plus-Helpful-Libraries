#pragma once

#include <type_traits>
#include <ratio>
#include <cstdint>

#include "cmoon/math.hpp"

namespace cmoon
{
	template<class T>
	struct is_ratio : std::false_type {};

	template<std::intmax_t N, std::intmax_t D>
	struct is_ratio<std::ratio<N, D>> : std::true_type {};

	template<class T>
	constexpr bool is_ratio_v = is_ratio<T>::value;

	template<class T>
	concept ratio_type = is_ratio_v<T>;

	template<ratio_type...>
	struct ratio_add_many;

	template<ratio_type R1>
	struct ratio_add_many<R1>
	{
		using type = R1;
	};

	template<ratio_type R1, ratio_type R2>
	struct ratio_add_many<R1, R2>
	{
		using type = std::ratio_add<R1, R2>;
	};

	template<ratio_type R1, ratio_type R2, ratio_type... RR>
	struct ratio_add_many<R1, R2, RR...>
	{
		using type = std::ratio_add<R1, typename ratio_add_many<R2, RR...>::type>;
	};

	template<ratio_type... RR>
	using ratio_add_many_t = typename ratio_add_many<RR...>::type;

	template<ratio_type...>
	struct ratio_subtract_many;

	template<ratio_type R1>
	struct ratio_subtract_many<R1>
	{
		using type = R1;
	};

	template<ratio_type R1, ratio_type R2>
	struct ratio_subtract_many<R1, R2>
	{
		using type = std::ratio_subtract<R1, R2>;
	};

	template<ratio_type R1, ratio_type R2, ratio_type... RR>
	struct ratio_subtract_many<R1, R2, RR...>
	{
		using type = std::ratio_subtract<R1, typename ratio_subtract_many<R2, RR...>::type>;
	};

	template<ratio_type... RR>
	using ratio_subtract_many_t = typename ratio_subtract_many<RR...>::type;

	template<ratio_type...>
	struct ratio_multiply_many;

	template<ratio_type R1>
	struct ratio_multiply_many<R1>
	{
		using type = R1;
	};

	template<ratio_type R1, ratio_type R2>
	struct ratio_multiply_many<R1, R2>
	{
		using type = std::ratio_multiply<R1, R2>;
	};

	template<ratio_type R1, ratio_type R2, ratio_type... RR>
	struct ratio_multiply_many<R1, R2, RR...>
	{
		using type = std::ratio_multiply<R1, typename ratio_multiply_many<R2, RR...>::type>;
	};

	template<ratio_type... RR>
	using ratio_multiply_many_t = typename ratio_multiply_many<RR...>::type;

	template<ratio_type...>
	struct ratio_divide_many;

	template<ratio_type R1>
	struct ratio_divide_many<R1>
	{
		using type = R1;
	};

	template<ratio_type R1, ratio_type R2>
	struct ratio_divide_many<R1, R2>
	{
		using type = std::ratio_divide<R1, R2>;
	};

	template<ratio_type R1, ratio_type R2, ratio_type... RR>
	struct ratio_divide_many<R1, R2, RR...>
	{
		using type = std::ratio_divide<R1, typename ratio_divide_many<R2, RR...>::type>;
	};

	template<ratio_type... RR>
	using ratio_divide_many_t = typename ratio_divide_many<RR...>::type;

	template<ratio_type Ratio>
	using ratio_reciprocal = std::ratio<Ratio::den, Ratio::num>;

	template<ratio_type Ratio>
	using ratio_additive_inverse = std::ratio<-Ratio::num, Ratio::den>;

	template<ratio_type Ratio>
	using ratio_canonical = std::ratio<Ratio::num / cmoon::static_gcd<Ratio::num, Ratio::den>::value,
									   Ratio::den / cmoon::static_gcd<Ratio::num, Ratio::den>::value
									   >;
}