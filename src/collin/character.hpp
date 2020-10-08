#pragma once

#include <cctype>
#include <cstddef>

namespace collin
{
	namespace character
	{
		[[nodiscard]] int digit(int code_point, int radix) noexcept
		{
			if (std::isdigit(code_point))
			{
				if (code_point < radix + '0')
				{
					return code_point;
				}

				return -1;
			}

			if (code_point >= 'A' && code_point <= 'Z')
			{
				if (code_point < radix + 'A' - 10)
				{
					return code_point - 'A' + 10;
				}

				return -1;
			}

			if (code_point >= 'a' && code_point <= 'z')
			{
				if (code_point < radix + 'a' - 10)
				{
					return code_point - 'a' + 10;
				}

				return -1;
			}

			if (code_point >= '\uFF21' && code_point <= '\uFF3A')
			{
				if (code_point < radix + '\uFF21' - 10)
				{
					return code_point - '\uFF21' + 10;
				}

				return -1;
			}

			if (code_point >= '\uFF41' && code_point <= '\uFF5A')
			{
				if (code_point < radix + '\uFF41' - 10)
				{
					return code_point - '\uFF41' + 10;
				}
			}

			return -1;
		}
	}
}