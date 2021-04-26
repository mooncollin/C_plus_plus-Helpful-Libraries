export module cmoon.linear.row;

import <span>;
import <algorithm>;

namespace cmoon::linear
{
	export
	template<class T, std::size_t Extent>
	class row : private std::span<T, Extent>
	{
		using base = std::span<T, Extent>;

		public:
			using element_type = typename base::element_type;
			using value_type = typename base::value_type;
			using size_type = typename base::size_type;
			using difference_type = typename base::difference_type;
			using pointer = typename base::pointer;
			using const_pointer = typename base::const_pointer;
			using reference = typename base::reference;
			using const_reference = typename base::const_reference;
			using iterator = typename base::iterator;
			using reverse_iterator = typename base::reverse_iterator;

			constexpr row(pointer first, std::size_t size)
				: base{first, size} {}

			[[nodiscard]] constexpr iterator begin() const noexcept
			{
				return base::begin();
			}

			[[nodiscard]] constexpr iterator end() const noexcept
			{
				return base::end();
			}

			[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
			{
				return base::rbegin();
			}

			[[nodiscard]] constexpr reverse_iterator rend() const noexcept
			{
				return base::rend();
			}

			[[nodiscard]] constexpr reference front() const noexcept
			{
				return base::front();
			}

			[[nodiscard]] constexpr reference back() const noexcept
			{
				return base::back();
			}

			[[nodiscard]] constexpr reference operator[](size_type i) const noexcept
			{
				return base::operator[](i);
			}

			[[nodiscard]] constexpr pointer data() const noexcept
			{
				return base::data();
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return base::size();
			}

			[[nodiscard]] constexpr bool empty() const noexcept
			{
				return base::empty();
			}

			template<size_type Count>
			[[nodiscard]] constexpr row<element_type, Count> first() const noexcept
			{
				return {data(), Count};
			}

			[[nodiscard]] constexpr row<element_type, std::dynamic_extent> first(size_type count) const noexcept
			{
				return {data(), count};
			}

			template<size_type Count>
			[[nodiscard]] constexpr row<element_type, Count> last() const noexcept
			{
				return {data() + (size() - Count), Count};
			}

			[[nodiscard]] constexpr row<element_type, std::dynamic_extent> last(size_type count) const noexcept
			{
				return {data() + (size() - count), count};
			}

			template<size_type Offset, size_type Count = std::dynamic_extent>
			[[nodiscard]] constexpr row<element_type, (Count != std::dynamic_extent) ? Count : (Extent != std::dynamic_extent) ? Extent - Offset : std::dynamic_extent>
				subrow() const noexcept
			{
				return {data() + Offset, (Count == std::dynamic_extent) ? size() - Offset : Count};
			}

			[[nodiscard]] constexpr row<element_type, std::dynamic_extent> subrow(size_type offset, size_type count = std::dynamic_extent) const noexcept
			{
				return {data() + offset, (count == std::dynamic_extent) ? size() - offset : count};
			}

			constexpr row& operator+=(const row& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::plus<>{});
				return *this;
			}

			constexpr row& operator-=(const row& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}

			constexpr row& operator*=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::multiplies{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr row& operator/=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::divides{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr row& operator%=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::modulus{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr value_type sum() const noexcept
			{
				return std::reduce(begin(), end());
			}

			constexpr value_type product() const noexcept
			{
				return std::reduce(begin(), end(), value_type{1}, std::multiplies<>{});
			}

			constexpr void swap(row rhs) noexcept
			{
				std::swap_ranges(begin(), end(), rhs.begin());
			}

			[[nodiscard]] constexpr operator row<const T, Extent>() const noexcept
			{
				return {data(), size()};
			}

			template<std::size_t Extent2>
			[[nodiscard]] friend constexpr bool operator==(const row& lhs, const row<T, Extent2>& rhs) noexcept
			{
				return std::size(lhs) == std::size(rhs) &&
						std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			template<std::size_t Extent2>
			[[nodiscard]] friend constexpr bool operator!=(const row& lhs, const row<T, Extent2>& rhs) noexcept
			{
				return !(lhs == rhs);
			}
	};
}

namespace std
{
	export
	template<class T, std::size_t Extent>
	constexpr void swap(cmoon::linear::row<T, Extent> lhs, cmoon::linear::row<T, Extent> rhs) noexcept
	{
		lhs.swap(rhs);
	}
}