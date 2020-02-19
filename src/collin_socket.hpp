#ifndef COLLIN_SOCKET
#define COLLIN_SOCKET

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
#include <optional>

namespace collin
{
	class Socket
	{
	public:
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

		Socket(int family=AF_INET, int socktype=SOCK_STREAM, int protocol=0) noexcept
			: sock(socket(family, socktype, protocol)) {}

		Socket(const Socket&) = default;
		Socket(Socket&&) = default;
		Socket& operator=(const Socket&) = default;
		Socket& operator=(Socket&&) = default;

		~Socket() noexcept
		{
			close();
		}

		int close() noexcept
		{
			int status = 0;
			if(*this)
			{
				#ifdef _WIN32
					status = shutdown(sock, SD_BOTH);
					if (status == 0)
					{
						status = closesocket(sock);
						sock = -1;
					}
				#else
					status = shutdown(sock, SHUT_RDWR);
					if (status == 0)
					{
						status = close(sock);
						sock = -1;
					}
				#endif
			}

			return status;
		}

		int connect(const sockaddr_in& address) noexcept
		{
			return ::connect(sock, &(sockaddr&)address, sizeof(sockaddr_in));
		}

		template<class T>
		Socket& operator<<(T value)
		{
			sendData(value);
			return *this;
		}

		template<class T>
		Socket& operator>>(T& value) noexcept
		{
			receiveData(value);
			return *this;
		}

		bool receiveData(void* buf, int buflen, int flags=0) noexcept
		{
			auto pbuf = static_cast<char*>(buf);
			if (pbuf == nullptr)
			{
				return false;
			}

			while (buflen > 0)
			{
				const auto num = ::recv(sock, pbuf, buflen, flags);
				if (num == 0)
				{
					return false;
				}
				if (num == SOCKET_ERROR)
				{
					close();
					return false;
				}

				pbuf += num;
				buflen -= num;
			}

			return true;
		}

		bool receiveData(unsigned long& value) noexcept
		{
			const auto result = receiveData(&value, sizeof(value));
			value = ::ntohl(value);
			return result;
		}

		bool receiveData(unsigned long long& value) noexcept
		{
			const auto result = receiveData(&value, sizeof(value));
			value = ::ntohll(value);
			return result;
		}

		bool receiveData(unsigned short& value) noexcept
		{
			const auto result = receiveData(&value, sizeof(value));
			value = ::ntohs(value);
			return result;
		}

		bool receiveData(std::string& str) noexcept
		{
			if (str.empty())
			{
				return false;
			}

			return receiveData(&str[0], str.size());
		}

		template<typename T, std::size_t N>
		bool receiveData(std::array<T, N>& data)
		{
			return receiveData(data.data(), sizeof(T) * N);
		}

		bool sendData(const void* buf, int buflen, int flags=0) noexcept
		{
			auto pbuf = static_cast<const char*>(buf);
			if (pbuf == nullptr)
			{
				return false;
			}

			while (buflen > 0)
			{
				const auto num = ::send(sock, pbuf, buflen, flags);
				if (num == SOCKET_ERROR)
				{
					close();
					return false;
				}
				pbuf += num;
				buflen -= num;
			}

			return true;
		}

		template<typename T, std::size_t N>
		bool sendData(std::array<T, N>& arr)
		{
			return sendData(reinterpret_cast<const char*>(arr.data()), sizeof(T) * N);
		}

		bool sendData(unsigned long value) noexcept
		{
			value = ::htonl(value);
			return sendData(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		bool sendData(unsigned long long value) noexcept
		{
			value = ::htonll(value);
			return sendData(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		bool sendData(unsigned short value) noexcept
		{
			value = ::htons(value);
			return sendData(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		bool sendData(std::string_view value) noexcept
		{
			return sendData(value.data(), value.length());
		}

		void setOption(int type, int option) noexcept
		{
			setsockopt(sock, SOL_SOCKET, type, reinterpret_cast<char*>(&option), sizeof(option));
		}

		bool operator==(int value) const noexcept
		{
			return value == sock;
		}

		bool operator!=(int value) const noexcept
		{
			return !(operator==(value));
		}

		explicit operator bool() const noexcept
		{
			return *this != -1;
		}

		SOCKET sock;
	};

	class AddressInfo
	{
	public:
		AddressInfo() = default;
		AddressInfo(const AddressInfo&) = default;
		AddressInfo(AddressInfo&&) = default;
		AddressInfo& operator=(const AddressInfo&) = default;
		AddressInfo& operator=(AddressInfo&&) = default;
		~AddressInfo() noexcept
		{
			if (resultList != nullptr)
			{
				::freeaddrinfo(resultList);
			}
		}

		addrinfo** operator&() noexcept
		{
			return &resultList;
		}

		addrinfo* operator->() noexcept
		{
			return resultList;
		}

		addrinfo* resultList = nullptr;
	};

	class AddressInfoHints
	{
	public:
		AddressInfoHints(int family=AF_INET, int socktype=SOCK_STREAM, int protocol=0) noexcept
		{
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = family;
			hints.ai_socktype = socktype;
			hints.ai_protocol = protocol;
		}

		addrinfo* operator&() noexcept
		{
			return &hints;
		}

		addrinfo hints;
	};

	std::optional<sockaddr_in> resolveHostName(std::string_view pszHostName, std::string_view service)
	{
		AddressInfo pResultList;
		AddressInfoHints hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);

		const auto ret = ::getaddrinfo(pszHostName.data(), service.data(), &hints, &pResultList);
		const auto result = (ret == 0) ? 1 : -1;

		if (result != -1)
		{
			return std::move(*reinterpret_cast<sockaddr_in*>(pResultList->ai_addr));
		}

		return {};
	}
}

#endif