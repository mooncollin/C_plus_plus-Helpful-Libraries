export module cmoon.utility.extent_type;

import <cstddef>;
import <span>;

namespace cmoon
{
	export
	template<std::size_t Ext>
	class extent_type
	{
		public:
			using size_type = std::size_t;

			constexpr extent_type() noexcept = default;

			template<size_type Other>
			constexpr extent_type(extent_type<Other> ext) noexcept
			{
				static_assert(Other == Ext);
			}

			constexpr extent_type(size_type) noexcept
			{
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return Ext;
			}

			constexpr void swap(extent_type& other) noexcept {}
	};

	export
	template<>
	class extent_type<std::dynamic_extent>
	{
		public:
			using size_type = std::size_t;

			template<size_type Other>
			explicit constexpr extent_type(extent_type<Other> ext) noexcept : size_(ext.size()) {}
			explicit constexpr extent_type(size_type size) noexcept : size_(size) {}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return size_;
			}

			constexpr void swap(extent_type& other) noexcept
			{
				std::swap(size_, other.size_);
			}
		private:
			size_type size_;
	};
}