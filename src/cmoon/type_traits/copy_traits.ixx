export module cmoon.type_traits.copy_traits;

import <type_traits>;

namespace cmoon
{
	export
	template<class From, class To>
	struct copy_const : std::conditional<std::is_const_v<From>, std::add_const_t<To>, std::remove_const_t<To>> {};

	export
	template<class From, class To>
	struct copy_volatile : std::conditional<std::is_volatile_v<From>, std::add_volatile_t<To>, std::remove_volatile_t<To>>{};

	export
	template<class From, class To>
	using copy_const_t = typename copy_const<From, To>::type;

	export
	template<class From, class To>
	using copy_volatile_t = typename copy_volatile<From, To>::type;

	export
	template<class From, class To>
	struct copy_cv : copy_const<From, copy_volatile_t<From, To>> {};

	export
	template<class From, class To>
	using copy_cv_t = typename copy_cv<From, To>::type;

	export
	template<class From, class To>
	struct copy_reference :
		std::conditional<
			std::is_rvalue_reference_v<From>,
			std::add_rvalue_reference_t<To>,
			std::conditional_t<
				std::is_lvalue_reference_v<From>,
				std::add_lvalue_reference_t<To>,
				std::remove_reference_t<To>
			>
		> {};

	export
	template<class From, class To>
	using copy_reference_t = typename copy_reference<From, To>::type;

	export
	template<class From, class To>
	struct copy_extent : std::type_identity<To> {};

	export
	template<class From, class To>
		requires(std::rank_v<From> > 0 &&
				 std::extent_v<From> > 0)
	struct copy_extent<From, To> : std::type_identity<To[std::extent_v<From>]> {};

	export
	template<class From, class To>
		requires(std::rank_v<From> > 0 &&
				 std::extent_v<From> == 0)
	struct copy_extent<From, To> : std::type_identity<To[]> {};

	export
	template<class From, class To>
	using copy_extent_t = typename copy_extent<From, To>::type;

	export
	template<class From, class To>
	struct copy_all_extents : std::type_identity<To> {};

	export
	template<class From, class To>
		requires(std::rank_v<From> > 0)
	struct copy_all_extents<From, To> : copy_extent<From,
													typename copy_all_extents<std::remove_extent_t<From>, To>::type> {};

	export
	template<class From, class To>
	using copy_all_extents_t = typename copy_all_extents<From, To>::type;

	export
	template<class T>
	struct remove_all_pointers : std::type_identity<T> {};

	export
	template<class T>
		requires(std::is_pointer_v<T>)
	struct remove_all_pointers<T> : remove_all_pointers<std::remove_pointer_t<T>> {};

	template<class From, class To>
	struct copy_pointer :
		std::conditional<
			std::is_pointer_v<From>,
			copy_cv_t<From, std::add_pointer_t<To>>,
			std::remove_pointer_t<To>
		> {};

	export
	template<class T>
	using remove_all_pointers_t = typename remove_all_pointers<T>::type;

	export
	template<class From, class To>
	using copy_pointer_t = typename copy_pointer<From, To>::type;

	template<class From, class To>
	struct copy_all_pointers : std::type_identity<To> {};

	template<class From, class To>
		requires(std::is_pointer_v<From>)
	struct copy_all_pointers<From, To> :
		copy_pointer<From,
			typename copy_all_pointers<
				std::remove_pointer_t<From>,
				To
			>::type
		> {};

	export
	template<class From, class To>
	using copy_all_pointers_t = typename copy_all_pointers<From, To>::type;

	export
	template<class From, class To>
	struct copy_cvref : copy_reference<From,
									   copy_cv_t<std::remove_reference_t<From>, To>> {};

	export
	template<class From, class To>
	using copy_cvref_t = typename copy_cvref<From, To>::type;
}