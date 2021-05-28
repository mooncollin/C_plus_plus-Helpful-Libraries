export module cmoon.net.buffers.transfer_exactly;

import <system_error>;
import <cstddef>;
import <algorithm>;

namespace cmoon::net
{
	export
	class transfer_exactly
	{
		public:
			explicit transfer_exactly(std::size_t e)
				: exact_{e} {}

			std::size_t operator()(const std::error_code& ec, std::size_t n) const
			{
				if (!ec && n < exact_)
				{
					return std::min(exact_ - n, n);
				}

				return 0;
			}
		private:
			std::size_t exact_;
	};
}