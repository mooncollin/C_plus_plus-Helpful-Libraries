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
				io_context() noexcept
				{
					io_context_create();
				}

				~io_context() noexcept = default;

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

				inline static void io_context_create()
				{
					static io_context_static_initialize io_static;
				}

				struct io_context_static_initialize
				{
					io_context_static_initialize() noexcept
					{
						socket_init();
					}

					io_context_static_initialize(const io_context_static_initialize&) = delete;
					io_context_static_initialize(io_context_static_initialize&&) = delete;

					io_context_static_initialize& operator=(const io_context_static_initialize&) = delete;
					io_context_static_initialize& operator=(io_context_static_initialize&&) = delete;

					~io_context_static_initialize()
					{
						socket_cleanup();
					}
				};
		};
	}
}