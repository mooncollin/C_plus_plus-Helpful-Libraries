export module cmoon.utility.not_null;

import <concepts>;
import <utility>;
import <iostream>;

import cmoon.test.assert;

namespace cmoon
{
	template<class T>
	concept comparable_to_null = 
		requires(T t)
	{
		{ t != nullptr } -> std::convertible_to<bool>;
	};

	export
	template<comparable_to_null T>
		requires(!std::same_as<T, std::nullptr_t>)
	class not_null
	{
		public:
			template<std::convertible_to<T> U>
			explicit constexpr not_null(U&& u)
				: ptr_{std::forward<U>(u)}
			{
				cmoon::test::assert_not_equal(ptr_, nullptr);
			}

			explicit constexpr not_null(T t)
				: ptr_{std::move(t)}
			{
				cmoon::test::assert_not_equal(ptr_, nullptr);
			}

			template<std::convertible_to<T> U>
			constexpr not_null(const not_null<U>& other)
				: not_null(other.get()) {}

			not_null(const not_null&) = default;
			not_null& operator=(const not_null&) = default;

			constexpr std::conditional_t<std::copy_constructible<T>, T, const T&> get() const
			{
				return ptr_;
			}

			constexpr operator T() const
			{
				return get();
			}

			constexpr decltype(auto) operator->() const
			{
				return get();
			}

			constexpr decltype(auto) operator*() const
			{
				return *get();
			}

			not_null(std::nullptr_t) = delete;
			not_null& operator=(std::nullptr_t) = delete;

			not_null& operator++() = delete;
			not_null& operator--() = delete;
			not_null operator++(int) = delete;
			not_null operator--(int) = delete;
			not_null& operator+=(std::ptrdiff_t) = delete;
			not_null& operator-=(std::ptrdiff_t) = delete;
			void operator[](std::ptrdiff_t) const = delete;

			template<class U>
			constexpr auto friend operator==(const not_null& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() == rhs.get()))
			{
				return lhs.get() == rhs.get();
			}

			template<class U>
			constexpr auto friend operator!=(const not_null& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() != rhs.get()))
			{
				return lhs.get() != rhs.get();
			}

			template<class U>
			constexpr auto friend operator<(const not_null& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() < rhs.get()))
			{
				return lhs.get() < rhs.get();
			}

			template<class U>
			constexpr auto friend operator<=(const not_null& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() <= rhs.get()))
			{
				return lhs.get() <= rhs.get();
			}

			template<class U>
			constexpr auto friend operator>(const not_null& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() > rhs.get()))
			{
				return lhs.get() > rhs.get();
			}

			template<class U>
			constexpr auto friend operator>=(const not_null& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() >= rhs.get()))
			{
				return lhs.get() >= rhs.get();
			}
		private:
			T ptr_;
	};

	export
	template<class T>
	not_null(T) -> not_null<T>;

	export
	template<class T, class U>
	std::ptrdiff_t operator-(const not_null<T>&, const not_null<U>&) = delete;

	export
	template<class T>
	not_null<T> operator-(const not_null<T>&, std::ptrdiff_t) = delete;

	export
	template<class T>
	not_null<T> operator+(const not_null<T>&, std::ptrdiff_t) = delete;

	export
	template<class T>
	not_null<T> operator+(std::ptrdiff_t, const not_null<T>&) = delete;

	export
	template<class T>
	std::ostream& operator<<(std::ostream& os, const not_null<T>& val)
	{
		os << val.get();
		return os;
	}
}

namespace std
{
	export
	template<class T>
	struct hash<cmoon::not_null<T>>
	{
		std::size_t operator()(const cmoon::not_null<T>& value) const
		{
			return hash<T>{}(value.get());
		}
	};
}