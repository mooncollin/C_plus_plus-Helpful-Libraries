#pragma once

#include <system_error>

#include "collin/net/executor.hpp"

#include <openssl/ssl.h>
#include <openssl/err.h>

namespace collin
{
	namespace net
	{
		class openssl_context
		{
			public:
				openssl_context()
					: ssl_ctx{SSL_CTX_new(SSLv23_client_method())}
				{
					if (!ssl_ctx)
					{
						throw std::runtime_error{"Unable to create SSL context"};
					}

					//SSL_CTX_set_ecdh_auto(ssl_ctx, 1);
					//if (SSL_CTX_use_certificate_file(ssl_ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0)
					//{
					//	throw_last_error();
					//}

					//if (SSL_CTX_use_PrivateKey_file(ssl_ctx, "key.pem", SSL_FILETYPE_PEM) <= 0)
					//{
					//	throw_last_error();
					//}
				}

				openssl_context(const openssl_context&) = delete;
				openssl_context& operator=(const openssl_context&) = delete;
				openssl_context(openssl_context&& rhs) noexcept
					: ssl_ctx{std::exchange(rhs.ssl_ctx, nullptr)} {}

				openssl_context& operator=(openssl_context&& rhs) noexcept
				{
					if (this != std::addressof(rhs))
					{
						ssl_ctx = std::exchange(rhs.ssl_ctx, nullptr);
					}

					return *this;
				}

				~openssl_context()
				{
					SSL_CTX_free(ssl_ctx);
				}

				SSL_CTX* get_context()
				{
					return ssl_ctx;
				}
			private:
				SSL_CTX* ssl_ctx;

				static constexpr std::size_t openssl_error_string_size = 256;

				void throw_last_error() const
				{
					const auto err = ERR_get_error();
					if (err != 0)
					{
						std::string last_error;
						last_error.resize(openssl_error_string_size);
						ERR_error_string_n(ERR_get_error(), last_error.data(), last_error.size());
						throw std::runtime_error{last_error};
					}
				}
		};

		class openssl_socket_service : public execution_context::service
		{
			public:
				using key_type = openssl_socket_service;

				explicit openssl_socket_service(execution_context& ctx)
					: execution_context::service{ctx}
				{
					SSL_library_init();
					SSL_load_error_strings();
					OpenSSL_add_ssl_algorithms();
				}

				openssl_socket_service(const openssl_socket_service&) = delete;
				openssl_socket_service& operator=(const openssl_socket_service&) = delete;

				openssl_context& get_context()
				{
					static openssl_context ctx {};
					return ctx;
				}
			private:
				void shutdown() noexcept override
				{
					EVP_cleanup();
				}
		};
	}
}