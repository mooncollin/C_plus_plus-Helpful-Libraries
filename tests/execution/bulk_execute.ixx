export module cmoon.tests.execution.bulk_execute;

import <atomic>;

import cmoon.execution;
import cmoon.property;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class bulk_execute_member_fn_test : public cmoon::test::test_case
	{
		struct bulk_s
		{
			template<class F, class N>
			void bulk_execute(F&& f, N n)
			{
				for (N i {0}; i < n; ++i)
				{
					f(i);
				}
			}
		};

		public:
			bulk_execute_member_fn_test()
				: cmoon::test::test_case{"bulk_execute_member_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				bulk_s b;
				int value {0};

				cmoon::execution::bulk_execute(b, [&value](auto) { ++value; }, expected);
				cmoon::test::assert_equal(value, expected);
			}
	};

	struct bulk_default_s {};

	template<class F, class N>
	void bulk_execute(bulk_default_s&, F&& f, N n)
	{
		for (N i {0}; i < n; ++i)
		{
			f(i);
		}
	}

	export
	class bulk_execute_default_fn_test : public cmoon::test::test_case
	{
		public:
			bulk_execute_default_fn_test()
				: cmoon::test::test_case{"bulk_execute_default_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				bulk_default_s b;
				int value {0};

				cmoon::execution::bulk_execute(b, [&value](auto) { ++value; }, expected);
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class bulk_execute_inline_fn_test : public cmoon::test::test_case
	{
		struct bulk_s
		{
			using index_type = int;
			using shape_type = int;

			template<class F>
			void execute(F&&) {}

			constexpr static cmoon::execution::bulk_guarantee_t query(const cmoon::execution::bulk_guarantee_t&) noexcept
			{
				return cmoon::execution::bulk_guarantee.unsequenced;
			}

			template<class Property>
			constexpr bulk_s require(const Property&) const noexcept
			{
				return *this;
			}

			template<class Property>
			constexpr friend bulk_s require(const bulk_s& s, const Property&) noexcept
			{
				return s;
			}

			[[nodiscard]] constexpr friend bool operator==(const bulk_s&, const bulk_s&) noexcept = default;
		};

		static_assert(cmoon::execution::executor<bulk_s>);
		static_assert(cmoon::is_applicable_property_v<bulk_s, cmoon::execution::bulk_guarantee_t>);

		public:
			bulk_execute_inline_fn_test()
				: cmoon::test::test_case{"bulk_execute_inline_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				bulk_s b;
				std::atomic<int> value {0};

				cmoon::execution::bulk_execute(b, [&value](auto) { ++value; }, expected);
				cmoon::test::assert_equal(value, expected);
			}
	};
}