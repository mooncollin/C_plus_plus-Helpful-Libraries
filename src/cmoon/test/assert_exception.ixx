module;
#include <stdexcept>

export module cmoon.test.assert_exception;

namespace cmoon::test
{
	export
	class assert_exception : public std::runtime_error
	{
		public:
			assert_exception(std::string_view str)
				: std::runtime_error{std::string(str)} {}
	};
}