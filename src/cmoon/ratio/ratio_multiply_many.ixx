export module cmoon.ratio.ratio_multiply_many;

import <ratio>;

import cmoon.ratio.is_ratio;

namespace cmoon
{
	export
	template<ratio_type...>
	struct ratio_multiply_many;

	export
	template<ratio_type R1>
	struct ratio_multiply_many<R1>
	{
		using type = R1;
	};

	export
	template<ratio_type R1, ratio_type R2>
	struct ratio_multiply_many<R1, R2>
	{
		using type = std::ratio_multiply<R1, R2>;
	};

	export
	template<ratio_type R1, ratio_type R2, ratio_type... RR>
	struct ratio_multiply_many<R1, R2, RR...>
	{
		using type = std::ratio_multiply<R1, typename ratio_multiply_many<R2, RR...>::type>;
	};

	export
	template<ratio_type... RR>
	using ratio_multiply_many_t = typename ratio_multiply_many<RR...>::type;
}