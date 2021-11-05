export module cmoon.tests.execution.schedule;

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
	class schedule_member_fn_test : public cmoon::test::test_case
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

		struct schedule_s
		{
			submit_s schedule()
			{
				schedule_called = true;
				return submit_s{};
			}

			bool schedule_called {false};
		};

		static_assert(cmoon::execution::operation_state<submit_s::op_state<void_receiver>>);
		static_assert(cmoon::execution::sender<submit_s>);

		public:
			schedule_member_fn_test()
				: cmoon::test::test_case{"schedule_member_fn_test"} {}

			void operator()() override
			{
				schedule_s s;

				auto sender = cmoon::execution::schedule(s);

				cmoon::test::assert_true(s.schedule_called);
			}
	};

	struct submit_default_s : public cmoon::execution::sender_base
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

	template<class R>
	submit_default_s::op_state<R> connect(submit_default_s& s, R&&)
	{
		s.connected = true;
		return {};
	}

	struct schedule_default_s
	{
		bool schedule_called {false};
	};

	submit_default_s schedule(schedule_default_s& s)
	{
		s.schedule_called = true;
		return submit_default_s{};
	}

	static_assert(cmoon::execution::operation_state<submit_default_s::op_state<void_receiver>>);
	static_assert(cmoon::execution::sender<submit_default_s>);

	export
	class schedule_default_fn_test : public cmoon::test::test_case
	{
		public:
			schedule_default_fn_test()
				: cmoon::test::test_case {"schedule_default_fn_test"} {}

			void operator()() override
			{
				schedule_default_s s;

				auto sender = cmoon::execution::schedule(s);

				cmoon::test::assert_true(s.schedule_called);
			}
	};

	export
	class schedule_as_executor_test : public cmoon::test::test_case
	{
		struct boolean_receiver
		{
			void set_done() noexcept {}

			template<class E>
			void set_error(E&&) noexcept {}

			void set_value()
			{
				value = true;
			}

			bool& value;
		};


		struct executor_s
		{
			template<class F>
			void execute(F&& f) const
			{
				std::invoke(f);
			}

			[[nodiscard]] constexpr friend bool operator==(const executor_s&, const executor_s&) noexcept = default;
		};

		public:
			schedule_as_executor_test()
				: cmoon::test::test_case {"schedule_as_executor_test"} {}

			void operator()() override
			{
				executor_s e;

				bool value {false};

				auto sender = cmoon::execution::schedule(e);
				auto state = cmoon::execution::connect(sender, boolean_receiver{value});
				cmoon::execution::start(state);

				cmoon::test::assert_true(value);
			}
	};
}