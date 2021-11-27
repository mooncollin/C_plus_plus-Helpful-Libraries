export module cmoon.tests.execution.set_error;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class set_error_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			friend void tag_invoke(cmoon::execution::set_error_t, receiver_s& r, int v) noexcept
			{
				r.internal_v = v;
			}

			int internal_v {};
		};

		public:
			set_error_test()
				: cmoon::test::test_case{"set_error_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				receiver_s r;

				cmoon::execution::set_error(r, expected);

				cmoon::test::assert_equal(r.internal_v, expected);
			}
	};
}