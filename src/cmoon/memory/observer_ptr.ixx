export module cmoon.memory.observer_ptr;

import <functional>;
import <concepts>;
import <type_traits>;

namespace cmoon
{
	export
	template<class W>
	class observer_ptr
	{
		public:
			using element_type = W;

			constexpr observer_ptr() noexcept = default;
			constexpr observer_ptr(std::nullptr_t) noexcept {}

			constexpr observer_ptr& operator=(std::nullptr_t) noexcept
			{
				ptr_ = nullptr;
				return *this;
			}

			explicit observer_ptr(element_type* p) noexcept
				: ptr_{p} {}

			template<class W2>
				requires(std::convertible_to<W2*, element_type*>)
			observer_ptr(observer_ptr<W2> other) noexcept
				: ptr_{other.get()} {}

			[[nodiscard]] constexpr element_type* get() const noexcept
			{
				return ptr_;
			}

			constexpr element_type* release() noexcept
			{
				return std::exchange(ptr_, nullptr);
			}

			constexpr void reset(element_type* p = nullptr) noexcept
			{
				ptr_ = p;
			}

			constexpr void swap(observer_ptr& other) noexcept
			{
				std::swap(ptr_, other.ptr_);
			}

			[[nodiscard]] constexpr explicit operator bool() const noexcept
			{
				return ptr_ != nullptr;
			}

			constexpr std::add_lvalue_reference_t<element_type> operator*() const
			{
				return *ptr_;
			}

			constexpr element_type* operator->() const noexcept
			{
				return ptr_;
			}

			constexpr explicit operator element_type*() const noexcept
			{
				return ptr_;
			}

			template<class W2>
			[[nodiscard]] friend constexpr bool operator==(const observer_ptr& p1, const observer_ptr<W2>& p2)
			{
				return p1.get() == p2.get();
			}

			template<class W2>
			[[nodiscard]] friend constexpr bool operator!=(const observer_ptr& p1, const observer_ptr<W2>& p2)
			{
				return !(p1 == p2);
			}

			[[nodiscard]] friend constexpr bool operator==(const observer_ptr& p1, std::nullptr_t)
			{
				return !p1;
			}

			[[nodiscard]] friend constexpr bool operator==(std::nullptr_t, const observer_ptr& p1)
			{
				return !p1;
			}

			[[nodiscard]] friend constexpr bool operator!=(const observer_ptr& p1, std::nullptr_t)
			{
				return static_cast<bool>(p1);
			}

			[[nodiscard]] friend constexpr bool operator!=(std::nullptr_t, const observer_ptr& p1)
			{
				return static_cast<bool>(p1);
			}

			template<class W2>
			[[nodiscard]] friend constexpr bool operator<(const observer_ptr& p1, const observer_ptr<W2>& p2)
			{
				using W3 = std::common_type_t<W, W2>;
				return std::less<W3>{}(p1.get(), p2.get());
			}

			template<class W2>
			[[nodiscard]] friend constexpr bool operator>(const observer_ptr& p1, const observer_ptr<W2>& p2)
			{
				return p2 < p1;
			}

			template<class W2>
			[[nodiscard]] friend constexpr bool operator<=(const observer_ptr& p1, const observer_ptr<W2>& p2)
			{
				return !(p2 < p1);
			}

			template<class W2>
			[[nodiscard]] friend constexpr bool operator>=(const observer_ptr& p1, const observer_ptr<W2>& p2)
			{
				return !(p1 < p2);
			}
		private:
			element_type* ptr_{nullptr};
	};

	export
	template<class W>
	[[nodiscard]] constexpr observer_ptr<W> make_observer(W* p) noexcept
	{
		return observer_ptr<W>{p};
}

namespace std
{
	export
	template<class W>
	void swap(cmoon::observer_ptr<W>& lhs, cmoon::observer_ptr<W>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	export
	template<class W>
	struct hash<cmoon::observer_ptr<W>> : public std::hash<W*> {};
}