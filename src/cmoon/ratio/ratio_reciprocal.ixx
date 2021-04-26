export module cmoon.ratio.ratio_reciprocal;

import <ratio>;

import cmoon.ratio.is_ratio;

namespace cmoon
{
	export
	template<ratio_type Ratio>
	using ratio_reciprocal = std::ratio<Ratio::den, Ratio::num>;
}