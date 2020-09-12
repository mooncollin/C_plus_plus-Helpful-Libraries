#pragma once

#include <type_traits>
#include <ratio>
#include <cstdint>

#include "collin/math.hpp"

namespace collin
{
	namespace ratio
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
		struct add_many;

		template<ratio_type R1>
		struct add_many<R1>
		{
			using type = R1;
		};

		template<ratio_type R1, ratio_type R2>
		struct add_many<R1, R2>
		{
			using type = std::ratio_add<R1, R2>;
		};

		template<ratio_type R1, ratio_type R2, ratio_type... RR>
		struct add_many<R1, R2, RR...>
		{
			using type = std::ratio_add<R1, typename add_many<R2, RR...>::type>;
		};

		template<ratio_type... RR>
		using add_many_t = typename add_many<RR...>::type;

		template<ratio_type...>
		struct subtract_many;

		template<ratio_type R1>
		struct subtract_many<R1>
		{
			using type = R1;
		};

		template<ratio_type R1, ratio_type R2>
		struct subtract_many<R1, R2>
		{
			using type = std::ratio_subtract<R1, R2>;
		};

		template<ratio_type R1, ratio_type R2, ratio_type... RR>
		struct subtract_many<R1, R2, RR...>
		{
			using type = std::ratio_subtract<R1, typename subtract_many<R2, RR...>::type>;
		};

		template<ratio_type... RR>
		using subtract_many_t = typename subtract_many<RR...>::type;

		template<ratio_type...>
		struct multiply_many;

		template<ratio_type R1>
		struct multiply_many<R1>
		{
			using type = R1;
		};

		template<ratio_type R1, ratio_type R2>
		struct multiply_many<R1, R2>
		{
			using type = std::ratio_multiply<R1, R2>;
		};

		template<ratio_type R1, ratio_type R2, ratio_type... RR>
		struct multiply_many<R1, R2, RR...>
		{
			using type = std::ratio_multiply<R1, typename multiply_many<R2, RR...>::type>;
		};

		template<ratio_type... RR>
		using multiply_many_t = typename multiply_many<RR...>::type;

		template<ratio_type...>
		struct divide_many;

		template<ratio_type R1>
		struct divide_many<R1>
		{
			using type = R1;
		};

		template<ratio_type R1, ratio_type R2>
		struct divide_many<R1, R2>
		{
			using type = std::ratio_divide<R1, R2>;
		};

		template<ratio_type R1, ratio_type R2, ratio_type... RR>
		struct divide_many<R1, R2, RR...>
		{
			using type = std::ratio_divide<R1, typename divide_many<R2, RR...>::type>;
		};

		template<ratio_type... RR>
		using divide_many_t = typename divide_many<RR...>::type;

		template<ratio_type Ratio>
		using reciprocal = std::ratio<Ratio::den, Ratio::num>;

		template<ratio_type Ratio>
		using additive_inverse = std::ratio<-Ratio::num, Ratio::den>;

		template<ratio_type Ratio>
		using canonical_ratio = std::ratio<Ratio::num / collin::math::static_gcd<Ratio::num, Ratio::den>::value,
										   Ratio::den / collin::math::static_gcd<Ratio::num, Ratio::den>::value
										  >;
	}
}