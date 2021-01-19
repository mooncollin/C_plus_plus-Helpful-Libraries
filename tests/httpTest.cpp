#include <iostream>
#include <sstream>
#include <fstream>

#include "cmoon/net/windows_socket_service.hpp"
#include "cmoon/net/http.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

class http_response_test : public cmoon::test::test_case
{
	public:
		http_response_test()
			: cmoon::test::test_case{"http_response_test"} {}

		void operator()() override
		{
			cmoon::net::io_context ctx;
			cmoon::http::http_client c{ctx};
			cmoon::http::http_request r;
			r.host("www.httpvshttps.com");
			auto response = c.send(r);
			cmoon::test::assert_equal(response.status().code, 200u);
		}
};

class https_response_test : public cmoon::test::test_case
{
	public:
		https_response_test()
			: cmoon::test::test_case{"https_response_test"} {}

		void operator()() override
		{
			cmoon::net::io_context ctx;
			cmoon::http::https_client c{ctx};
			cmoon::http::http_request r;
			r.host("www.google.com");
			auto response = c.send(r);
			cmoon::test::assert_equal(response.status().code, 200u);
		}
};


int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<http_response_test>();
	suite.add_test_case<https_response_test>();

	cmoon::test::text_test_runner runner(std::cout);

	return !runner.run(suite);
}