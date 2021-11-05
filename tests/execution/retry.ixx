export module cmoon.tests.execution.retry;

import <exception>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{

	export
	class retry_test : public cmoon::test::test_case
	{
		struct retry_r
		{
			constexpr retry_r(int retries)
				: retries {retries} {}

			void set_value(int)
			{
				if (retries != 0)
				{
					--retries;
					throw std::exception{};
				}

				value_set = true;
			}

			void set_error(auto&&) noexcept {}
			void set_done() noexcept {}

			int retries;
			bool value_set{false};
		};

		static_assert(cmoon::execution::receiver<retry_r>);

		public:
			retry_test()
				: cmoon::test::test_case{"retry_test"} {}

			void operator()() override
			{
				retry_r r{3};

				auto s = cmoon::execution::just(1) |
						 cmoon::execution::retry();

				auto op = cmoon::execution::connect(std::move(s), r);
				cmoon::execution::start(std::move(op));

				cmoon::test::assert_true(r.value_set);
				cmoon::test::assert_equal(r.retries, 0);
			}
	};
}