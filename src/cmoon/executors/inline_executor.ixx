export module cmoon.executors.inline_executor;

import <functional>;
import <type_traits>;
import <cstddef>;
import <concepts>;

import cmoon.execution;

namespace cmoon::executors
{
	struct simd_executor;

	export
	struct inline_executor
	{
		using shape_type = std::size_t;
		using index_type = std::size_t;

		template<class F>
			requires(std::is_invocable_r_v<void, F>)
		constexpr void execute(F&& f) const noexcept(std::is_nothrow_invocable_v<F>)
		{
			std::invoke(std::forward<F>(f));
		}

		template<class F>
			requires(std::is_invocable_r_v<void, F, index_type>)
		constexpr void bulk_execute(F&& f, index_type n) const noexcept(std::is_nothrow_invocable_v<F, index_type>)
		{
			for (index_type i {0}; i != n; ++i)
			{
				std::invoke(f, n);
			}
		}

		constexpr inline_executor require(const cmoon::execution::bulk_guarantee_t::sequenced_t) const noexcept
		{
			return *this;
		}

		constexpr simd_executor require(const cmoon::execution::bulk_guarantee_t::unsequenced_t) const noexcept;

		[[nodiscard]] static constexpr cmoon::execution::blocking_t query(const cmoon::execution::blocking_t) noexcept
		{
			return cmoon::execution::blocking.always;
		}

		[[nodiscard]] static constexpr cmoon::execution::bulk_guarantee_t query(const cmoon::execution::bulk_guarantee_t) noexcept
		{
			return cmoon::execution::bulk_guarantee.sequenced;
		}

		[[nodiscard]] constexpr friend bool operator==(const inline_executor&, const inline_executor&) noexcept
		{
			return true;
		}

		[[nodiscard]] constexpr friend bool operator!=(const inline_executor&, const inline_executor&) noexcept = default;
	};

	export
	struct simd_executor : public inline_executor
	{
		using index_type = typename inline_executor::index_type;
		using shape_type = typename inline_executor::shape_type;

		template<class F>
		void bulk_execute(F&& f, index_type n) const noexcept(std::is_nothrow_invocable_v<F, index_type>)
		{
			#pragma omp simd
			for (index_type i{0}; i != n; ++i)
			{
				std::invoke(f, i);
			}
		}

		[[nodiscard]] constexpr friend bool operator==(const simd_executor&, const simd_executor&) noexcept
		{
			return true;
		}

		[[nodiscard]] constexpr friend bool operator!=(const simd_executor&, const simd_executor&) noexcept = default;
	};

	constexpr simd_executor inline_executor::require(const cmoon::execution::bulk_guarantee_t::unsequenced_t) const noexcept
	{
		return simd_executor{};
	}

	static_assert(cmoon::execution::executor<inline_executor>);
	static_assert(cmoon::execution::executor<simd_executor>);
}