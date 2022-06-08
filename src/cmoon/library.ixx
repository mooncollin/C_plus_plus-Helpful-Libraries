export module cmoon.library;

import cmoon.test;

import <memory>;

namespace cmoon
{
	export
	enum class library
	{
		benchmarking,
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
		parallel,
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
	template<library>
	struct library_traits
	{
	};

	export
	template<library L>
	concept testable_library =
		requires(const std::allocator<cmoon::test::test_case>& alloc)
	{
		{ library_traits<L>::tests(alloc) };
	};
}