export module cmoon.net.buffers.transfer_at_least;

import <system_error>;
import <cstddef>;

namespace cmoon::net
{
	export
	class transfer_at_least
	{
		public:
			explicit transfer_at_least(std::size_t m)
				: minimum_{m} {}

			std::size_t operator()(const std::error_code& ec, std::size_t n) const
			{
				if (!ec && n < minimum_)
				{
					return n;
				}

				return 0;
			}
		private:
			std::size_t minimum_;
	};
}