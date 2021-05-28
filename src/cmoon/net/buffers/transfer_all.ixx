export module cmoon.net.buffers.transfer_all;

import <system_error>;
import <cstddef>;

namespace cmoon::net
{
	export
	class transfer_all
	{
		public:
			std::size_t operator()(const error_code& ec, std::size_t n) const
			{
				if (!ec)
				{
					return n;
				}

				return 0;
			}
	};
}