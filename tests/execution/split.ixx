export module cmoon.tests.execution.split;

import <utility>;
import <atomic>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class split_single_test : public cmoon::test::test_case
	{
		struct dummy_r
		{
			friend void tag_invoke(cmoon::execution::set_value_t, dummy_r&& r, int v) noexcept
			{
				*r.v = v;
				r.v->notify_one();
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, dummy_r&&, E&&) noexcept
			{
			}

			friend void tag_invoke(cmoon::execution::set_done_t, dummy_r&&) noexcept
			{
			}

			std::atomic<int>* v;
		};

		public:
			split_single_test()
				: cmoon::test::test_case{"split_single_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};
				dummy_r r{&value};
				auto s = cmoon::execution::split(
							cmoon::execution::just(expected)
						 );

				cmoon::execution::start(
					cmoon::execution::connect(
						s,
						std::move(r)
					)
				);

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class split_multi_test : public cmoon::test::test_case
	{
		struct dummy_r
		{
			friend void tag_invoke(cmoon::execution::set_value_t, dummy_r&& r, int v) noexcept
			{
				*r.v = v;
				r.v->notify_one();
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, dummy_r&&, E&&) noexcept
			{
			}

			friend void tag_invoke(cmoon::execution::set_done_t, dummy_r&&) noexcept
			{
			}

			std::atomic<int>* v;
		};

		public:
			split_multi_test()
				: cmoon::test::test_case{"split_multi_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};
				std::atomic<int> value2 {0};
				dummy_r r{&value};
				dummy_r r2{&value2};
				auto s = cmoon::execution::split(
							cmoon::execution::just(expected)
						 );

				cmoon::execution::start(
					cmoon::execution::connect(
						s,
						std::move(r)
					)
				);

				value.wait(0);
				cmoon::test::assert_equal(value, expected);

				cmoon::execution::start(
					cmoon::execution::connect(
						s,
						std::move(r2)
					)
				);

				value2.wait(0);
				cmoon::test::assert_equal(value2, expected);
			}
	};

	export
	class split_cancel_test : public cmoon::test::test_case
	{
		struct dummy_r
		{
			friend void tag_invoke(cmoon::execution::set_value_t, dummy_r&& r, int v) noexcept
			{
				*r.v = v;
				r.v->notify_one();
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, dummy_r&&, E&&) noexcept
			{
			}

			friend void tag_invoke(cmoon::execution::set_done_t, dummy_r&&) noexcept
			{
			}

			std::atomic<int>* v;
		};

		public:
			split_cancel_test()
				: cmoon::test::test_case{"split_cancel_test"} {}

			void operator()() override
			{
				std::atomic<int> value {0};
				dummy_r r{&value};
				auto s = cmoon::execution::split(
							cmoon::execution::just(1)
						);

				{
					auto op = cmoon::execution::connect(s, std::move(r));
				}

				cmoon::test::assert_equal(value, 0);
			}
	};
}