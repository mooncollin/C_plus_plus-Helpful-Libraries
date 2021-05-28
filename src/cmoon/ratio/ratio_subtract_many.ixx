export module cmoon.ratio.ratio_subtract_many;

import <ratio>;

import cmoon.ratio.is_ratio;
import cmoon.ratio.ratio_add_many;
import cmoon.ratio.ratio_additive_inverse;

namespace cmoon
{
	export
	template<ratio_type R1, ratio_type... RR>
	using ratio_subtract_many = ratio_add_many<R1,
											   ratio_additive_inverse<RR>...>;
}