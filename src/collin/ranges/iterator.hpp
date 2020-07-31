#pragma once

#include <iterator>

#include "collin/concepts.hpp"
#include "collin/ranges/ranges.hpp"
#include "collin/type_traits.hpp"

namespace collin
{
	namespace ranges
	{
		template<class I>
		constexpr auto iter_move(I& i) noexcept;

		template<class T>
		void begin(T& t);

		template<class T>
		void end(T& t);
	}

	namespace iterators
	{
		template<class I>
		struct value_type {};

		template<class T>
		struct value_type<T*> {};

		template<concepts::object T>
		struct value_type<T*>
		{
			using type = typename std::remove_cv_t<T>;
		};

		template<concepts::array I>
		struct value_type<I> : value_type<std::decay_t<I>> {};

		template<class T>
		struct value_type<const T> : value_type<std::decay_t<T>> {};

		namespace details
		{
			template<class T>
			concept has_value_type =
				requires
			{
				typename T::value_type;
			};

			template<class T>
			concept has_element_type = 
				requires
			{
				typename T::element_type;
			};

			template<class T>
			concept has_difference_type =
				requires
			{
				typename T::difference_type;
			};
		}

		template<details::has_value_type T>
			requires(concepts::object<typename T::value_type>)
		struct value_type<T>
		{
			using type = typename T::value_type;
		};

		template<details::has_element_type T>
			requires(concepts::object<typename T::element_type>)
		struct value_type<T>
		{
			using type = typename std::remove_cv_t<typename T::element_type>;
		};

		template<class I>
		struct indirectly_readable_traits {};

		template<concepts::object T>
		struct indirectly_readable_traits<T*>
		{
			using value_type = typename std::remove_cv_t<T>;
		};

		template<concepts::array I>
		struct indirectly_readable_traits<I>
		{
			using value_type = typename std::remove_cv_t<std::remove_extent_t<I>>;
		};

		template<class T>
		using value_type_t = typename value_type<T>::type;

		template<class T>
		struct indirectly_readable_traits<const T> : value_type<T> {};

		template<details::has_value_type T>
			requires(concepts::object<typename T::value_type>)
		struct indirectly_readable_traits<T>
		{
			using value_type = typename std::remove_cv_t<typename T::value_type>;
		};

		template<details::has_element_type T>
			requires(concepts::object<typename T::element_type>)
		struct indirectly_readable_traits<T>
		{
			using value_type = typename std::remove_cv_t<typename T::element_type>;
		};

		template<class I>
		struct incrementable_traits {};

		template<concepts::object T>
		struct incrementable_traits<T*>
		{
			using difference_type = std::ptrdiff_t;
		};

		template<class T>
		struct incrementable_traits<const T> : incrementable_traits<T> {};

		template<details::has_difference_type T>
		struct incrementable_traits<T>
		{
			using difference_type = typename T::difference_type;
		};

		namespace details
		{
			template<class T>
			concept complex_incrementable_traits = !has_difference_type<T> &&
				requires(const T& a, const T& b)
			{
				{ a - b } -> concepts::integral;
			};
		}

		template<details::complex_incrementable_traits T>
		struct incrementable_traits<T>
		{
			using difference_type = typename std::make_signed_t<decltype(std::declval<T>() - std::declval<T>())>;
		};

		namespace details
		{
			template<class T>
			concept has_iterator_traits =
				requires
			{
				typename std::iterator_traits<std::remove_cvref_t<T>>::value_type;
			} && concepts::complete_type<std::iterator_traits<std::remove_cvref_t<T>>>;

			template<class T>
			struct iter_value_t_impl
			{
				using type = typename indirectly_readable_traits<std::remove_cvref_t<T>>::value_type;
			};

			template<has_iterator_traits T>
			struct iter_value_t_impl<T>
			{
				using type = typename std::iterator_traits<std::remove_cvref_t<T>>::value_type;
			};

			template<class T>
			struct iter_difference_t_impl
			{
				using difference = typename incrementable_traits<std::remove_cvref_t<T>>::difference_type;
			};

			template<has_iterator_traits T>
			struct iter_difference_t_impl<T>
			{
				using difference = typename std::iterator_traits<std::remove_cvref_t<T>>::difference_type;
			};
		}

		template<class T>
		using iter_value_t = typename details::iter_value_t_impl<T>::type;

		template<concepts::referenceable T>
		using iter_reference_t = decltype(*std::declval<T&>());

		template<class T>
		using iter_difference_t = typename details::iter_difference_t_impl<T>::difference;

		template<concepts::referenceable T>
		using iter_rvalue_reference_t = decltype(ranges::iter_move(std::declval<T&>()));

		namespace details
		{
			template<class In>
			concept indirectly_readable_impl =
				requires(const In in)
			{
				typename iter_value_t<In>;
				typename iter_reference_t<In>;
				typename iter_rvalue_reference_t<In>;
				{ *in } -> concepts::same<iter_reference_t<In>>;
				{ ranges::iter_move(in) } -> concepts::same<iter_rvalue_reference_t<In>>;
			} &&
			concepts::common_reference<iter_reference_t<In>&&, iter_value_t<In>&> &&
			concepts::common_reference<iter_reference_t<In>&&, iter_rvalue_reference_t<In>> &&
			concepts::common_reference<iter_rvalue_reference_t<In>&&, const iter_value_t<In>&>;
		}

		template<class In>
		concept indirectly_readable = details::indirectly_readable_impl<std::remove_cvref_t<In>>;

		template<indirectly_readable T>
		using iter_common_reference_t = type_traits::common_reference_t<iter_reference_t<T>, iter_value_t<T>&>;

		template<class I>
		concept weakly_incrementable = concepts::default_constructible<I> &&
									   concepts::movable<I> &&
			requires(I i)
		{
			typename iter_difference_t<I>;
			requires concepts::signed_integer_like<iter_difference_t<I>>;
			{ ++i } -> concepts::same<I&>;
			i++;
		};

		template<class I>
		concept weakly_decrementable = concepts::default_constructible<I> &&
									   concepts::movable<I> &&
			requires(I i)
		{
			typename iter_difference_t<I>;
			{ --i } -> concepts::same<I&>;
			i--;
		};

		template<class I>
		concept incrementable = concepts::regular<I> &&
								weakly_incrementable<I> &&
			requires(I i)
		{
			{ i++ } -> concepts::same<I>;
		};

		template<class I>
		concept decrementable = concepts::regular<I> &&
								weakly_decrementable<I> &&
			requires(I i)
		{
			{ i-- } -> concepts::same<I>;
		};

		template<class I>
		concept advanceable =
			requires
		{
			typename iter_difference_t<I>;
		} && requires(I& i, iter_difference_t<I> n)
		{
			{ i + n } -> concepts::same<I>;
		};

		template<class I>
		concept input_or_output_iterator = 
			requires(I i)
		{
			{ *i } -> concepts::referenceable;
		} && weakly_incrementable<I>;
	}
}