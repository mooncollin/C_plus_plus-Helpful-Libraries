export module cmoon.vectorized.impl.vectorized_projection;

import <concepts>;
import <type_traits>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<class T, class Proj>
	concept vectorized_projection = std::regular_invocable<Proj, T> &&
									cmoon::arithmetic<std::invoke_result_t<Proj, T>>;
}