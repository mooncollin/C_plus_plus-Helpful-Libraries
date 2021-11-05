export module cmoon.tests.execution.start;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class start_member_fn_test : public cmoon::test::test_case
	{
		struct op_state
		{
			void start() noexcept
			{
				started = true;
			}

			bool started {false};
		};

		public:
			start_member_fn_test()
				: cmoon::test::test_case{"start_member_fn_test"} {}

			void operator()() override
			{
				op_state o;

				cmoon::execution::start(o);

				cmoon::test::assert_true(o.started);
			}
	};

	struct op_state_default
	{
		bool started {false};
	};

	void start(op_state_default& o) noexcept
	{
		o.started = true;
	}

	export
	class start_default_fn_test : public cmoon::test::test_case
	{
		public:	
			start_default_fn_test()
				: cmoon::test::test_case{"start_default_fn_test"} {}

			void operator()() override
			{
				op_state_default o;

				cmoon::execution::start(o);

				cmoon::test::assert_true(o.started);
			}
	};
}