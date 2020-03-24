#include "net/collin_http.hpp"
#include "net/collin_iocontext.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <chrono>

int main()
{
	collin::net::io_context ctx;
	collin::http::HttpClient c(ctx);
	collin::http::HttpRequest<std::string> r;
	r.host(ctx, collin::net::ip::address_v4::loopback().to_string(), 5000);
	r.resource("/");

	std::string body;
	std::error_code ec;
	const auto op = c.send(r, body, ec);
	if (op)
	{
		std::cout << body << '\n';
	}
	else
	{
		if (ec)
		{
			std::cout << "Error code: " << ec << '\n';
		}
		else
		{
			std::cout << "No response\n";
		}
	}

	std::getchar();
}