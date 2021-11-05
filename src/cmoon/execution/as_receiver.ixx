export module cmoon.execution.as_receiver;

import <exception>;
import <type_traits>;
import <functional>;
import <utility>;

namespace cmoon::execution
{
	export
	template<class F, class = void>
	struct as_receiver
	{
		public:
			constexpr as_receiver(F&& f)
				: f_{std::forward<F>(f)} {}

			template<class... Args>
			void set_value(Args&&... args) noexcept(std::is_nothrow_invocable_v<F&, Args...>)
			{
				std::invoke(f_, std::forward<Args>(args)...);
			}

			[[noreturn]] void set_error(std::exception_ptr) noexcept
			{
				std::terminate();
			}

			void set_done() noexcept {}
		private:
			F f_;
	};

	export
	template<class F, class E>
	as_receiver(F&&) -> as_receiver<F, E>;
}