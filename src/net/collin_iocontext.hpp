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
					init();
				}

				~io_context()
				{
					cleanup();
				}
			private:
				static int init(void) noexcept
				{
					#ifdef _WIN32
					WSADATA data;
					return WSAStartup(MAKEWORD(2, 2), &data);
					#else
					return 0;
					#endif
				}

				static int cleanup(void) noexcept
				{
					#ifdef _WIN32
					return WSACleanup();
					#else
					return 0;
					#endif
				}
		};
	}
}