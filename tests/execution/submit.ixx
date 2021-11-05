export module cmoon.tests.execution.submit;

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
	class submit_member_fn_test : public cmoon::test::test_case
	{
		struct submit_s : public cmoon::execution::sender_base
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

			template<class R>
			void submit(R&&)
			{
				submitted = true;
			}

			bool connected {false};
			bool submitted {false};
		};

		static_assert(cmoon::execution::operation_state<submit_s::op_state<void_receiver>>);
		static_assert(cmoon::execution::sender<submit_s>);

		public:
			submit_member_fn_test()
				: cmoon::test::test_case{"submit_member_fn_test"} {}

			void operator()() override
			{
				submit_s s;

				cmoon::execution::submit(s, void_receiver{});

				cmoon::test::assert_true(s.submitted);
			}
	};

	struct submit_default_s : public cmoon::execution::sender_base
	{
		template<class R>
		struct op_state_default
		{
			void start() noexcept {}
		};

		bool connected {false};
		bool submitted {false};
	};

	static_assert(cmoon::execution::operation_state<submit_default_s::op_state_default<void_receiver>>);
	static_assert(cmoon::execution::sender<submit_default_s>);

	template<class R>
	submit_default_s::op_state_default<R> connect(submit_default_s& s, R&&)
	{
		s.connected = true;
		return {};
	}

	template<class R>
	void submit(submit_default_s& s, R&&)
	{
		s.submitted = true;
	}

	export
	class submit_default_fn_test : public cmoon::test::test_case
	{
		public:
			submit_default_fn_test()
				: cmoon::test::test_case{"submit_default_fn_test"} {}

			void operator()() override
			{
				submit_default_s s;

				cmoon::execution::submit(s, void_receiver{});

				cmoon::test::assert_true(s.submitted);
			}
	};

	export
	class submit_start_fn_test : public cmoon::test::test_case
	{
		struct submit_s : public cmoon::execution::sender_base
		{
			template<class R>
			struct op_state
			{
				submit_s* p_;

				void start() noexcept
				{
					p_->started = true;
				}
			};

			template<class R>
			op_state<R> connect(R&&)
			{
				connected = true;
				return op_state<R>{this};
			}

			bool connected {false};
			bool started {false};
		};

		public:
			submit_start_fn_test()
				: cmoon::test::test_case{"submit_start_fn_test"} {}

			void operator()() override
			{
				submit_s s;

				cmoon::execution::submit(s, void_receiver{});

				cmoon::test::assert_true(s.started);
			}
	};
}