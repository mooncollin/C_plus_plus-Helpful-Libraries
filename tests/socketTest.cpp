#include "net/collin_iocontext.hpp"
#include "net/collin_socket.hpp"
#include "net/collin_internet.hpp"
#include <system_error>
#include <iostream>

using namespace collin::net;
using namespace collin::net::ip;

int main()
{
	io_context ctx;
	basic_stream_socket<tcp> s(ctx);
	basic_resolver<tcp> r(ctx);
	basic_endpoint<tcp> ep(make_address_v4("172.217.4.196"), 80);
	std::cout << ep << '\n';
	//std::error_code ec;
	//s.connect(ep, ec);
	//if (!ec)
	//{
	//	s.send("Hello!");
	//}
	const auto results = r.resolve("localhost", "5000");
	if (results.size() > 0)
	{
		std::error_code ec;
		auto result = connect(s, results, ec);
		std::cout << result << '\n';
		if (!ec)
		{
			s.send("Hello!");
		}
	}
}