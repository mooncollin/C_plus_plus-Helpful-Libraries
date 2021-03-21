#pragma once

namespace cmoon
{
	namespace execution
	{
		struct execption_arg_t
		{
			explicit execption_arg_t() = default;
		};

		inline constexpr execption_arg_t execption_arg {};
	}
}