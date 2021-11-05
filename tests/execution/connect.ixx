export module cmoon.tests.execution.connect;

import <utility>;
import <functional>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	struct void_receiver
	{
		void set_done() noexcept {}

		template<class E>
		void set_error(E&& e) noexcept {}

		void set_value() {}
	};

	static_assert(cmoon::execution::receiver<void_receiver>);

	export
	class connect_member_fn_test : public cmoon::test::test_case
	{
		struct connect_s : public cmoon::execution::sender_base
		{
			template<class R>
			struct op_state
			{
				void start() noexcept {}
			};

			template<class R>
			op_state<R> connect(R&&)
			{
				connected = true;
				return op_state<R>{};
			}

			bool connected {false};
		};

		static_assert(cmoon::execution::operation_state<connect_s::op_state<void_receiver>>);
		static_assert(cmoon::execution::sender<connect_s>);

		public:
			connect_member_fn_test()
				: cmoon::test::test_case{"connect_member_fn_test"} {}

			void operator()() override
			{
				connect_s c;

				auto o = cmoon::execution::connect(c, void_receiver{});

				cmoon::test::assert_true(c.connected);
			}
	};

	struct connect_default_s : public cmoon::execution::sender_base
	{
		template<class R>
		struct op_state_default
		{
			void start() noexcept {}
		};

		bool connected {false};
	};

	static_assert(cmoon::execution::operation_state<connect_default_s::op_state_default<void_receiver>>);
	static_assert(cmoon::execution::sender<connect_default_s>);

	template<class R>
	connect_default_s::op_state_default<R> connect(connect_default_s& s, R&&)
	{
		s.connected = true;
		return {};
	}

	export
	class connect_default_fn_test : public cmoon::test::test_case
	{
		public:
			connect_default_fn_test()
				: cmoon::test::test_case{"connect_default_fn_test"} {}

			void operator()() override
			{
				connect_default_s c;

				auto o = cmoon::execution::connect(c, void_receiver{});

				cmoon::test::assert_true(c.connected);
			}
	};
}