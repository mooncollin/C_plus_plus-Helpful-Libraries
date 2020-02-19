#include "collin_http.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

int main()
{
	collin::http::HttpClient c;
	collin::http::HttpRequest<std::string> r;
	r.header("Host", "www.google.com");

	std::vector<std::future<std::optional<collin::http::HttpResponse<std::string>>>> calls;

	for (auto i = 0; i < 10; i++)
	{
		calls.push_back(c.sendAsync<std::string>(r));
	}

	for (auto& fut : calls)
	{
		auto op = fut.get();
		if (op)
		{
			std::cout << op.value().response_code() << '\n';
		}
		else
		{
			std::cout << "No reply\n";
		}
	}

	std::getchar();
}