#pragma once

#ifdef _WIN32
	#include <WinSock2.h>
#endif

#include <mutex>

namespace collin
{
	namespace net
	{
		class io_context
		{
			public:
				io_context()
				{
					if (!has_started) // Do not lock a mutex if we have started
					{
						std::scoped_lock l{started_m};
						if(!has_started) // Check again if we were waiting on this mutex
						{
							socket_init();
							has_started = true;
						}
					}
				}

				~io_context() = default;

			private:
				static inline int socket_init(void) noexcept
				{
					#ifdef _WIN32
					WSADATA data;
					return WSAStartup(MAKEWORD(2, 2), &data);
					#else
					return 0;
					#endif
				}

				static inline int socket_cleanup(void) noexcept
				{
					#ifdef _WIN32
					return WSACleanup();
					#else
					return 0;
					#endif
				}

				inline static bool has_started {false};
				inline static std::mutex started_m;
		};
	}
}