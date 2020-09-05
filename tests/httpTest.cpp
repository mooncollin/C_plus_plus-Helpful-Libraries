#include <iostream>
#include <sstream>

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
			collin::http::basic_http_request r{ctx};
			r.host("www.httpvshttps.com");
			auto response = c.send(r);
			collin::test::assert_equal(response.status().code, 200);
		}
};


int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<http_response_test>();

	collin::test::text_test_runner runner(std::cout);

	return !runner.run(suite);
}