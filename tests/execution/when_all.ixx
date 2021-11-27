export module cmoon.tests.execution.when_all;

import <exception>;
import <variant>;

import cmoon.execution;
import cmoon.test;
import cmoon.meta;

namespace cmoon::tests::execution
{
	struct receiver_s
	{
		friend void tag_invoke(cmoon::execution::set_value_t, receiver_s&& r, int v1, int v2, int v3)
		{
			r.value = v1 + v2 + v3;
		}

		friend void tag_invoke(cmoon::execution::set_value_t, receiver_s&&)
		{

		}

		friend void tag_invoke(cmoon::execution::set_value_t, receiver_s&& r, int v)
		{
			r.value = v;
		}

		template<class E>
		friend void tag_invoke(cmoon::execution::set_error_t, receiver_s&& r, E&&) noexcept
		{
			r.error_set = true;
		}

		friend void tag_invoke(cmoon::execution::set_done_t, receiver_s&& r) noexcept
		{
			r.done_set = true;
		}

		int value {0};
		bool error_set{false};
		bool done_set{false};
	};

	export
	class when_all_single_value_test : public cmoon::test::test_case
	{
		public:
			when_all_single_value_test()
				: cmoon::test::test_case{"when_all_single_value_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1)
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 1);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class when_all_single_error_test : public cmoon::test::test_case
	{
		public:
			when_all_single_error_test()
				: cmoon::test::test_case{"when_all_single_error_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just_error(std::exception{})
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);
				cmoon::test::assert_true(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class when_all_single_done_test : public cmoon::test::test_case
	{
		public:
			when_all_single_done_test()
				: cmoon::test::test_case{"when_all_single_done_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just_done()
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_true(r.done_set);
			}
	};

	export
	class when_all_value_test : public cmoon::test::test_case
	{
		public:
			when_all_value_test()
				: cmoon::test::test_case{"when_all_value_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just(2),
					cmoon::execution::just(3)
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 6);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class when_all_error_test : public cmoon::test::test_case
	{
		public:
			when_all_error_test()
				: cmoon::test::test_case{"when_all_error_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just(2),
					cmoon::execution::just_error(std::exception{})
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);
				cmoon::test::assert_true(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class when_all_done_test : public cmoon::test::test_case
	{
		public:
			when_all_done_test()
				: cmoon::test::test_case{"when_all_done_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just(2),
					cmoon::execution::just_done()
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_true(r.done_set);
			}
	};

	export
	class when_all_error_and_done_test : public cmoon::test::test_case
	{
		public:
			when_all_error_and_done_test()
				: cmoon::test::test_case{"when_all_error_and_done_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just_error(std::exception{}),
					cmoon::execution::just_done()
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);

				// Either done or error gets set, but
				// not both
				cmoon::test::assert_true(r.done_set != r.error_set);
			}
	};
}