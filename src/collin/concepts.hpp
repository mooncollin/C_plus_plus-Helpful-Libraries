#pragma once

#include <type_traits>

#include "collin/type_traits.hpp"

namespace collin
{
	namespace concepts
	{
		template<class T>
		concept object = std::is_object_v<T>;

		template<class T>
		concept array = std::is_array_v<T>;

		template<class T, class U>
		concept same = std::is_same_v<T, U> && std::is_same_v<U, T>;

		template<class From, class To>
		concept convertible_to = std::is_convertible_v<From, To>;

		template<class T, class U>
		concept derived_from = std::is_base_of_v<U, T>;

		template<class T, class U>
		concept common_reference = same<collin::type_traits::common_reference_t<T, U>, collin::type_traits::common_reference_t<U, T>> &&
								   convertible_to<T, collin::type_traits::common_reference_t<T, U>> &&
								   convertible_to<U, collin::type_traits::common_reference_t<T, U>>;

		template<class T, class U>
		concept common =
			requires(T t, U u)
		{
			typename collin::type_traits::common_type_t<T, U>;
			typename collin::type_traits::common_type_t<U, T>;
			same<collin::type_traits::common_type_t<U, T>, collin::type_traits::common_type_t<T, U>>;
			collin::type_traits::common_type_t<T, U>(std::forward<T>(t));
			collin::type_traits::common_type_t<T, U>(std::forward<U>(u));
		};

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

		template<class T>
		concept integer_like = integral<T> || same<T, long long> || same<T, unsigned long long>;

		template<class T>
		concept signed_integer_like = signed_integral<T> || same<T, long long>;

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
		concept default_constructible = constructible<T> &&
			requires
		{
			T{};
			::new (static_cast<void*>(nullptr)) T;
		};

		template<class T>
		concept move_constructible = constructible<T, T> && convertible_to<T, T>;

		template<class T>
		concept copy_constructible = move_constructible<T> &&
									 constructible<T, T&> && convertible_to<T&, T> &&
									 constructible<T, const T&> && convertible_to<const T&, T> &&
									 constructible<T, const T> && convertible_to<const T, T>;

		template<class T>
		concept movable = object<T> &&
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
		concept referenceable = object<T> ||
								std::is_function_v<T> ||
								std::is_reference_v<T>;

		/*template<class T>
		concept iterator = concepts::copyable<T> &&
						   concepts::swappable<T> &&
			requires(T it)
		{
			typename std::iterator_traits<T>::value_type;
			typename std::iterator_traits<T>::difference_type;
			typename std::iterator_traits<T>::reference;
			typename std::iterator_traits<T>::pointer;
			typename std::iterator_traits<T>::iterator_category;

			{ *it }  -> concepts::referenceable;
			{ ++it } -> concepts::convertible_to<T&>;
			{ *it++ } -> concepts::referenceable;
		};

		template<class T>
		concept input_iterator = iterator<T> &&
								 concepts::equality_comparable<T> &&
			requires(T it, const T const_it)
		{
			{ it != const_it }  -> concepts::boolean;
			{ it.operator->() };
		};

		template<class T, class U>
		concept output_iterator = iterator<T> &&
								  (std::is_pointer_v<T> || std::is_class_v<T>) &&
			requires(T it, U&& u)
		{
			{ ++it } -> concepts::convertible_to<T&>;
			{ it++ } -> concepts::convertible_to<const T&>;
			{ *it++ = std::forward<U>(u) };
			{ *it = std::forward<U>(u) };
		};

		template<class T>
		concept forward_iterator = input_iterator<T> &&
								   concepts::default_constructible<T> &&
								   ((output_iterator<T, std::add_lvalue_reference_t<std::iterator_traits<T>::value_type>> && concepts::same<std::iterator_traits<T>::reference, std::add_lvalue_reference_t<std::iterator_traits<T>::value_type>>) ||
									concepts::same<std::iterator_traits<T>::reference, const std::add_lvalue_reference_t<std::iterator_traits<T>::value_type>>) &&
			requires(T it)
		{
			{ it++ }  -> concepts::same<T>;
			{ *it++ } -> concepts::same<std::iterator_traits<T>::reference>;
		};

		template<class T>
		concept bidirectional_iterator = forward_iterator<T> &&
			requires(T a)
		{
			{ --a }  -> concepts::same<T&>;
			{ a-- }  -> concepts::convertible_to<const T&>;
			{ *a-- } -> concepts::same<std::iterator_traits<T>::reference>;
		};

		template<class T>
		concept random_access_iterator = bidirectional_iterator<T> &&
			requires(T a, const T b, T& r, typename std::iterator_traits<T>::difference_type n)
		{
			{ r += n } -> concepts::same<T&>;
			{ a + n }  -> concepts::same<T>;
			{ n + a }  -> concepts::same<T>;
			{ r -= n } -> concepts::same<T&>;
			{ a - n }  -> concepts::same<T>;
			{ b - a }  -> concepts::same<std::iterator_traits<T>::difference_type>;
			{ a[n] }   -> concepts::convertible_to<std::iterator_traits<T>::reference>;
			{ a < b }  -> concepts::boolean;
			{ a > b }  -> concepts::boolean;
			{ a >= b } -> concepts::boolean;
			{ a <= b } -> concepts::boolean;
		};

		template<class T>
		concept contiguous_iterator = random_access_iterator<T> &&
			requires(T a, typename std::iterator_traits<T>::difference_type n)
		{
			{ *(a + n) } -> concepts::same<decltype(*(std::addressof(*a) + n))>;
		};*/

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