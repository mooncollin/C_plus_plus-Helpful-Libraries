module;

#include <string>
#include <stdexcept>

export module cmoon.format.format_error;

namespace cmoon
{
	export
	class format_error : public std::runtime_error
	{
		public:
			explicit format_error(const std::string& what_arg)
				: std::runtime_error{what_arg} {}
			explicit format_error(const char* what_arg)
				: std::runtime_error{what_arg} {}
	};
}