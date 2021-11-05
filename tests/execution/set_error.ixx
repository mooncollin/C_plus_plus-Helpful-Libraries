export module cmoon.tests.execution.set_error;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class set_error_member_fn_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			void set_error(int v)
			{
				internal_v = v;
			}

			int internal_v {};
		};

		public:
			set_error_member_fn_test()
				: cmoon::test::test_case{"set_error_member_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				receiver_s r;

				cmoon::execution::set_error(r, expected);

				cmoon::test::assert_equal(r.internal_v, expected);
			}
	};

	struct receiver_default
	{
		int internal_v{};
	};

	void set_error(receiver_default& r, int v)
	{
		r.internal_v = v;
	}

	export
	class set_error_default_fn_test : public cmoon::test::test_case
	{
		
		public:
			set_error_default_fn_test()
				: cmoon::test::test_case{"set_error_default_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				receiver_default r;

				cmoon::execution::set_error(r, expected);

				cmoon::test::assert_equal(r.internal_v, expected);
			}
	};
}