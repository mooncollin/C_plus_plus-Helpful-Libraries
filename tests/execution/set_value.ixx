export module cmoon.tests.execution.set_value;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class set_value_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			friend void tag_invoke(cmoon::execution::set_value_t, receiver_s& r, int v)
			{
				r.internal_v = v;
			}

			int internal_v {};
		};

		public:
			set_value_test()
				: cmoon::test::test_case{"set_value_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				receiver_s r;

				cmoon::execution::set_value(r, expected);

				cmoon::test::assert_equal(r.internal_v, expected);
			}
	};
}