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
		public:
			struct dummy_sender : public cmoon::execution::sender_base
			{
				template<class R>
				struct op
				{
					R r_;

					friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
					{
						cmoon::execution::set_value(std::move(o.r_));
					}
				};

				template<class R>
				friend auto tag_invoke(cmoon::execution::connect_t, dummy_sender&&, R&& r)
				{
					return op<R>{std::forward<R>(r)};
				}
			};

			struct executor_s
			{
				friend dummy_sender tag_invoke(cmoon::execution::schedule_t, executor_s&)
				{
					return dummy_sender{};
				}

				[[nodiscard]] constexpr friend bool operator==(const executor_s&, const executor_s&) noexcept = default;
			};

			async_test()
				: cmoon::test::test_case{"async_test"} {}

			void operator()() override
			{
				constexpr int expected {8};

				executor_s e;

				auto r = cmoon::execution::async(e, std::plus<>{}, 3, 5);
				cmoon::test::assert_equal(r.get(), expected);
			}
	};
}