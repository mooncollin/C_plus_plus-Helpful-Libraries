export module cmoon.tests.execution.set_value;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class set_value_member_fn_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			void set_value(int v)
			{
				internal_v = v;
			}

			int internal_v {};
		};

		public:
			set_value_member_fn_test()
				: cmoon::test::test_case{"set_value_member_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				receiver_s r;

				cmoon::execution::set_value(r, expected);

				cmoon::test::assert_equal(r.internal_v, expected);
			}
	};

	struct receiver_default
	{
		int internal_v{};
	};

	void set_value(receiver_default& r, int v)
	{
		r.internal_v = v;
	}

	export
	class set_value_default_fn_test : public cmoon::test::test_case
	{
		
		public:
			set_value_default_fn_test()
				: cmoon::test::test_case{"set_value_default_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				receiver_default r;

				cmoon::execution::set_value(r, expected);

				cmoon::test::assert_equal(r.internal_v, expected);
			}
	};
}