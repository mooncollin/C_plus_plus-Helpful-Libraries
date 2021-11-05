export module cmoon.tests;

import cmoon.test;

namespace cmoon::tests
{
	export
	enum class library
	{
		algorithm,
		benchmark,
		bio,
		concepts,
		csv,
		cuda,
		dynamic,
		execution,
		executors,
		files,
		finance,
		flat_map,
		functional,
		geometry,
		graphics,
		hash,
		io,
		iterator,
		json,
		language,
		linear,
		math,
		measures,
		memory,
		meta,
		multidimensional,
		multiprecision,
		mutex,
		net,
		numbers,
		platform,
		property,
		ranges,
		ratio,
		scope,
		stats,
		stemmer,
		string,
		thread,
		type_traits,
		utility,
		windows
	};

	export
	template<library Lib>
	cmoon::test::test_suite get_test_suite();
}