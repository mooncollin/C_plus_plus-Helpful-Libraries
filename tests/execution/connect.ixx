export module cmoon.tests.execution.connect;

import <utility>;
import <functional>;

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

	export
	class connect_test : public cmoon::test::test_case
	{
		struct connect_s : public cmoon::execution::sender_base
		{
			template<class R>
			struct op_state
			{
				friend void tag_invoke(cmoon::execution::start_t, op_state&) noexcept {}
			};

			template<class R>
			friend op_state<R> tag_invoke(cmoon::execution::connect_t, connect_s& s, R&&)
			{
				s.connected = true;
				return op_state<R>{};
			}

			bool connected {false};
		};

		static_assert(cmoon::execution::operation_state<connect_s::op_state<void_receiver>>);
		static_assert(cmoon::execution::sender<connect_s>);

		public:
			connect_test()
				: cmoon::test::test_case{"connect_test"} {}

			void operator()() override
			{
				connect_s c;

				auto o = cmoon::execution::connect(c, void_receiver{});

				cmoon::test::assert_true(c.connected);
			}
	};
}