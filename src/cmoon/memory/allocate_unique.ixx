export module cmoon.memory.allocate_unique;

import <memory>;
import <utility>;
import <type_traits>;
import <functional>;

namespace cmoon
{
	export
	template<class T, class Alloc>
	class allocator_delete
	{
		public:
			using allocator_type = std::remove_cv_t<Alloc>;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;

			template<class OtherAlloc>
			constexpr allocator_delete(OtherAlloc&& other)
				: alloc{std::forward<OtherAlloc>(other)} {}

			constexpr void operator()(pointer p)
			{
				std::destroy_at(p);
				alloc.deallocate(p, 1);
			}

			constexpr allocator_type& get_allocator()
			{
				return alloc;
			}

			constexpr const allocator_type& get_allocator() const
			{
				return alloc;
			}
		private:
			allocator_type alloc;
	};

	export
	template<class T, class OtherAlloc>
	allocator_delete(OtherAlloc&&) -> allocator_delete<T, typename std::allocator_traits<OtherAlloc>::template rebind_alloc<T>>;

	export
	template<class T, class Alloc>
	allocator_delete(std::reference_wrapper<Alloc>) -> allocator_delete<T, Alloc&>;

	export
	template<class T, class Alloc, class... Args>
	[[nodiscard]] constexpr std::unique_ptr<T, allocator_delete<T, typename std::allocator_traits<Alloc>::template rebind_alloc<T>>>
	allocate_unique(Alloc&& alloc, Args&&... args)
	{
		using traits = typename std::allocator_traits<Alloc>::template rebind_traits<T>;
		typename traits::allocator_type my_alloc {alloc};
		auto hold_deleter = [&my_alloc](auto p) {
			traits::deallocate(my_alloc, p, 1);
		};

		using hold_t = std::unique_ptr<T, decltype(hold_deleter)>;
		hold_t hold{traits::allocate(my_alloc, 1), hold_deleter};
		traits::construct(my_alloc, hold.get(), std::forward<Args>(args)...);
		auto deleter = allocator_delete<T>{my_alloc};
		return std::unique_ptr<T, decltype(deleter)>{hold.release(), std::move(deleter)};
	}

	export
	template<class T, class Alloc, class... Args>
	[[nodiscard]] constexpr std::unique_ptr<T, allocator_delete<T, Alloc&>>
	allocate_unique(std::reference_wrapper<Alloc> alloc, Args&&... args)
	{
		using traits = typename std::allocator_traits<Alloc>::template rebind_traits<T>;
		typename traits::allocator_type my_alloc {alloc};
		auto hold_deleter = [&my_alloc](auto p) {
			traits::deallocate(my_alloc, p, 1);
		};

		using hold_t = std::unique_ptr<T, decltype(hold_deleter)>;
		hold_t hold{traits::allocate(my_alloc, 1), hold_deleter};
		traits::construct(my_alloc, hold.get(), std::forward<Args>(args)...);
		auto deleter = allocator_delete<T>{my_alloc};
		return std::unique_ptr<T, decltype(deleter)>{hold.release(), std::move(deleter)};
	}
}