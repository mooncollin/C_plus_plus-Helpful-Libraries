#pragma once

#include <concepts>

namespace collin
{
	namespace concepts
	{
		template<class T>
		concept complete_type = 
			requires
		{
			sizeof(T);
		};

		template<class T, class S>
		concept stream_writable =
			requires(const T& t, S& s)
		{
			{ s << t };
		};
	}
}