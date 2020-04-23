#include "collin/net/http.hpp"
#include "collin/net/iocontext.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <sstream>

int main()
{
	collin::net::io_context ctx;
	collin::http::http_client c(ctx);
	collin::http::basic_http_request r;
	r.host("www.scratchpads.eu");
	r.resource("/explore/sites-list");

	std::string body;
	std::error_code ec;
	auto now = std::chrono::steady_clock::now();
	//const auto op = c.send(r, body, ec);
	//const auto op = c.send(r, ec);
	auto future = c.sendAsync(r, ec);
	auto time_elapsed = std::chrono::steady_clock::now() - now;
	const auto op = future.get();
	if (op && !ec)
	{
		//std::cout << "Status Code: " << op->response_code() << '\n';
		//std::cout << body << '\n';
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time_elapsed).count() << "ms\n";
	}
	else
	{
		if (ec)
		{
			std::cout << "Error code: " << ec.message() << '\n';
		}
		else
		{
			std::cout << "No response\n";
		}
	}

	std::getchar();
}