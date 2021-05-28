export module cmoon.net.buffers.stream_errc;

import <system_error>;

namespace cmoon::net
{
	class stream_category_t : public std::error_category
	{
		public:
			const char* name() const noexcept override
			{
				return "stream";
			}
	};

	export
	enum class stream_errc
	{
		eof = 1,
		not_found = 2
	};

	export
	const std::error_category& stream_category() noexcept
	{
		static stream_category_t category;
		return category;
	}
}

namespace std
{
	export
	std::error_code make_error_code(cmoon::net::stream_errc e) noexcept
	{
		return std::error_code{static_cast<int>(e), cmoon::net::stream_category()};
	}

	export
	std::error_condition make_error_condition(collin::net::stream_errc e) noexcept
	{
		return std::error_condition{static_cast<int>(e), cmoon::net::stream_category()};
	}
}