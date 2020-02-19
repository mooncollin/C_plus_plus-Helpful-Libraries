#include "collin_http.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>

using namespace std::literals;

int main()
{
	collin::http::HttpClient c;
	collin::http::HttpRequest<std::string> r;
	r.header(collin::http::host_header, "www.google.com");
	std::string extra = "Hello";
	r.contentProvider(extra);

	const auto op = c.send<std::string>(r);
	if (op)
	{
		std::cout << op.value().body() << '\n';
	}
	else
	{
		std::cout << "No response\n";
	}

	std::getchar();
}