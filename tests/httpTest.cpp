#include <iostream>
#include <sstream>
#include <fstream>

#include "collin/net/windows_socket_service.hpp"
#include "collin/net/http.hpp"
#include "collin/test.hpp"

class http_response_test : public collin::test::test_case
{
	public:
		http_response_test()
			: collin::test::test_case{"http_response_test"} {}

		void operator()() override
		{
			collin::net::io_context ctx;
			collin::http::http_client c{ctx};
			collin::http::http_request r;
			r.host("www.httpvshttps.com");
			std::ofstream out_file{"test.html"};
			auto response = c.send(r, out_file);
			collin::test::assert_equal(response.status().code, 200);
		}
};

class https_response_test : public collin::test::test_case
{
	public:
		https_response_test()
			: collin::test::test_case{"https_response_test"} {}

		void operator()() override
		{
			collin::net::io_context ctx;
			collin::http::https_client c{ctx};
			collin::http::http_request r;
			r.host("www.google.com");
			std::ofstream f{"test.html"};
			auto response = c.send(r, f);
			collin::test::assert_equal(response.status().code, 200);
		}
};


int main()
{
	collin::test::test_suite suite;
	//suite.add_test_case<http_response_test>();
	suite.add_test_case<https_response_test>();

	collin::test::text_test_runner runner(std::cout);

	return !runner.run(suite);
}