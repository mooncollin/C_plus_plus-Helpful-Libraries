export module cmoon.ratio.ratio_divide_many;

import <ratio>;

import cmoon.ratio.is_ratio;
import cmoon.ratio.ratio_multiply_many;
import cmoon.ratio.ratio_reciprocal;

namespace cmoon
{
	export
	template<ratio_type R1, ratio_type... RR>
	using ratio_divide_many = ratio_multiply_many<R1,
												  ratio_reciprocal<RR>...>;
}