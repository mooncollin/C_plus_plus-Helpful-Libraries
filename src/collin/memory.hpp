#pragma once

#include <memory>
#include <map>
#include <type_traits>

namespace collin
{
	namespace memory
	{
		template<class T, class Allocator = std::allocator<T>>
		class Recycler
		{
			public:
				using value_type = T;
				using pointer = T*;
				using const_pointer = const pointer;
				using reference = value_type&;
				using const_reference = const reference;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;

				template<typename U>
				struct rebind
				{
					using other = Recycler<U>;
				};

				Recycler()
				{
					initializeMemory();
				}

				Recycler(const Recycler&) = default;
				Recycler(Recycler&&) = default;
				Recycler& operator=(const Recycler&) = default;
				Recycler& operator=(Recycler&&) = default;
				
				template<typename U>
				Recycler(const Recycler<U>& other)
				{
					initializeMemory();
				}

				Recycler(size_type amount_until_release)
					: _until_release(amount_until_release)
				{
					initializeMemory();
				}

				Recycler(size_type amount_until_release, const Allocator& alloc)
					: _until_release(amount_until_release), alloc(alloc)
				{
					initializeMemory();
				}

				pointer allocate(std::size_t n)
				{
					const auto location = storedMemory->find(n);
					if(location != std::end(*storedMemory))
					{
						auto ptr = location->second;
						storedMemory->erase(location);
						_currently_held -= n;
						return ptr;
					}

					return alloc.allocate(n);
				}

				void deallocate(pointer p, size_type n)
				{
					if(n + _currently_held >= _until_release
						|| n <= sizeof(typename memory_storage::allocator_type::value_type))
					{
						alloc.deallocate(p, n);
						return;
					}

					storedMemory->emplace(n, p);
					_currently_held += n;
				}

				void force_deallocate(pointer p, size_type n)
				{
					alloc.deallocate(p, n);
				}

				size_type until_release() const noexcept
				{
					return _until_release;
				}

				void until_release(size_type n)
				{
					_until_release = n;
					if(_currently_held >= _until_release)
					{
						free(_until_release);
					}
				}

				size_type currently_held() const noexcept
				{
					return _currently_held;
				}

				void currently_held(size_type n)
				{
					_currently_held = n;
				}

				void free(size_type n)
				{
					if(n >= _currently_held)
					{
						for(auto [n, ptr] : storedMemory)
						{
							force_deallocate(ptr, n);
						}
					}
					else
					{
						while(_currently_held >= n)
						{
							const auto key = _currently_held - n;
							const auto location = storedMemory.lower_bound(key);
							if(location != std::end(storedMemory))
							{
								force_deallocate(*location, key);
							}
							else
							{
								break;
							}
						}
					}
				}
			private:
				using memory_storage = std::multimap<size_type, pointer>;
				size_type _until_release = 1024 * 500;
				size_type _currently_held = 0;
				Allocator alloc;
				std::shared_ptr<memory_storage> storedMemory;

				void initializeMemory()
				{
					storedMemory = std::shared_ptr<memory_storage>(new memory_storage(), [&](memory_storage* storage) {
						for (auto [n, ptr] : *storage)
						{
							alloc.deallocate(ptr, n);
						}

						delete storage;
					});
				}
		};
	}
}