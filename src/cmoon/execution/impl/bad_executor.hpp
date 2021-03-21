#pragma once

#include <exception>

namespace cmoon
{
	namespace execution
	{
		class bad_executor : public std::exception
		{
			public:
				bad_executor() noexcept {}

				const char* what() const noexcept
				{
					return "bad executor";
				}
		};
	}
}