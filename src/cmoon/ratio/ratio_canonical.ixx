export module cmoon.ratio.ratio_canonical;

import <ratio>;
import <numeric>;

import cmoon.ratio.is_ratio;

namespace cmoon
{
	export
	template<ratio_type Ratio>
	using ratio_canonical = std::ratio<Ratio::num / std::gcd(Ratio::num, Ratio::den),
									   Ratio::den / std::gcd(Ratio::num, Ratio::den)
									   >;
}