export module cmoon.ratio.ratio_additive_inverse;

import <ratio>;

import cmoon.ratio.is_ratio;

namespace cmoon
{
	export
	template<ratio_type Ratio>
	using ratio_additive_inverse = std::ratio<-Ratio::num, Ratio::den>;
}