export module cmoon.ratio.ratio_multiply_many;

import <ratio>;
import <type_traits>;

import cmoon.ratio.is_ratio;

namespace cmoon
{
	template<ratio_type...>
	struct ratio_multiply_many_impl;

	template<ratio_type R1>
	struct ratio_multiply_many_impl<R1> : public std::type_identity<R1> {};

	template<ratio_type R1, ratio_type R2>
	struct ratio_multiply_many_impl<R1, R2> : public std::type_identity<std::ratio_multiply<R1, R2>> {};

	template<ratio_type R1, ratio_type R2, ratio_type... RR>
	struct ratio_multiply_many_impl<R1, R2, RR...> : public std::type_identity<std::ratio_multiply<R1, typename ratio_multiply_many_impl<R2, RR...>::type>> {};

	export
	template<ratio_type... RR>
	using ratio_multiply_many = typename ratio_multiply_many_impl<RR...>::type;
}