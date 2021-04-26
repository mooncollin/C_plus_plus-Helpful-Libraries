export module cmoon.utility.throw_on_error;

import <string_view>;
import <system_error>;
import <string>;

namespace cmoon
{
	export
	struct throw_on_error
	{
		explicit throw_on_error(std::string_view msg)
			: msg{msg} {}

		~throw_on_error() noexcept(false)
		{
			if (ec)
			{
				throw std::system_error(ec, msg.c_str());
			}
		}

		throw_on_error(const throw_on_error&) = delete;
		throw_on_error& operator=(const throw_on_error&) = delete;

		throw_on_error(throw_on_error&&) noexcept = default;
		throw_on_error& operator=(throw_on_error&&) noexcept = default;

		operator std::error_code& () noexcept
		{
			return ec;
		}

		std::string msg;
		std::error_code ec;
	};
}