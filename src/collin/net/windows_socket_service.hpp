#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include <system_error>

#include "collin/net/executor.hpp"

namespace collin
{
	namespace net
	{
		class windows_socket_service : public execution_context::service
		{
			public:
				using key_type = windows_socket_service;
				
				explicit windows_socket_service(execution_context& ctx)
					: execution_context::service{ctx}
				{
					#ifdef _WIN32
					WSADATA data;
					if (auto result = WSAStartup(MAKEWORD(2, 2), &data); result != 0)
					{
						throw std::system_error{result, std::system_category()};
					}
					#endif
				}

				windows_socket_service(const windows_socket_service&) = delete;
				windows_socket_service& operator=(const windows_socket_service&) = delete;
			private:
				void shutdown() noexcept override
				{
					#ifdef _WIN32
					WSACleanup();
					#endif
				}
		};
	}
}