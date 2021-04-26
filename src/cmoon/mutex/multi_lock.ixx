export module cmoon.mutex.multi_lock;

import <tuple>;
import <mutex>;
import <type_traits>;
import <system_error>;
import <utility>;
import <shared_mutex>;

import cmoon.tuple;

namespace cmoon
{
	export
	template<class... MutexTypes>
		requires(sizeof...(MutexTypes) > 0)
	class multi_lock
	{
		public:
			using mutex_type = std::tuple<std::add_pointer_t<MutexTypes>...>;

			multi_lock() noexcept
				: locks{std::add_pointer_t<MutexTypes>{nullptr}...} {}

			multi_lock(multi_lock&& other) noexcept
				: locks{std::move(other.locks)}, has_lock{std::exchange(other.has_lock, false)} {}

			multi_lock& operator=(multi_lock&& other) noexcept
			{
				if (this != std::addressof(other))
				{
					locks = std::move(other.locks);
					has_lock = std::exchange(other.has_lock, false);
				}

				return *this;
			}

			explicit multi_lock(MutexTypes&... mutexes)
				requires(sizeof...(MutexTypes) > 0)
				: locks{std::addressof(mutexes)...}, has_lock{true}
			{
				lock_unsafe();
			}

			multi_lock(MutexTypes&... mutexes, std::defer_lock_t) noexcept
				: locks{std::addressof(mutexes)...}, has_lock{false} {}

			multi_lock(MutexTypes&... mutexes, std::try_to_lock_t)
				: locks{std::addressof(mutexes)...}, has_lock{try_lock_unsafe()} {}

			multi_lock(MutexTypes&... mutexes, std::adopt_lock_t) noexcept
				: locks{std::addressof(mutexes)...}, has_lock{true} {}

			~multi_lock()
			{
				if (has_lock)
				{
					unlock_unsafe();
				}
			}

			void unlock()
			{
				if (!has_lock)
				{
					throw std::system_error{std::make_error_code(std::errc::operation_not_permitted)};
				}

				unlock_unsafe();
				has_lock = false;
			}

			void lock()
			{
				if (!has_associated_mutexes())
				{
					throw std::system_error{std::make_error_code(std::errc::operation_not_permitted)};
				}
				else if (has_lock)
				{
					throw std::system_error{std::make_error_code(std::errc::resource_deadlock_would_occur)};
				}


				lock_unsafe();
				has_lock = true;
			}

			bool try_lock()
			{
				if (!has_associated_mutexes())
				{
					throw std::system_error{std::make_error_code(std::errc::operation_not_permitted)};
				}
				else if (has_lock)
				{
					throw std::system_error{std::make_error_code(std::errc::resource_deadlock_would_occur)};
				}

				return has_lock = try_lock_unsafe();
			}

			mutex_type release() noexcept
			{
				const auto rel = locks;
				cmoon::tuples::for_each(locks, [](auto& m) { m = nullptr; });
				return rel;
			}

			mutex_type mutex() const noexcept
			{
				return locks;
			}

			bool owns_lock() const noexcept
			{
				return has_lock;
			}

			explicit operator bool() const noexcept
			{
				return has_lock;
			}

			void swap(multi_lock& other) noexcept
			{
				if (this != std::addressof(other))
				{
					locks.swap(other.locks);
					std::swap(has_lock, other.has_lock);
				}
			}
		private:
			mutex_type locks;
			bool has_lock {false};

			[[nodiscard]] inline bool has_associated_mutexes() const noexcept
			{
				return cmoon::tuples::all_of(locks, [](auto m) { return m != nullptr; });
			}

			inline void unlock_unsafe()
			{
				cmoon::tuples::for_each(locks, [](auto m) { m->unlock(); });
			}

			template<std::size_t... I>
			inline void lock_unsafe_helper(std::index_sequence<I...>)
			{
				if constexpr (sizeof...(MutexTypes) == 1)
				{
					std::get<0>(locks)->lock();
				}
				else
				{
					std::lock(*std::get<I>(locks)...);
				}
			}

			inline void lock_unsafe()
			{
				lock_unsafe_helper(std::index_sequence_for<MutexTypes...>{});
			}

			template<std::size_t... I>
			inline bool try_lock_unsafe(std::index_sequence<I...>)
			{
				if constexpr (sizeof...(MutexTypes) == 1)
				{
					return std::get<0>(locks)->try_lock();
				}
				else
				{
					return std::try_lock(*std::get<I>(locks)...) == -1;
				}
			}

			inline bool try_lock_unsafe()
			{
				return try_lock_unsafe(std::index_sequence_for<MutexTypes...>{});
			}
	};
}

namespace std
{
	export
	template<class... MutexTypes>
	void swap(cmoon::multi_lock<MutexTypes...>& lhs, cmoon::multi_lock<MutexTypes...>& rhs)
	{
		lhs.swap(rhs);
	}
}