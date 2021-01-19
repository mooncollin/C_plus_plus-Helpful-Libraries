#pragma once

#include <stdexcept>

namespace cmoon
{
	namespace test
	{
		class assert_exception : public std::runtime_error
		{
			public:
				assert_exception(std::string_view str)
					: std::runtime_error{std::string(str)} {}
		};
	}
}