#pragma once

#ifdef _WIN32
	#include <WinSock2.h>
#endif

namespace collin
{
	namespace net
	{
		class io_context
		{
			public:
				io_context()
				{
					std::scoped_lock l {socket_count_m};
					if(socket_ref_count == 0)
					{
						socket_init();
						socket_ref_count++;
					}
				}

				~io_context()
				{
					std::scoped_lock l {socket_count_m};
					socket_ref_count--;
					if(socket_ref_count == 0)
					{
						socket_cleanup();
					}
				}

			private:
				static int socket_init(void) noexcept
				{
					#ifdef _WIN32
					WSADATA data;
					return WSAStartup(MAKEWORD(2, 2), &data);
					#else
					return 0;
					#endif
				}

				static int socket_cleanup(void) noexcept
				{
					#ifdef _WIN32
					return WSACleanup();
					#else
					return 0;
					#endif
				}

				static inline std::mutex socket_count_m;
				static inline std::size_t socket_ref_count {0};
		};
	}
}