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
	collin::http::http_client c;
	collin::http::basic_http_request r;
	//r.host("www.scratchpads.eu");
	r.host("www.httpvshttps.com");
	//r.resource("/explore/sites-list");

	std::error_code ec;
	auto now = std::chrono::steady_clock::now();
	//const auto op = c.send(r, body, ec);
	auto op = c.send(r, ec);
	//auto future = c.sendAsync(r, ec);
	//const auto op = future.get();
	if (op && !ec)
	{
		std::cout << "Status Code: " << op->response_code() << '\n';
		std::cout << op.value() << '\n';
		auto time_elapsed = std::chrono::steady_clock::now() - now;
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time_elapsed).count() << "ms\n";
	}
	else
	{
		std::cout << "Error code: " << ec << '\n';
	}

	std::getchar();
}