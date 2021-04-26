export module cmoon.platform.architecture;

namespace cmoon
{
	export
	enum class architecture_type
	{
		alpha,
		amd64,
		arm,
		arm_thumb,
		i386,
		mips,
		powerpc,
		not_available
	};

	export
	inline constexpr architecture_type architecture = 
	#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
	architecture_type::alpha
	#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64)
	architecture_type::amd64
	#elif defined(__arm__) || defined(_ARM) || defined(__arm) || defined(_M_ARM)
		#if defined(__thumb__) || defined(_M_ARMT)
			architecture_type::arm_thumb
		#else
			architecture_type::arm
		#endif
	#elif defined(__i386__) || defined(__i386) || defined(_M_I86)
	architecture_type::i386
	#elif defined(__mips__)
	architecture_type::mips
	#elif defined(__powerpc) || defined(__powerpc__) || defined(_M_PPC)
	architecture_type::powerpc
	#else
	architecture_type::not_available
	#endif
	;
}