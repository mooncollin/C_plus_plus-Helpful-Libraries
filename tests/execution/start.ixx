export module cmoon.tests.execution.start;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class start_test : public cmoon::test::test_case
	{
		struct op_state
		{
			friend void tag_invoke(cmoon::execution::start_t, op_state& o) noexcept
			{
				o.started = true;
			}

			bool started {false};
		};

		public:
			start_test()
				: cmoon::test::test_case{"start_test"} {}

			void operator()() override
			{
				op_state o;

				cmoon::execution::start(o);

				cmoon::test::assert_true(o.started);
			}
	};
}