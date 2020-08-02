#include <iostream>

#include "collin/net/http.hpp"
#include "collin/test.hpp"

class http_response_test : public collin::test::test_case
{
	public:
		http_response_test()
			: collin::test::test_case{"http_response_test"} {}

		void operator()() override
		{
			collin::http::http_client c;
			collin::http::basic_http_request r;
			r.host("www.httpvshttps.com");

			std::error_code ec;
			auto fut = c.async_send(r, ec);
			auto op = fut.get();
			collin::test::assert_true(op.has_value());
			collin::test::assert_equal(ec.value(), 0);
			collin::test::assert_equal(op->response_code(), 200);
		}
};


int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<http_response_test>();

	collin::test::text_test_runner runner(std::cout);

	return !runner.run(suite);
}