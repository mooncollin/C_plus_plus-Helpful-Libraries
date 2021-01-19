#pragma once

#include <type_traits>
#include <exception>
#include "cmoon/concepts.hpp"

namespace cmoon
{
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
				: fn_{std::forward<EF>(other.fn_)}, active_{other.active_}
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

	template<class EF>
	scope_exit(EF) -> scope_exit<EF>;

	template<class EF>
		requires((cmoon::function_object<EF> || cmoon::function_object<std::remove_reference_t<EF>> || std::is_function_v<std::remove_reference_t<EF>>) &&
				 std::invocable<std::remove_reference_t<EF>>)
	class scope_success
	{
		public:
			template<class Fn>
				requires(std::is_lvalue_reference_v<Fn> && !std::is_same_v<std::remove_cvref_t<Fn>, scope_success> && std::is_constructible_v<EF, Fn> && std::invocable<Fn>)
			explicit scope_success(Fn&& fn) noexcept(std::is_nothrow_constructible_v<EF, Fn&>)
				: fn_{fn}, active_{true}, exception_counter{std::uncaught_exceptions()} {}

			template<class Fn>
				requires(!std::is_lvalue_reference_v<Fn> && !std::is_same_v<std::remove_cvref_t<Fn>, scope_success> && std::is_constructible_v<EF, Fn> && std::invocable<Fn>)
			explicit scope_success(Fn&& fn) noexcept(std::is_nothrow_constructible_v<EF, Fn>)
				: fn_{std::forward<Fn>(fn)}, active_{true}, exception_counter{std::uncaught_exceptions()} {}

			scope_success(scope_success&& other) noexcept
				requires(std::is_nothrow_move_constructible_v<EF>)
				: fn_{std::forward<EF>(other.fn_)}, active_{other.active_}, exception_counter{other.exception_counter}
			{
				other.release();
			}

			scope_success(scope_success&& other) noexcept(std::is_nothrow_copy_constructible_v<EF>)
				requires(std::is_copy_constructible_v<EF> && !std::is_nothrow_move_constructible_v<EF>)
				: fn_{other.fn_}, active_{other.active_}, exception_counter{other.exception_counter}
			{
				other.release();
			}

			scope_success(const scope_success&) = delete;
			scope_success& operator=(const scope_success&) = delete;
			scope_success& operator=(scope_success&&) = delete;

			~scope_success() noexcept(noexcept(std::declval<EF&>()()))
			{
				if (active_ && std::uncaught_exceptions() <= exception_counter)
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

	template<class EF>
	scope_success(EF) -> scope_success<EF>;

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

	template<class EF>
	scope_fail(EF) -> scope_fail<EF>;

	template<class R, class D>
		requires(cmoon::object<std::remove_reference_t<R>> && 
				 std::move_constructible<std::remove_reference_t<R>> &&
				 (std::copy_constructible<std::remove_reference_t<R>> || std::is_nothrow_move_constructible_v<std::remove_reference_t<R>>) &&
				 std::destructible<D> &&
				 std::move_constructible<D> &&
				 (std::copy_constructible<D> || std::is_nothrow_move_constructible_v<D>) &&
				 cmoon::function_object<D, std::remove_reference_t<R>>
				)
	class unique_resource
	{
		using RS = std::conditional_t<std::is_object_v<R>, R, std::reference_wrapper<std::remove_reference_t<R>>>;

		template<class RR, class DD>
		static constexpr auto establish_constructible = (std::is_constructible_v<RS, RR> || std::is_constructible_v<RS, RR&>) &&
														(std::is_constructible_v<D, DD> || std::is_constructible_v<D, DD&>);

		public:
			unique_resource() requires(std::is_default_constructible_v<R> && std::is_default_constructible_v<D>) = default;

			template<class RR, class DD>
			unique_resource(RR&& r, DD&& d) noexcept
				requires(establish_constructible<RR, DD> &&
						 (std::is_nothrow_constructible_v<RS, RR> && std::is_nothrow_constructible_v<D, DD>) ||
						 (std::is_nothrow_constructible_v<RS, RR&> && std::is_nothrow_constructible_v<D, DD&>) &&
						 requires(RR&& r, DD&& d, RS res_, D del_) { d(r); d(res_); del_(res_); })
				: res_{std::forward<RR>(r)}, del_{std::forward<DD>(d)}, owns_{true} {}

			template<class RR, class DD>
			unique_resource(RR&& r, DD&& d)
				requires(establish_constructible<RR, DD> &&
						 (std::is_nothrow_constructible_v<RS, RR> || std::is_nothrow_constructible_v<RS, RR&>) &&
						 (!std::is_nothrow_constructible_v<D, DD> || !std::is_nothrow_constructible_v<D, DD&>) &&
						 requires(RR&& r, DD&& d, RS res_, D del_) { d(r); d(res_); del_(res_); })
				 try : res_{std::forward<RR>(r)}, del_{d}, owns_{true}
				 {
				 }
				 catch(...)
				 {
					 d(res_);
				 }

			template<class RR, class DD>
			unique_resource(RR&& r, DD&& d)
				requires(establish_constructible<RR, DD> &&
						 (!std::is_nothrow_constructible_v<RS, RR> || !std::is_nothrow_constructible_v<RS, RR&>) &&
						 (std::is_nothrow_constructible_v<D, DD> || std::is_nothrow_constructible_v<D, DD&>) &&
						 requires(RR&& r, DD&& d, RS res_, D del_) { d(r); d(res_); del_(res_); })
				try : res_{r}, del_{std::forward<DD>(d)}, owns_{true}
				{
				}
				catch(...)
				{
					d(r);
				}

			template<class RR, class DD>
			unique_resource(RR&& r, DD&& d)
				requires(establish_constructible<RR, DD> &&
						 (!std::is_nothrow_constructible_v<D, DD> || !std::is_nothrow_constructible_v<D, DD&>) &&
						 (!std::is_nothrow_constructible_v<RS, RR> || !std::is_nothrow_constructible_v<RS, RR&>) &&
						 requires(RR&& r, DD&& d, RS res_, D del_) { d(r); d(res_); del_(res_); })
				try : res_{r}, del_{d}, owns_{true}
				{
				}
				catch(...)
				{
					d(r);
				}

			unique_resource(unique_resource&& other) noexcept
				requires(std::is_nothrow_move_constructible_v<RS> && std::is_nothrow_move_constructible_v<D>)
				: res_{std::move(other.res_)}, del_{std::move(other.del_)}, owns_{std::exchange(other.owns_, false)} {}

			unique_resource(unique_resource&& other)
				requires(std::is_nothrow_move_constructible_v<RS> && !std::is_nothrow_move_constructible<D>)
				try : res_{std::move(other.res_)}, del_{std::move(other.del_)}, owns_{std::exchange(other.owns_, false)}
				{
				}
				catch(...)
				{
					if (other.owns_)
					{
						other.del_(res_);
					}
				}

			unique_resource(unique_resource&& other)
				: res_{std::move(other.res_)}, del_{std::move(other.del_)}, owns_{std::exchange(other.owns_, false)} {}

			unique_resource& operator=(unique_resource&& other) noexcept
				requires(std::is_nothrow_move_assignable_v<RS> && std::is_nothrow_move_assignable_v<D>)
			{
				if (this != std::addressof(other))
				{
					reset();
					res_ = std::move(other.res_);
					del_ = std::move(other.del_);
					owns_ = std::exchange(other.owns_, false);
				}

				return *this;
			}

			unique_resource& operator=(unique_resource&& other)
				requires(std::is_nothrow_move_assignable_v<RS>)
			{
				if (this != std::addressof(other))
				{
					reset();
					del_ = std::move(other.del_);
					res_ = std::move(other.res_);
					owns_ = std::exchange(other.owns_, false);
				}

				return *this;
			}

			unique_resource& operator=(unique_resource&& other)
				requires(std::is_nothrow_move_assignable_v<D>)
			{
				if (this != std::addressof(other))
				{
					reset();
					res_ = std::move(other.res_);
					del_ = std::move(other.del_);
					owns_ = std::exchange(other.owns_, false);
				}

				return *this;
			}

			unique_resource& operator=(unique_resource&& other)
			{
				if (this != std::addressof(other))
				{
					reset();
					res_ = std::move(other.res_);
					try
					{
						del_ = std::move(other.del_);
					}
					catch (...)
					{
						other.res_ = std::move(res_); // Move it back
						throw std::current_exception();
					}
					owns_ = std::exchange(other.owns_, false);
				}

				return *this;
			}

			~unique_resource()
			{
				if (owns_)
				{
					del_(res_);
				}
			}

			void reset() noexcept
			{
				if (owns_)
				{
					del_(res_);
					owns_ = false;
				}
			}

			template<class RR>
				requires(std::is_nothrow_assignable_v<RS, RR>)
			void reset(RR&& r) noexcept
			{
				reset();
				res_ = std::forward<RR>(r);
				owns_ = true;
			}

			template<class RR>
			void reset(RR&& r)
			{
				reset();
				try
				{
					res_ = std::as_const(r);
					owns_ = true;
				}
				catch(...)
				{
					del_(r);
				}
			}

			void release() noexcept
			{
				owns_ = false;
			}

			const R& get() const noexcept
			{
				return res_;
			}

			const D& get_deleter() const noexcept
			{
				return del_;
			}

			std::add_lvalue_reference_t<std::remove_pointer_t<R>> operator*() const noexcept
				requires(std::is_pointer_v<R> && !std::is_void_v<std::remove_pointer_t<R>>)
			{
				return *res_;
			}

			R operator->() const noexcept
				requires(std::is_pointer_v<R>)
			{
				return res_;
			}
		private:
			RS res_ {};
			D del_ {};
			bool owns_ {false};

			template<class R2, class D2, class S>
				requires(requires(R2 r, S s) { { r == s } -> std::convertible_to<bool>; })
			friend auto make_unique_resource_checked(R2&& r, const S& invalid, D2&& d);// noexcept(std::is_nothrow_constructible_v<std::decay_t<R2>, R2> && std::is_nothrow_constructible_v<std::decay_t<D2>, D2>);

			struct doesnt_own_constructor_t {};
			static constexpr doesnt_own_constructor_t doesnt_own_constructor {};

			template<class RR, class DD>
			unique_resource(doesnt_own_constructor_t, RR&& r, DD&& d)
				: unique_resource{std::forward<RR>(r), std::forward<DD>(d)}
			{
				owns_ = false;
			}
	};

	template<class R, class D>
	unique_resource(R, D) -> unique_resource<R, D>;

	template<class R, class D, class S = std::decay_t<R>>
		requires(requires(R r, S s) { { r == s } -> std::convertible_to<bool>; })
	auto make_unique_resource_checked(R&& r, const S& invalid, D&& d)// noexcept(std::is_nothrow_constructible_v<std::decay_t<R>, R> && std::is_nothrow_constructible_v<std::decay_t<D>, D>) // MSVC cannot match this up with its friend. Probably a bug.
	{
		using ur_t = unique_resource<std::decay_t<R>, std::decay_t<D>>;

		if (r == invalid)
		{
			return ur_t{ur_t::doesnt_own_constructor, std::forward<R>(r), std::forward<D>(d)};
		}

		return ur_t{std::forward<R>(r), std::forward<D>(d)};
	}
}