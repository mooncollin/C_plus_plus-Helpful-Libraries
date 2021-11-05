export module cmoon.tests.execution.set_done;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class set_done_member_fn_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			void set_done()
			{
				done_set = true;
			}

			bool done_set {false};
		};

		public:
			set_done_member_fn_test()
				: cmoon::test::test_case{"set_done_member_fn_test"} {}

			void operator()() override
			{
				receiver_s r;

				cmoon::execution::set_done(r);

				cmoon::test::assert_true(r.done_set);
			}
	};

	struct receiver_default
	{
		bool done_set {false};
	};

	void set_done(receiver_default& r)
	{
		r.done_set = true;
	}

	export
	class set_done_default_fn_test : public cmoon::test::test_case
	{
		
		public:
			set_done_default_fn_test()
				: cmoon::test::test_case{"set_done_default_fn_test"} {}

			void operator()() override
			{
				receiver_default r;

				cmoon::execution::set_done(r);

				cmoon::test::assert_true(r.done_set);
			}
	};
}