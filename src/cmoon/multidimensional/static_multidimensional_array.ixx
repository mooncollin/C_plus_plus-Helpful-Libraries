export module cmoon.multidimensional.static_multidimensional_array;

import <initializer_list>;
import <ranges>;
import <array>;
import <type_traits>;

import "cmoon/multidimensional/container_definitions.hpp";

namespace cmoon
{
	template<class T, std::size_t... Dimensions>
		requires(sizeof...(Dimensions) > 0)
	class static_multidimensional_array
	{
		static_assert((... && (Dimensions > 0)));

		static constexpr std::size_t size_ = (... * Dimensions);
		using storage_t = std::array<T, size_>;
		static constexpr auto dimensions_ = std::array{Dimensions...};
		static constexpr auto dimensions_size = sizeof...(Dimensions);

		public:
			using value_type = typename storage_t::value_type;
			using size_type = typename storage_t::size_type;
			using difference_type = typename storage_t::difference_type;
			using reference = typename storage_t::reference;
			using const_reference = typename storage_t::const_reference;
			using pointer = typename storage_t::pointer;
			using const_pointer = typename storage_t::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			constexpr static_multidimensional_array() noexcept(std::is_nothrow_default_constructible_v<value_type>) {}

			template<std::convertible_to<value_type> T2>
			constexpr static_multidimensional_array(const static_multidimensional_array<T2>& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
				: data_{other.data_} {}

			template<std::convertible_to<value_type>... Elements>
			constexpr static_multidimensional_array(Elements&&... elements) noexcept(std::is_nothrow_constructible_v<value_type>)
				: data_{static_cast<value_type>(std::forward<Elements>(elements))...} {}

			constexpr static_multidimensional_array(const static_multidimensional_array&) noexcept(std::is_nothrow_copy_constructible_v<value_type>) = default;
			constexpr static_multidimensional_array(static_multidimensional_array&&) noexcept = default;

			template<std::convertible_to<value_type> T2>
			constexpr static_multidimensional_array& operator=(const static_multidimensional_array<T2>& other) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
			{
				if (this != std::addressof(other))
				{
					data_ = other.data_;
				}

				return *this;
			}

			constexpr static_multidimensional_array& operator=(const static_multidimensional_array&) noexcept(std::is_nothrow_copy_assignable_v<value_type>) = default;
			constexpr static_multidimensional_array& operator=(static_multidimensional_array&&) noexcept = default;

			[[nodiscard]] constexpr const auto& dimensions() const noexcept
			{
				return dimensions_;
			}

			template<class... Indexes>
				requires(sizeof...(Indexes) == dimensions_size)
			constexpr reference get(Indexes&&... indexes) noexcept
			{
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			template<class... Indexes>
				requires(sizeof...(Indexes) == dimensions_size)
			constexpr const_reference get(Indexes&&... indexes) const noexcept
			{
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			constexpr reference operator[](std::size_t index) noexcept
			{
				return data_[index];
			}

			constexpr const_reference operator[](std::size_t index) const noexcept
			{
				return data_[index];
			}

			[[nodiscard]] constexpr const_pointer data() const noexcept
			{
				return data_.data();
			}

			[[nodiscard]] constexpr pointer data() noexcept
			{
				return data_.data();
			}

			[[nodiscard]] constexpr iterator begin() noexcept
			{
				return std::begin(data_);
			}

			[[nodiscard]] constexpr const_iterator begin() const noexcept
			{
				return std::begin(data_);
			}

			[[nodiscard]] constexpr const_iterator cbegin() const noexcept
			{
				return std::cbegin(data_);
			}

			[[nodiscard]] constexpr iterator end() noexcept
			{
				return std::end(data_);
			}

			[[nodiscard]] constexpr const_iterator end() const noexcept
			{
				return std::end(data_);
			}

			[[nodiscard]] constexpr const_iterator cend() const noexcept
			{
				return std::cend(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rbegin() noexcept
			{
				return std::rbegin(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
			{
				return std::rbegin(data_);
			}

			[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
			{
				return std::crbegin(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rend() noexcept
			{
				return std::rend(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rend() const noexcept
			{
				return std::rend(data_);
			}

			[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
			{
				return std::crend(data_);
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return static_multidimensional_array::size_;
			}

			void swap(static_multidimensional_array& other) noexcept
			{
				std::swap(data_, other.data_);
			}
		private:
			storage_t data_ {};

			template<std::size_t Index = 0, class... Indexes>
			[[nodiscard]] static constexpr std::size_t index(std::size_t current_index, Indexes&&... indexes) noexcept
			{
				return current_index * dimension_multiplier<Index + 1>::value
					+ index<Index + 1>(std::forward<Indexes>(indexes)...);
			}

			template<std::size_t TupleIndex>
			[[nodiscard]] static constexpr std::size_t index(std::size_t current_index) noexcept
			{
				return current_index;
			}

			template<std::size_t Index>
			struct dimension_multiplier
			{
				static constexpr std::size_t value {std::get<Index>(dimensions_) * dimension_multiplier<Index + 1>::value};
			};

			template<>
			struct dimension_multiplier<dimensions_size>
			{
				static constexpr std::size_t value {1};
			};
	};
}

namespace std
{
	export
	template<class T, std::size_t... Dimensions>
	void swap(cmoon::static_multidimensional_array<T, Dimensions...>& lhs, cmoon::static_multidimensional_array<T, Dimensions...>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}