#ifdef _WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    using SOCKET = int;
#endif

#include <iostream>
#include <string_view>

namespace collin
{
	class Socket
	{
	public:
		static int init(void)
		{
			#ifdef _WIN32
				WSADATA data;
				return WSAStartup(MAKEWORD(2, 2), &data);
			#else
				return 0;
			#endif
		}

		static int cleanup(void)
		{
			#ifdef _WIN32
				return WSACleanup();
			#else
				return 0;
			#endif
		}

		Socket(int family, int socktype, int protocol)
			: sock(socket(family, socktype, protocol)) {}

		~Socket()
		{
			close();
		}

		int close()
		{
			int status = 0;
			if(*this)
			{
				#ifdef _WIN32
					status = shutdown(sock, SD_BOTH);
					if (status == 0)
					{
						status = closesocket(sock);
					}
				#else
					status = shutdown(sock, SHUT_RDWR);
					if (status == 0)
					{
						status = close(sock);
					}
				#endif
			}

			return status;
		}

		int connect(sockaddr* address)
		{
			return ::connect(sock, address, sizeof(sockaddr_in));
		}

		int connect(sockaddr_in* address)
		{
			return connect((sockaddr*)address);
		}

		template<class T>
		Socket& operator<<(T value)
		{
			sendData(value);
			return *this;
		}

		template<class T>
		Socket& operator>>(T& value)
		{
			receiveData(value);
			return *this;
		}

		bool receiveData(void* buf, int buflen, int flags=0)
		{
			auto pbuf = (char*)buf;
			while (buflen > 0)
			{
				const auto num = ::recv(sock, pbuf, buflen, flags);
				if (num == SOCKET_ERROR)
				{
					return false;
				}

				pbuf += num;
				buflen -= num;
			}

			return true;
		}

		template<class T>
		bool receiveData(T& value)
		{
			auto result = receiveData(&value, sizeof(T));
			
			if constexpr (std::is_same_v<T, unsigned long>)
			{
				value = ::ntohl(value);
			}
			else if constexpr (std::is_same_v<T, unsigned long long>)
			{
				value = ::ntohll(value);
			}
			else if constexpr (std::is_same_v<T, unsigned short>)
			{
				value = ::ntohs(value);
			}

			return result;
		}

		template<>
		bool receiveData(std::string& str)
		{
			return receiveData(&str[0], str.size());
		}

		bool sendData(void* buf, int buflen, int flags=0)
		{
			auto pbuf = (const char*)buf;
			while (buflen > 0)
			{
				const auto num = ::send(sock, pbuf, buflen, flags);
				if (num == SOCKET_ERROR)
				{
					return false;
				}
				pbuf += num;
				buflen -= num;
			}

			return true;
		}

		template<class T>
		bool sendData(T value)
		{
			void* ptr = nullptr;
			int length = 0;
			if constexpr (std::is_integral_v<T>)
			{
				if constexpr (std::is_same_v<T, unsigned long>)
				{
					value = ::htonl(value);
				}
				else if constexpr (std::is_same_v<T, unsigned long long>)
				{
					value = ::htonll(value);
				}
				else if constexpr (std::is_same_v<T, unsigned short>)
				{
					value = ::htons(value);
				}
				else
				{
					throw "Type not supported";
				}

				ptr = (void*)&value;
				length = sizeof(T);
			}
			else if constexpr (std::is_convertible_v<T, std::string_view>)
			{
				std::string_view s = value;
				ptr = (void*)s.data();
				length = s.length();
			}
			else
			{
				throw "Type not supported";
			}

			return sendData(ptr, length);
		}

		bool operator==(int value) const
		{
			return value == sock;
		}

		bool operator!=(int value) const
		{
			return !(operator==(value));
		}

		explicit operator bool() const
		{
			return *this != -1;
		}

		SOCKET sock;
	};

	class AddressInfo
	{
	public:
		AddressInfo() = default;
		~AddressInfo()
		{
			if (resultList != nullptr)
			{
				::freeaddrinfo(resultList);
			}
		}

		addrinfo** operator&()
		{
			return &resultList;
		}

		addrinfo* operator->()
		{
			return resultList;
		}

		addrinfo* resultList = nullptr;
	};

	class AddressInfoHints
	{
	public:
		AddressInfoHints(int family=AF_INET, int socktype=SOCK_STREAM, int protocol=0)
		{
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = family;
			hints.ai_socktype = socktype;
			hints.ai_protocol = protocol;
		}

		addrinfo* operator&()
		{
			return &hints;
		}

		addrinfo hints;
	};

	int resolveHostName(std::string_view pszHostName, std::string_view service, sockaddr_in* pAddr)
	{
		AddressInfo pResultList;
		AddressInfoHints hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);

		const auto ret = ::getaddrinfo(pszHostName.data(), service.data(), &hints, &pResultList);
		auto result = (ret == 0) ? 1 : -1;

		if (result != -1)
		{
			*pAddr = *(sockaddr_in*)(pResultList->ai_addr);
			result = 0;
		}

		return result;
	}
}