export module cmoon.platform.cpp_version;

import <version>;

namespace cmoon
{
	export
	enum class cpp_version_type
	{
		cpp_1998,
		cpp_2011,
		cpp_2014,
		cpp_2017,
		cpp_2020,
		not_available
	};

	export
	inline constexpr cpp_version_type cpp_version = 
	#if defined(_MSVC_LANG)
		#if _MSVC_LANG > 201703L
		cpp_version_type::cpp_2020
		#elif _MSVC_LANG > 201402L
		cpp_version_type::cpp_2017
		#elif _MSVC_LANG > 201103L
		cpp_version_type::cpp_2014
		#elif _MSVC_LANG > 199711L
		cpp_version_type::cpp_2011
		#else
		cpp_version_type::cpp_1998
		#endif
	#elif defined(__cplusplus)
		#if __cplusplus > 201703L
		cpp_version_type::cpp_2020
		#elif __cplusplus > 201402L
		cpp_version_type::cpp_2017
		#elif __cplusplus > 201103L
		cpp_version_type::cpp_2014
		#elif __cplusplus > 199711L
		cpp_version_type::cpp_2011
		#else
		cpp_version_type::cpp_1998
		#endif
	#else
	cpp_version_type::not_available
	#endif
	;
}