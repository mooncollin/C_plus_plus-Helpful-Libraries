export module cmoon.utility.copyable_box;

import <concepts>;
import <utility>;
import <initializer_list>;
import <optional>;
import <memory>;

namespace cmoon
{
	export
	template<class T>
		requires(std::copy_constructible<T> && std::is_object_v<T>)
	class copyable_box
	{
		public:
			constexpr copyable_box(const copyable_box& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
				: set{other.set}
			{
				if (set)
				{
					std::construct_at(std::addressof(value), other.value);
				}
			}

			constexpr copyable_box(copyable_box&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
				: set{other.set}
			{
				if (set)
				{
					std::construct_at(std::addressof(value), std::move(other.value));
				}
			}

			template<class... Args>
				requires(std::constructible_from<T, Args...>)
			constexpr explicit copyable_box(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
				: set{true}, value{std::forward<Args>(args)...} {}

			template<class U, class... Args>
				requires(std::constructible_from<T, std::initializer_list<U>, Args...>)
			constexpr explicit copyable_box(std::in_place_t, std::initializer_list<U> ilist, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
				: set{true}, value{ilist, std::forward<Args>(args)...} {}

			constexpr copyable_box() noexcept(std::is_nothrow_default_constructible_v<T>)
				requires(std::default_initializable<T>)
				: copyable_box{std::in_place} {}

			constexpr copyable_box(const T& t) noexcept(std::is_nothrow_copy_constructible_v<T>)
				: copyable_box{std::in_place, t} {}

			constexpr copyable_box(T&& t) noexcept(std::is_nothrow_copy_constructible_v<T>)
				: copyable_box{std::in_place, std::move(t)} {}

			constexpr copyable_box& operator=(const copyable_box& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
			{
				if (this != std::addressof(other))
				{
					if (other)
					{
						emplace(*other);
					}
					else
					{
						reset();
					}
				}

				return *this;
			}

			constexpr copyable_box& operator=(copyable_box&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
			{
				if (this != std::addressof(other))
				{
					if (other)
					{
						emplace(std::move(*other));
					}
					else
					{
						reset();
					}
				}

				return *this;
			}

			~copyable_box() noexcept(std::is_nothrow_destructible_v<T>)
			{
				if (set)
				{
					value.~T();
				}
			}

			constexpr T* operator->() noexcept
			{
				return std::addressof(value);
			}

			constexpr const T* operator->() const noexcept
			{
				return std::addressof(value);
			}

			constexpr T& operator*() & noexcept
			{
				return value;
			}

			constexpr const T& operator*() const& noexcept
			{
				return value;
			}

			constexpr T&& operator*() && noexcept
			{
				return std::move(value);
			}

			constexpr const T&& operator*() const&& noexcept
			{
				return std::move(value);
			}

			constexpr explicit operator bool() const noexcept
			{
				return set;
			}

			constexpr bool has_value() const noexcept
			{
				return set;
			}

			template<class... Args>
			constexpr T& emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
			{
				reset();
				std::construct_at(std::addressof(value), std::forward<Args>(args)...);
				return value;
			}

			template<class U, class... Args>
			constexpr T& emplace(std::initializer_list<U> ilist, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
			{
				reset();
				std::construct_at(std::addressof(value), ilist, std::forward<Args>(args)...);
				return value;
			}
		private:
			bool set {false};
			union { T value; };

			constexpr void reset() noexcept
			{
				if (set)
				{
					value.~T();
					set = false;
				}
			}
	};

	export
	template<class T>
		requires(std::copyable<T> || (std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>))
	class copyable_box<T>
	{
		public:
			constexpr copyable_box(const copyable_box&) noexcept(std::is_nothrow_copy_constructible_v<T>) = default;
			constexpr copyable_box(copyable_box&&) noexcept(std::is_nothrow_move_constructible_v<T>) = default;

			template<class... Args>
				requires(std::constructible_from<T, Args...>)
			constexpr explicit copyable_box(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
				: value{std::forward<Args>(args)...} {}

			template<class U, class... Args>
				requires(std::constructible_from<T, std::initializer_list<U>, Args...>)
			constexpr explicit copyable_box(std::in_place_t, std::initializer_list<U> ilist, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
				: value{ilist, std::forward<Args>(args)...} {}

			constexpr copyable_box() noexcept(std::is_nothrow_default_constructible_v<T>)
				requires(std::default_initializable<T>)
				: copyable_box{std::in_place} {}

			constexpr copyable_box(const T& t) noexcept(std::is_nothrow_copy_constructible_v<T>)
				: copyable_box{std::in_place, t} {}

			constexpr copyable_box(T&& t) noexcept(std::is_nothrow_copy_constructible_v<T>)
				: copyable_box{std::in_place, std::move(t)} {}

			constexpr copyable_box& operator=(const copyable_box& other) noexcept = default;
			constexpr copyable_box& operator=(copyable_box&& other) noexcept = default;

			constexpr T* operator->() noexcept
			{
				return std::addressof(value);
			}

			constexpr const T* operator->() const noexcept
			{
				return std::addressof(value);
			}

			constexpr T& operator*() & noexcept
			{
				return value;
			}

			constexpr const T& operator*() const& noexcept
			{
				return value;
			}

			constexpr T&& operator*() && noexcept
			{
				return std::move(value);
			}

			constexpr const T&& operator*() const&& noexcept
			{
				return std::move(value);
			}

			constexpr explicit operator bool() const noexcept
			{
				return true;
			}

			constexpr bool has_value() const noexcept
			{
				return true;
			}

			template<class... Args>
			constexpr T& emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
			{
				value = T{std::forward<Args>(Args)...};
				return value;
			}

			template<class U, class... Args>
			constexpr T& emplace(std::initializer_list<U> ilist, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
			{
				value = T{ilist, std::forward<Args>(Args)...};
				return value;
			}
		private:
			T value;
	};
}