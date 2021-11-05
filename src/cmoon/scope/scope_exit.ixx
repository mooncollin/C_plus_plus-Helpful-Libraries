export module cmoon.scope.scope_exit;

import <type_traits>;
import <concepts>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<class EF>
		requires((cmoon::function_object<EF> || cmoon::function_object<std::remove_reference_t<EF>> || std::is_function_v<std::remove_reference_t<EF>>) &&
				 std::invocable<std::remove_reference_t<EF>>)
	class scope_exit
	{
		public:
			template<class Fn>
				requires(std::is_lvalue_reference_v<Fn> && !std::is_same_v<std::remove_cvref_t<Fn>, scope_exit> && std::is_constructible_v<EF, Fn> && std::invocable<Fn>)
			explicit scope_exit(Fn&& fn) noexcept(std::is_nothrow_constructible_v<EF, Fn&>)
				: fn_{fn}, active_{true} {}

			template<class Fn>
				requires(!std::is_lvalue_reference_v<Fn> && !std::is_same_v<std::remove_cvref_t<Fn>, scope_exit> && std::is_constructible_v<EF, Fn> && std::invocable<Fn>)
			explicit scope_exit(Fn&& fn) noexcept(std::is_nothrow_constructible_v<EF, Fn>)
				: fn_{std::forward<Fn>(fn)}, active_{true} {}

			scope_exit(scope_exit&& other) noexcept
				requires(std::is_nothrow_move_constructible_v<EF>)
				: fn_{std::move<EF>(other.fn_)}, active_{other.active_}
			{
				other.release();
			}

			scope_exit(scope_exit&& other) noexcept(std::is_nothrow_copy_constructible_v<EF>)
				requires(std::is_copy_constructible_v<EF> && !std::is_nothrow_move_constructible_v<EF>)
				: fn_{other.fn_}, active_{other.active_}
			{
				other.release();
			}

			scope_exit(const scope_exit&) = delete;
			scope_exit& operator=(const scope_exit&) = delete;
			scope_exit& operator=(scope_exit&&) = delete;

			~scope_exit() noexcept
			{
				if (active_)
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
	};

	export
	template<class EF>
	scope_exit(EF) -> scope_exit<EF>;
}