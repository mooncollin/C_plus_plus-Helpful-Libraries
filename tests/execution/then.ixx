export module cmoon.tests.execution.then;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class then_test : public cmoon::test::test_case
	{
		struct then_r
		{
			constexpr friend void tag_invoke(cmoon::execution::set_value_t, then_r&& r, int v)
			{
				r.value = v;
			}

			template<class E>
			constexpr friend void tag_invoke(cmoon::execution::set_error_t, then_r&&, E&&) noexcept {}

			constexpr friend void tag_invoke(cmoon::execution::set_done_t, then_r&&) noexcept {}

			int value {0};
		};

		static_assert(cmoon::execution::receiver<then_r>);

		public:
			then_test()
				: cmoon::test::test_case{"then_test"} {}

			void operator()() override
			{
				constexpr int expected {12};

				constexpr auto f = [](int i1, int i2, int i3) noexcept {
					return i1 + i2 + i3;
				};

				constexpr auto f2 = [](int i) noexcept {
					return i * 2;
				};

				then_r r;

				auto s = cmoon::execution::just(1, 2, 3) |
						 cmoon::execution::then(f) |
						 cmoon::execution::then(f2);

				cmoon::execution::start(cmoon::execution::connect(std::move(s), r));

				cmoon::test::assert_equal(r.value, expected);
			}
	};
}