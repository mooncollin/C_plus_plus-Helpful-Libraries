export module cmoon.tests.execution.execute;

import <utility>;
import <functional>;
import <concepts>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class execute_member_fn_test : public cmoon::test::test_case
	{
		struct executor_s
		{
			template<class F>
			void execute(F&& f) const
			{
				std::invoke(std::forward<F>(f));
			}

			[[nodiscard]] constexpr friend bool operator==(const executor_s&, const executor_s&) noexcept = default;
		};

		public:
			execute_member_fn_test()
				: cmoon::test::test_case{"execute_member_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				executor_s e;

				int v;

				cmoon::execution::execute(e, [&v, expected]{ v = expected; });

				cmoon::test::assert_equal(v, expected);
			}
	};

	struct executor_default
	{
		[[nodiscard]] constexpr friend bool operator==(const executor_default&, const executor_default&) noexcept = default;
	};

	template<class F>
	void execute(const executor_default&, F&& f)
	{
		std::invoke(std::forward<F>(f));
	}

	export
	class execute_default_fn_test : public cmoon::test::test_case
	{
		public:
			execute_default_fn_test()
				: cmoon::test::test_case{"execute_default_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				executor_default e;

				int v {0};

				cmoon::execution::execute(e, [&v, expected]{ v = expected; });

				cmoon::test::assert_equal(v, expected);
			}
	};

	export
	class execute_submit_test : public cmoon::test::test_case
	{
		struct sender_s : public cmoon::execution::sender_base
		{
			template<class R>
			struct op_state
			{
				R r_;

				void start() noexcept
				{
					cmoon::execution::set_value(std::move(r_));
				}
			};

			template<class R>
			op_state<R> connect(R&& r) const
			{
				return op_state<R>{std::forward<R>(r)};
			}

			[[nodiscard]] constexpr friend bool operator==(const sender_s&, const sender_s&)
			{
				return true;
			}

			[[nodiscard]] constexpr friend bool operator!=(const sender_s&, const sender_s&)
			{
				return false;
			}
		};

		static_assert(cmoon::execution::sender<sender_s>);

		public:
			execute_submit_test()
				: cmoon::test::test_case{"execute_submit_test"} {}

			void operator()() override
			{
				sender_s s;

				bool value {false};

				cmoon::execution::execute(s, [&value] { value = true; });
				
				cmoon::test::assert_true(value);
			}
	};
}