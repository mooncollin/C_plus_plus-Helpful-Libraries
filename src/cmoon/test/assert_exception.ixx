export module cmoon.test.assert_exception;

import <stdexcept>;
import <string>;

namespace cmoon::test
{
	export
	class assert_exception : public std::runtime_error
	{
		public:
			assert_exception(const std::string& str)
				: std::runtime_error{str} {}

			assert_exception(const char* str)
				: std::runtime_error{str} {}
	};
}