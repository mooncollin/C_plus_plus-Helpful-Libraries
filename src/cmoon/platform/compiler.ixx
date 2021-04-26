export module cmoon.platform.compiler;

namespace cmoon
{
	export
	enum class compiler_type
	{
		clang,
		como,
		compaq,
		gcc,
		intel,
		msvc,
		mingw32,
		mingw64,
		not_available
	};

	export
	inline constexpr compiler_type compiler =
	#if defined(__clang__)
	compiler_type::clang
	#elif defined(__como__)
	compiler_type::como
	#elif defined(__DECCXX__)
	compiler_type::compaq
	#elif defined(__GNUC__)
	compiler_type::gcc
	#elif defined(__INTEL_COMPILER__)
	compiler_type::intel
	#elif defined(_MSC_VER)
	compiler_type::msvc
	#elif defined(__MINGW64__)
	compiler_type::mingw64
	#elif defined(__WINGW32__)
	compiler_type::mingw32
	#else
	compiler_type::not_available
	#endif
	;
}