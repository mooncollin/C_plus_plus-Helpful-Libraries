export module cmoon.tests.execution.just_done;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class just_done_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			friend void tag_invoke(cmoon::execution::set_value_t, just_r&& r, Ts&&...)
			{
				r.value_set = true;
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, just_r&& r, E&&) noexcept
			{
				r.error_set = true;
			}

			friend void tag_invoke(cmoon::execution::set_done_t, just_r&& r) noexcept
			{
				r.done_set = true;
			}

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		public:
			just_done_test()
				: cmoon::test::test_case{"just_done_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just_done();


				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_false(r.value_set);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_true(r.done_set);
			}
	};
}