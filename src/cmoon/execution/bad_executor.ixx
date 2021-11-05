export module cmoon.execution.bad_executor;

import <exception>;

namespace cmoon::execution
{
	export
	class bad_executor : public std::exception
	{
		public:
			bad_executor() noexcept = default;

			const char* what() const noexcept override
			{
				return "executor object has no target";
			}
	};
}