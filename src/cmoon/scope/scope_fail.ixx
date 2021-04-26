export module cmoon.scope.scope_fail;

import <type_traits>;
import <concepts>;
import <exception>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<class EF>
		requires((cmoon::function_object<EF> || cmoon::function_object<std::remove_reference_t<EF>> || std::is_function_v<std::remove_reference_t<EF>>) &&
				 std::invocable<std::remove_reference_t<EF>>)
	class scope_fail
	{
		public:
			template<class Fn>
				requires(std::is_lvalue_reference_v<Fn> && !std::is_same_v<std::remove_cvref_t<Fn>, scope_fail> && std::is_constructible_v<EF, Fn> && std::invocable<Fn>)
			explicit scope_fail(Fn&& fn) noexcept(std::is_nothrow_constructible_v<EF, Fn&>)
				: fn_{fn}, active_{true}, exception_counter{std::uncaught_exceptions()} {}

			template<class Fn>
				requires(!std::is_lvalue_reference_v<Fn> && !std::is_same_v<std::remove_cvref_t<Fn>, scope_fail> && std::is_constructible_v<EF, Fn> && std::invocable<Fn>)
			explicit scope_fail(Fn&& fn) noexcept(std::is_nothrow_constructible_v<EF, Fn>)
				: fn_{std::forward<Fn>(fn)}, active_{true}, exception_counter{std::uncaught_exceptions()} {}

			scope_fail(scope_fail&& other) noexcept
				requires(std::is_nothrow_move_constructible_v<EF>)
				: fn_{std::forward<EF>(other.fn_)}, active_{other.active_}, exception_counter{other.exception_counter}
			{
				other.release();
			}

			scope_fail(scope_fail&& other) noexcept(std::is_nothrow_copy_constructible_v<EF>)
				requires(std::is_copy_constructible_v<EF> && !std::is_nothrow_move_constructible_v<EF>)
				: fn_{other.fn_}, active_{other.active_}, exception_counter{other.exception_counter}
			{
				other.release();
			}

			scope_fail(const scope_fail&) = delete;
			scope_fail& operator=(const scope_fail&) = delete;
			scope_fail& operator=(scope_fail&&) = delete;

			~scope_fail() noexcept(noexcept(std::declval<EF&>()()))
			{
				if (active_ && std::uncaught_exceptions() > exception_counter)
				{
					fn_();
				}
			}

			void release() noexcept
			{
				active_ = false;
			}
		private:
			EF fn_;
			bool active_;
			int exception_counter;
	};

	export
	template<class EF>
	scope_fail(EF) -> scope_fail<EF>;
}