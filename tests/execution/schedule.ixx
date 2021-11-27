export module cmoon.tests.execution.schedule;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	struct void_receiver
	{
		friend void tag_invoke(cmoon::execution::set_done_t, void_receiver&&) noexcept {}

		template<class E>
		friend void tag_invoke(cmoon::execution::set_error_t, void_receiver&&, E&&) noexcept {}

		friend void tag_invoke(cmoon::execution::set_value_t, void_receiver&&) {}
	};

	static_assert(cmoon::execution::receiver<void_receiver>);

	struct submit_s : public cmoon::execution::sender_base
	{
		template<class R>
		struct op_state
		{
			friend void tag_invoke(cmoon::execution::start_t, op_state&) noexcept {}
		};

		template<class R>
		friend op_state<R> tag_invoke(cmoon::execution::connect_t, submit_s& s, R&&)
		{
			s.connected = true;
			return op_state<R>{};
		}

		bool connected {false};
		bool submitted {false};
	};

	struct schedule_s
	{
		friend submit_s tag_invoke(cmoon::execution::schedule_t, schedule_s& s)
		{
			s.schedule_called = true;
			return submit_s{};
		}

		bool schedule_called {false};
	};

	static_assert(cmoon::execution::operation_state<submit_s::op_state<void_receiver>>);
	static_assert(cmoon::execution::sender<submit_s>);

	export
	class schedule_test : public cmoon::test::test_case
	{
		public:
			schedule_test()
				: cmoon::test::test_case{"schedule_test"} {}

			void operator()() override
			{
				schedule_s s;

				auto sender = cmoon::execution::schedule(s);

				cmoon::test::assert_true(s.schedule_called);
			}
	};
}