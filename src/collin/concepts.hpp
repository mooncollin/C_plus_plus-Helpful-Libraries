#pragma once

#include <type_traits>

#include "collin/type_traits.hpp"

namespace collin
{
	namespace concepts
	{
		template<class T, class U>
		concept same = std::is_same_v<T, U>;

		template<class From, class To>
		concept convertible_to = std::is_convertible_v<From, To> &&
			requires(From (&f)())
		{
			static_cast<To>(f());
		};

		template<class T, class U>
		concept derived_from = std::is_base_of_v<U, T> &&
							   convertible_to<std::remove_cv_t<T>*, std::remove_cv_t<U>*>;

		template<class T, class U>
		concept common_reference = same<collin::type_traits::common_reference_t<T, U>, collin::type_traits::common_reference_t<U, T>> &&
								   convertible_to<T, collin::type_traits::common_reference_t<T, U>> &&
								   convertible_to<U, collin::type_traits::common_reference_t<T, U>>;

		template<class T, class U>
		concept common = same<collin::type_traits::common_type_t<T, U>, collin::type_traits::common_type_t<U, T>> &&
						 convertible_to<T, collin::type_traits::common_type_t<T, U>> &&
						 convertible_to<U, collin::type_traits::common_type_t<T, U>> &&
						 common_reference<
							std::add_lvalue_reference_t<const T>,
							std::add_lvalue_reference_t<const U>
						 > &&
						 common_reference<
							std::add_lvalue_reference_t<collin::type_traits::common_type_t<T, U>>,
							collin::type_traits::common_reference_t<
								std::add_lvalue_reference_t<const T>,
							std::add_lvalue_reference_t<const U>
							>
						 >;

		template<class T>
		concept integral = std::is_integral_v<T>;

		template<class T>
		concept signed_integral = integral<T> && std::is_signed_v<T>;

		template<class T>
		concept unsigned_integral = integral<T> && !signed_integral<T>;

		template<class T>
		concept arithmetic = std::is_arithmetic_v<T>;

		template<class T>
		concept floating_point = std::is_floating_point_v<T>;

		template<class T, class U>
		concept assignable = std::is_assignable_v<T, U>;

		template<class T>
		concept swappable = 
			requires(T& a, T& b)
		{
			{ std::swap(a, b) };
		};

		template<class T, class U>
		concept swappable_with = common_reference<
									const std::remove_reference_t<T>&,
									const std::remove_reference_t<U>&
								 > &&
			requires(T&& t, U&& u)
		{
			{ std::swap(std::forward<T>(t), std::forward<T>(t)) };
			{ std::swap(std::forward<U>(u), std::forward<U>(u)) };
			{ std::swap(std::forward<T>(t), std::forward<U>(u)) };
			{ std::swap(std::forward<U>(u), std::forward<T>(t)) };
		};

		template<class T>
		concept destructible = std::is_nothrow_destructible_v<T>;

		template<class T, class... Args>
		concept constructible = destructible<T> && std::is_constructible_v<T, Args...>;

		template<class T>
		concept default_constructible = constructible<T>;

		template<class T>
		concept move_constructible = constructible<T, T> && convertible_to<T, T>;

		template<class T>
		concept copy_constructible = move_constructible<T> &&
									 constructible<T, T&> && convertible_to<T&, T> &&
									 constructible<T, const T&> && convertible_to<const T&, T> &&
									 constructible<T, const T> && convertible_to<const T, T>;

		template<class T>
		concept movable = std::is_object_v<T> &&
						  move_constructible<T> &&
						  assignable<T&, T> &&
						  swappable<T>;

		template<class T>
		concept copyable = copy_constructible<T> &&
						   movable<T> &&
						   assignable<T&, const T&>;

		template<class T>
		concept semi_regular = copyable<T> && default_constructible<T>;

		template<class F, class... Args>
		concept invocable = std::is_invocable_v<F, Args...>;

		template<class F, class... Args>
		concept regular_invocable = invocable<F, Args...>;

		template<class B>
		concept boolean = movable<std::decay_t<B>> &&
			requires(const std::remove_reference_t<B>& b1, const std::remove_reference_t<B>& b2, const bool a)
		{
			{ b1 }			-> convertible_to<bool>;
			{ !b1 }			-> convertible_to<bool>;
			{ b1 && a }		-> same<bool>;
			{ b1 || a }		-> same<bool>;
			{ b1 && b2 }	-> same<bool>;
			{ a && b2 }		-> same<bool>;
			{ b1 || b2 }	-> same<bool>;
			{ a || b2 }		-> same<bool>;
			{ b1 == b2 }	-> convertible_to<bool>;
			{ b1 == a }		-> convertible_to<bool>;
			{ a == b2 }		-> convertible_to<bool>;
			{ b1 != b2 }	-> convertible_to<bool>;
			{ b1 != a }		-> convertible_to<bool>;
			{ a != b2 }		-> convertible_to<bool>;
		};

		template<class T, class U>
		concept weakly_equality_comparable_with = 
			requires(const std::remove_reference_t<T>& t, const std::remove_reference_t<U>& u)
		{
			{ t == u } -> boolean;
			{ t != u } -> boolean;
			{ u == t } -> boolean;
			{ u != t } -> boolean;
		};

		template<class T>
		concept equality_comparable = weakly_equality_comparable_with<T, T>;

		template<class T, class U>
		concept equality_comparable_with = equality_comparable<T> &&
										   equality_comparable<U> &&
										   common_reference<
												const std::remove_reference_t<T>&,
												const std::remove_reference_t<U>&
										   > &&
										   equality_comparable<
												collin::type_traits::common_reference_t<
													const std::remove_reference_t<T>&,
													const std::remove_reference_t<U>&
												>
										   > &&
										   weakly_equality_comparable_with<T, U>;

		template<class T>
		concept strict_totally_ordered = equality_comparable<T> &&
			requires(const std::remove_reference_t<T>& a, const std::remove_reference_t<T>& b)
		{
			{ a < b }	-> boolean;
			{ a > b }	-> boolean;
			{ a <= b }	-> boolean;
			{ a >= b }	-> boolean;
		};

		template<class T, class U>
		concept strict_totally_ordered_with = strict_totally_ordered<T> &&
											  strict_totally_ordered<U> &&
											  common_reference<
													const std::remove_reference_t<T>&,
													const std::remove_reference_t<U>&
											  > &&
											  strict_totally_ordered<
													collin::type_traits::common_reference_t<
														const std::remove_reference_t<T>&,
														const std::remove_reference_t<U>&
													>
											  > &&
											  equality_comparable_with<T, U> &&
			requires(const std::remove_reference_t<T>& t, const std::remove_reference_t<U>& u)
		{
			{ t < u }	-> boolean;
			{ t > u }	-> boolean;
			{ t <= u }	-> boolean;
			{ t >= u }	-> boolean;
			{ u < t }	-> boolean;
			{ u > t }	-> boolean;
			{ u <= t }	-> boolean;
			{ u >= t }	-> boolean;
		};

		template<class T>
		concept regular = semi_regular<T> && equality_comparable<T>;

		template<class F, class... Args>
		concept predicate = regular_invocable<F, Args...> &&
							boolean<std::invoke_result_t<F(Args...)>>;

		template<class R, class T, class U>
		concept relation = predicate<R, T, T> &&
						   predicate<R, U, U> &&
						   common_reference<
								const std::remove_reference_t<T>&,
								const std::remove_reference_t<U>&
						   > &&
						   predicate<R,
								collin::type_traits::common_reference_t<
									const std::remove_reference_t<T>&,
									const std::remove_reference_t<U>&
								>,
								collin::type_traits::common_reference_t<
									const std::remove_reference_t<T>&,
									const std::remove_reference_t<U>&
								>
						   > &&
						   predicate<R, T, U> &&
						   predicate<R, U, T>;
		
		template<class R, class T, class U>
		concept strict_weak_order = relation<R, T, U>;

		template<class T>
		concept iterator = collin::type_traits::is_iterator_v<T>;

		template<class T>
		concept complete_type = 
			requires
		{
			sizeof(T);
		};

		template<class T, class S>
		concept stream_writable =
			requires(const T& t, S& s)
		{
			{ s << t };
		};
	}
}