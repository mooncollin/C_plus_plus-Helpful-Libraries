export module cmoon.memory.allocate_unique;

import <memory>;
import <utility>;

namespace cmoon
{
	export
	template<typename Allocator>
	struct allocator_deleter
	{
		Allocator alloc;

		template<class T>
		void operator()(T* p)
		{
			p->~T();
			alloc.deallocate(p, 1);
		}
	};

	export
	template<typename T, typename Allocator, typename... Args>
	[[nodiscard]] auto allocate_unique(const Allocator& alloc, Args&&... args)
	{
		using rebind = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
		using deleter = allocator_deleter<rebind>;

		deleter d {rebind{alloc}};
		auto* p = d.alloc.allocate(1);

		try
		{
			p = new(p) T{std::forward<Args>(args)...};
		}
		catch (...)
		{
			d.alloc.deallocate(p, 1);
			throw;
		}

		return std::unique_ptr<T, deleter>{p, d};
	}
}