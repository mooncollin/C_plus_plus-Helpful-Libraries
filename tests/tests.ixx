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
		emulation,
		execution,
		executors,
		files,
		finance,
		flat_map,
		geometry,
		graphics,
		hash,
		iostream,
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
		tuple,
		type_traits,
		utility,
		windows
	};

	export
	template<library Lib>
	cmoon::test::test_suite get_test_suite();
}