export module cmoon.tests.execution.async;

import <thread>;
import <functional>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class async_test : public cmoon::test::test_case
	{
		struct system_executor
		{
			template<class F>
			void execute(F&& f) const
			{
				std::thread{std::forward<F>(f)}.detach();
			}

			[[nodiscard]] constexpr friend auto operator<=>(const system_executor&, const system_executor&) = default;
		};

		public:
			async_test()
				: cmoon::test::test_case{"async_test"} {}

			void operator()() override
			{
				constexpr int expected {8};

				system_executor e;

				auto r = cmoon::execution::async(e, std::plus<>{}, 3, 5);
				cmoon::test::assert_equal(r.get(), expected);
			}
	};
}