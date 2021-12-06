export module cmoon.executors.context_stopped;

import <exception>;

namespace cmoon::executors
{
	export
	class context_stopped : public std::exception
	{
		public:
			constexpr context_stopped() = default;

			constexpr const char* what() const noexcept final
			{
				return "context already stopped";
			}
	};
}