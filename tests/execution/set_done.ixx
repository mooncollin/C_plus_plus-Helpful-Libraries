export module cmoon.tests.execution.set_done;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class set_done_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			friend void tag_invoke(cmoon::execution::set_done_t, receiver_s& r) noexcept
			{
				r.done_set = true;
			}

			bool done_set {false};
		};

		public:
			set_done_test()
				: cmoon::test::test_case{"set_done_test"} {}

			void operator()() override
			{
				receiver_s r;

				cmoon::execution::set_done(r);

				cmoon::test::assert_true(r.done_set);
			}
	};
}