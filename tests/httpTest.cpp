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
	//r.header(collin::http::host_header, "www.google.com");
	r.header(collin::http::host_header, "127.0.0.1:5000");
	r.resource("/video");

	std::string body;
	const auto op = c.send(r, body);
	//const auto op = c.send(r);
	if (op)
	{
		//std::cout << op.value().response_code() << '\n';
		std::cout << body << '\n';
	}
	else
	{
		std::cout << "No response\n";
	}

	std::getchar();
}