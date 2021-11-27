export module cmoon.tests.execution.upon_error;

import <exception>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class upon_error_test : public cmoon::test::test_case
	{
		public:
			upon_error_test()
				: cmoon::test::test_case{"upon_error_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				auto s = cmoon::execution::just() |
						 cmoon::execution::then([] { 
							throw std::exception{};
						 }) |
						 cmoon::execution::upon_error([&value, expected](auto&&) {
							value = expected;
						 });

				cmoon::execution::start_detached(std::move(s));

				cmoon::test::assert_equal(value, expected);
			}
	};
}