export module cmoon.tests.execution.done_as_optional;

import <optional>;
import <atomic>;
import <utility>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class done_as_optional_test : public cmoon::test::test_case
	{
		public:
			done_as_optional_test()
				: cmoon::test::test_case{"done_as_optional_test"} {}

			void operator()() override
			{
				cmoon::test::error("MSVC cannot compile this test correctly");

				/*
				constexpr int expected {5};
				std::optional<int> value;

				auto work = cmoon::execution::just(expected) |
							cmoon::execution::done_as_optional() |
							cmoon::execution::then([&value](auto v) {
								value = v;
							});

				cmoon::execution::start_detached(std::move(work));
				
				cmoon::test::assert_equal(value.value(), expected)*/
			}
	};
}