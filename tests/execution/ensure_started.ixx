export module cmoon.tests.execution.ensure_started;

import <atomic>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	struct receiver_ens
	{
		receiver_ens(std::atomic<int>& v)
			: value{v} {}

		friend void tag_invoke(cmoon::execution::set_value_t, receiver_ens&& r, int v)
		{
			r.value.get() = v;
			r.value.get().notify_all();
		}

		template<class E>
		friend void tag_invoke(cmoon::execution::set_error_t, receiver_ens&& r, E&&) noexcept
		{
			r.error_set = true;
		}

		friend void tag_invoke(cmoon::execution::set_done_t, receiver_ens&& r) noexcept
		{
			r.done_set = true;
		}

		std::reference_wrapper<std::atomic<int>> value;
		bool error_set{false};
		bool done_set{false};
	};

	export
	class ensure_started_value_test : public cmoon::test::test_case
	{
		public:
			ensure_started_value_test()
				: cmoon::test::test_case{"ensure_started_value_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};

				auto work = cmoon::execution::just(expected) |
							cmoon::execution::ensure_started();

				cmoon::execution::start(
					cmoon::execution::connect(
						std::move(work),
						receiver_ens{value}
					)
				);

				value.wait(0);

				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class ensure_started_error_test : public cmoon::test::test_case
	{
		public:
			ensure_started_error_test()
				: cmoon::test::test_case{"ensure_started_error_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};

				//auto work = cmoon::execution::just(expected) |
							//cmoon::execution::just_error(std::exception{}) |
							//cmoon::execution::ensure_started();

				//cmoon::execution::start(
					//cmoon::execution::connect(
						//std::move(work),
						//receiver_ens{value}
					//)
				//);

				value.wait(0);

				cmoon::test::assert_equal(value, expected);
			}
	};
}