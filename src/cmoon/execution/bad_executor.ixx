export module cmoon.execution.bad_executor;

import <exception>;

namespace cmoon::execution
{
	export
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